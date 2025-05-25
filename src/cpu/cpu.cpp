#include "cpu.hpp"
#include "../mmu.hpp"
#include "../logger.hpp"

void CPU::ExecuteOpcode(std::uint8_t opcode)
{
  PLOG(plog::info) << "Executing OPCode: 0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex
                   << static_cast<int>(opcode);
}

void CPU::Halt()
{
  halted = true;
}

void CPU::Tick()
{
  std::uint8_t opcode = mmu.Get(registers.PC);
  ExecuteOpcode(opcode);
  ++registers.PC;

  if (registers.PC >= 0x100)
  {
    Halt();
  }
}

void CPU::Registers::SetFlag(int bit, bool value)
{
  std::uint8_t bitMask = (1 << bit);

  if (value == true)
  {
    F |= bitMask;
  }
  else
  {
    F &= (~bitMask);
  }
}

bool CPU::Registers::GetFlag(int bit)
{
  return (F & (1 << bit));
}

void CPU::Registers::SetZeroFlag(bool value)
{
  SetFlag(BITPOS_ZERO_FLAG, value);
}

void CPU::Registers::SetSubtractionFlag(bool value)
{
  SetFlag(BITPOS_SUBTRACTION_FLAG, value);
}

void CPU::Registers::SetHalfCarryFlag(bool value)
{
  SetFlag(BITPOS_HALF_CARRY_FLAG, value);
}

void CPU::Registers::SetCarryFlag(bool value)
{
  SetFlag(BITPOS_CARRY_FLAG, value);
}

bool CPU::Registers::GetZeroFlag()
{
  return GetFlag(BITPOS_ZERO_FLAG);
}

bool CPU::Registers::GetSubtractionFlag()
{
  return GetFlag(BITPOS_SUBTRACTION_FLAG);
}

bool CPU::Registers::GetHalfCarryFlag()
{
  return GetFlag(BITPOS_HALF_CARRY_FLAG);
}

bool CPU::Registers::GetCarryFlag()
{
  return GetFlag(BITPOS_CARRY_FLAG);
}

/**********************************************************************************/
/* Operations                                                                     */
/**********************************************************************************/
void CPU::INC_R8(std::uint8_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(reg & 0x0F);
  ++reg;
  registers.SetZeroFlag(reg == 0);
}

void CPU::DEC_R8(std::uint8_t& reg)
{
  registers.SetSubtractionFlag(true);
  --reg;
  registers.SetZeroFlag(reg == 0);
  registers.SetHalfCarryFlag((reg & 0x0F) == 0x0F);
}

void CPU::ADD_HL_R16(std::uint16_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(IsHalfCarryOverflowADD16(registers.HL, reg));
  registers.SetCarryFlag(IsCarryOverflowADD16(registers.HL, reg));

  registers.HL += reg;
}

std::uint8_t CPU::GetN8()
{
  return mmu.Get(registers.PC++);
}

std::uint16_t CPU::GetN16()
{
  std::uint8_t low = mmu.Get(registers.PC++);
  std::uint8_t high = mmu.Get(registers.PC++);

  return (high << 8) & low;
}

std::int8_t CPU::GetE8()
{
  return mmu.Get(registers.PC++);
}

void CPU::JR_CC_E8(bool condition)
{
  std::int8_t addressOffset = GetE8();

  if (condition)
  {
    registers.PC += addressOffset;
  }
}

bool CPU::IsHalfCarryOverflowADD16(std::uint16_t a, std::uint16_t b)
{
  // Overflow from bit 11
  return ((a & 0x07FF) + (b & 0x07FF)) > 0x07FF;
}

bool CPU::IsCarryOverflowADD16(std::uint16_t a, std::uint16_t b)
{
  return ((a + b) > 0xFFFF);
}

/**********************************************************************************/
/* Opcodes                                                                        */
/**********************************************************************************/
void CPU::NOP()
{
}

void CPU::LD_BC_N16()
{
  registers.BC = GetN16();
}

void CPU::LD_dBC_A()
{
  mmu.Set(registers.BC, registers.A);
}

void CPU::INC_BC()
{
  ++registers.BC;
}

void CPU::INC_B()
{
  INC_R8(registers.B);
}

void CPU::DEC_B()
{
  DEC_R8(registers.B);
}

void CPU::LD_B_N8()
{
  registers.B = GetN8();
}

void CPU::RLCA()
{
  registers.SetZeroFlag(false);
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);
  registers.SetCarryFlag(registers.A & 0x80);

  registers.A = registers.A << 1;
  registers.A = registers.A ^ registers.GetCarryFlag();
}

void CPU::LD_DN16_SP()
{
  std::uint16_t baseAddr = GetN16();
  mmu.Set(baseAddr, registers.SP & 0xFF);
  mmu.Set(baseAddr + 1, (registers.SP >> 8));
}

void CPU::ADD_HL_BC()
{
  ADD_HL_R16(registers.BC);
}

void CPU::LD_A_DBC()
{
  registers.A = mmu.Get(registers.BC);
}

void CPU::DEC_BC()
{
  --registers.BC;
}

void CPU::INC_C()
{
  INC_R8(registers.C);
}

void CPU::DEC_C()
{
  DEC_R8(registers.C);
}

void CPU::LD_C_N8()
{
  registers.C = GetN8();
}

void CPU::RRCA()
{
  registers.SetZeroFlag(false);
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);
  registers.SetCarryFlag(registers.A & 1);

  registers.A = (registers.A >> 1);
}

void CPU::STOP_N8()
{
  std::uint8_t nextByte = mmu.Get(++registers.PC);
  if (nextByte)
  {
    throw std::runtime_error{"Unexpected Stop Opcode."};
  }
}

void CPU::LD_DE_N16()
{
  registers.DE = GetN16();
}

void CPU::LD_dDE_A()
{
  mmu.Set(registers.DE, registers.A);
}

void CPU::INC_DE()
{
  ++registers.DE;
}

void CPU::INC_D()
{
  INC_R8(registers.D);
}

void CPU::DEC_D()
{
  DEC_R8(registers.D);
}

void CPU::LD_D_N8()
{
  registers.D = GetN8();
}

void CPU::RLA()
{
  registers.SetZeroFlag(false);
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);

  bool oldCarryFlag = registers.GetCarryFlag();
  registers.SetCarryFlag(registers.A & 0x80);
  registers.A = (registers.A << 1);
  registers.A = registers.A | oldCarryFlag;
}

void CPU::JR_E8()
{
  std::int8_t addressOffset = GetE8();
  registers.PC += addressOffset;
}

void CPU::ADD_HL_DE()
{
  ADD_HL_R16(registers.DE);
}

void CPU::LD_A_dDE()
{
  registers.A = mmu.Get(registers.DE);
}

void CPU::DEC_DE()
{
  --registers.DE;
}

void CPU::INC_E()
{
  INC_R8(registers.E);
}

void CPU::LD_E_N8()
{
  registers.E = GetN8();
}

void CPU::RAA()
{
  registers.SetZeroFlag(false);
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);

  bool oldCarryFlag = registers.GetCarryFlag();
  registers.SetCarryFlag(registers.A & 1);
  registers.A = (registers.A >> 1);
  registers.A = registers.A | (oldCarryFlag << 7);
}

void CPU::JR_NZ_E8()
{
  JR_CC_E8(!registers.GetZeroFlag());
}

void CPU::LD_HL_N16()
{
  registers.HL = GetN16();
}

void CPU::LD_dHLi_A()
{
  mmu.Set(registers.HL++, registers.A);
}

void CPU::INC_HL()
{
  ++registers.HL;
}

void CPU::INC_H()
{
  INC_R8(registers.H);
}

void CPU::DEC_H()
{
  DEC_R8(registers.H);
}

void CPU::LD_H_N8()
{
  registers.H = GetN8();
}

void CPU::DAA()
{
  std::uint8_t adjustment = 0;

  if (registers.GetSubtractionFlag())
  {
    if (registers.GetHalfCarryFlag())
    {
      adjustment += 0x6;
    }
    if (registers.GetCarryFlag())
    {
      adjustment += 0x60;
    }
    registers.A -= adjustment;
  }
  else
  {
    if (registers.GetHalfCarryFlag() || ((registers.A & 0xF) > 0x9))
    {
      adjustment += 0x6;
    }
    if (registers.GetCarryFlag() || (registers.A > 0x99))
    {
      registers.SetCarryFlag(true);
      adjustment += 0x60;
    }
    registers.A += adjustment;
  }
}

void CPU::JR_Z_E8()
{
  JR_CC_E8(registers.GetZeroFlag());
}

void CPU::ADD_HL_HL()
{
  ADD_HL_R16(registers.HL);
}

void CPU::LD_A_dHLi()
{
  registers.A = mmu.Get(registers.HL++);
}

void CPU::DEC_HL()
{
  --registers.HL;
}

void CPU::INC_L()
{
  INC_R8(registers.L);
}

void CPU::DEC_L()
{
  DEC_R8(registers.L);
}

void CPU::LD_L_N8()
{
  registers.L = GetN8();
}

void CPU::CPL()
{
  registers.SetSubtractionFlag(true);
  registers.SetHalfCarryFlag(true);
  registers.A = ~registers.A;
}

void CPU::JR_NC_E8()
{
  JR_CC_E8(!registers.GetCarryFlag());
}

void CPU::LD_SP_N16()
{
  registers.SP = GetN16();
}

void CPU::LD_dHLd_A()
{
  mmu.Set(registers.HL--, registers.A);
}

void CPU::INC_SP()
{
  ++registers.SP;
}

void CPU::INC_dHL()
{
  mmu.Set(registers.HL, mmu.Get(registers.HL) + 1);
}

void CPU::DEC_dHL()
{
  mmu.Set(registers.HL, mmu.Get(registers.HL) - 1);
}

void CPU::LD_dHL_N8()
{
  mmu.Set(registers.HL, GetN8());
}

void CPU::SCF()
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);
  registers.SetCarryFlag(true);
}

void CPU::JR_C_E8()
{
  JR_CC_E8(registers.GetCarryFlag());
}

void CPU::ADD_HL_SP()
{
  ADD_HL_R16(registers.SP);
}

void CPU::LD_A_dHLd()
{
  registers.A = mmu.Get(registers.HL--);
}

void CPU::DEC_SP()
{
  --registers.SP;
}

void CPU::INC_A()
{
  INC_R8(registers.A);
}

void CPU::DEC_A()
{
  DEC_R8(registers.A);
}

void CPU::LD_A_N8()
{
  registers.A = GetN8();
}

void CPU::CCF()
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);

  registers.SetHalfCarryFlag(!registers.GetHalfCarryFlag());
}

void CPU::LD_B_B()
{
}

void CPU::LD_B_C()
{
}

void CPU::LD_B_D()
{
}

void CPU::LD_B_E()
{
}

void CPU::LD_B_H()
{
}

void CPU::LD_B_L()
{
}

void CPU::LD_B_dHL()
{
}

void CPU::LD_B_A()
{
}

void CPU::LD_C_B()
{
}

void CPU::LD_C_C()
{
}

void CPU::LD_C_D()
{
}

void CPU::LD_C_E()
{
}

void CPU::LD_C_H()
{
}

void CPU::LD_C_L()
{
}

void CPU::LD_C_dHL()
{
}

void CPU::LD_C_A()
{
}

void CPU::LD_D_B()
{
}

void CPU::LD_D_C()
{
}

void CPU::LD_D_D()
{
}

void CPU::LD_D_E()
{
}

void CPU::LD_D_H()
{
}

void CPU::LD_D_L()
{
}

void CPU::LD_D_dHL()
{
}

void CPU::LD_D_A()
{
}

void CPU::LD_E_B()
{
}

void CPU::LD_E_C()
{
}

void CPU::LD_E_D()
{
}

void CPU::LD_E_E()
{
}

void CPU::LD_E_H()
{
}

void CPU::LD_E_L()
{
}

void CPU::LD_E_dHL()
{
}

void CPU::LD_E_A()
{
}

void CPU::LD_H_B()
{
}

void CPU::LD_H_C()
{
}

void CPU::LD_H_D()
{
}

void CPU::LD_H_E()
{
}

void CPU::LD_H_H()
{
}

void CPU::LD_H_L()
{
}

void CPU::LD_H_dHL()
{
}

void CPU::LD_H_A()
{
}

void CPU::LD_L_B()
{
}

void CPU::LD_L_C()
{
}

void CPU::LD_L_D()
{
}

void CPU::LD_L_E()
{
}

void CPU::LD_L_H()
{
}

void CPU::LD_L_L()
{
}

void CPU::LD_L_dHL()
{
}

void CPU::LD_L_A()
{
}

void CPU::LD_dHL_B()
{
}

void CPU::LD_dHL_C()
{
}

void CPU::LD_dHL_D()
{
}

void CPU::LD_dHL_E()
{
}

void CPU::LD_dHL_H()
{
}

void CPU::LD_dHL_L()
{
}

void CPU::HALT()
{
}

void CPU::LD_dHL_A()
{
}

void CPU::LD_A_B()
{
}

void CPU::LD_A_C()
{
}

void CPU::LD_A_D()
{
}

void CPU::LD_A_E()
{
}

void CPU::LD_A_H()
{
}

void CPU::LD_A_L()
{
}

void CPU::LD_A_dHL()
{
}

void CPU::LD_A_A()
{
}

void CPU::ADD_A_B()
{
}

void CPU::ADD_A_C()
{
}

void CPU::ADD_A_D()
{
}

void CPU::ADD_A_E()
{
}

void CPU::ADD_A_H()
{
}

void CPU::ADD_A_L()
{
}

void CPU::ADD_A_dHL()
{
}

void CPU::ADD_A_A()
{
}

void CPU::ADC_A_B()
{
}

void CPU::ADC_A_C()
{
}

void CPU::ADC_A_D()
{
}

void CPU::ADC_A_E()
{
}

void CPU::ADC_A_H()
{
}

void CPU::ADC_A_L()
{
}

void CPU::ADC_A_dHL()
{
}

void CPU::ADC_A_A()
{
}

void CPU::SUB_A_B()
{
}

void CPU::SUB_A_C()
{
}

void CPU::SUB_A_D()
{
}

void CPU::SUB_A_E()
{
}

void CPU::SUB_A_H()
{
}

void CPU::SUB_A_L()
{
}

void CPU::SUB_A_dHL()
{
}

void CPU::SUB_A_A()
{
}

void CPU::SBC_A_B()
{
}

void CPU::SBC_A_C()
{
}

void CPU::SBC_A_D()
{
}

void CPU::SBC_A_E()
{
}

void CPU::SBC_A_H()
{
}

void CPU::SBC_A_L()
{
}

void CPU::SBC_A_dHL()
{
}

void CPU::SBC_A_A()
{
}

void CPU::AND_A_B()
{
}

void CPU::AND_A_C()
{
}

void CPU::AND_A_D()
{
}

void CPU::AND_A_E()
{
}

void CPU::AND_A_H()
{
}

void CPU::AND_A_L()
{
}

void CPU::AND_A_dHL()
{
}

void CPU::AND_A_A()
{
}

void CPU::XOR_A_B()
{
}

void CPU::XOR_A_C()
{
}

void CPU::XOR_A_D()
{
}

void CPU::XOR_A_E()
{
}

void CPU::XOR_A_H()
{
}

void CPU::XOR_A_L()
{
}

void CPU::XOR_A_dHL()
{
}

void CPU::XOR_A_A()
{
}

void CPU::OR_A_B()
{
}

void CPU::OR_A_C()
{
}

void CPU::OR_A_D()
{
}

void CPU::OR_A_E()
{
}

void CPU::OR_A_H()
{
}

void CPU::OR_A_L()
{
}

void CPU::OR_A_dHL()
{
}

void CPU::OR_A_A()
{
}

void CPU::CP_A_B()
{
}

void CPU::CP_A_C()
{
}

void CPU::CP_A_D()
{
}

void CPU::CP_A_E()
{
}

void CPU::CP_A_H()
{
}

void CPU::CP_A_L()
{
}

void CPU::CP_A_dHL()
{
}

void CPU::CP_A_A()
{
}

void CPU::RET_NZ()
{
}

void CPU::POP_BC()
{
}

void CPU::JP_NZ_A16()
{
}

void CPU::JP_A16()
{
}

void CPU::CALL_NZ_A16()
{
}

void CPU::PUSH_BC()
{
}

void CPU::ADD_A_N8()
{
}

void CPU::RST_00()
{
}

void CPU::RET_Z()
{
}

void CPU::RET()
{
}

void CPU::JP_Z_A16()
{
}

void CPU::CALL_Z_A16()
{
}

void CPU::CALL_A16()
{
}

void CPU::ADC_A_N8()
{
}

void CPU::RST_08()
{
}

void CPU::RET_NC()
{
}

void CPU::POP_DE()
{
}

void CPU::JP_NC_A16()
{
}

void CPU::CALL_NC_A16()
{
}

void CPU::PUSH_DE()
{
}

void CPU::SUB_A_N8()
{
}

void CPU::RST_10()
{
}

void CPU::RET_C()
{
}

void CPU::RETI()
{
}

void CPU::JP_C_A16()
{
}

void CPU::CALL_C_A16()
{
}

void CPU::RST_18()
{
}

void CPU::SBC_A_N8()
{
}

void CPU::LDH_dA8_A()
{
}

void CPU::POP_HL()
{
}

void CPU::LDH_dC_A()
{
}

void CPU::PUSH_HL()
{
}

void CPU::AND_A_N8()
{
}

void CPU::RST_20()
{
}

void CPU::ADD_SP_E8()
{
}

void CPU::JP_HL()
{
}

void CPU::LD_dA16_A()
{
}

void CPU::XOR_A_N8()
{
}

void CPU::RST_28()
{
}

void CPU::LDH_A_dA8()
{
}

void CPU::POP_AF()
{
}

void CPU::LDH_A_dC()
{
}

void CPU::DI()
{
}

void CPU::PUSH_AF()
{
}

void CPU::OR_A_N8()
{
}

void CPU::RST_30()
{
}

void CPU::LD_HL_SP_p_E8()
{
}

void CPU::LD_SP_HL()
{
}

void CPU::LD_A_dA16()
{
}

void CPU::EI()
{
}

void CPU::CP_A_N8()
{
}

void CPU::RST_38()
{
}

void CPU::RLC_B()
{
}

void CPU::RLC_C()
{
}

void CPU::RLC_D()
{
}

void CPU::RLC_E()
{
}

void CPU::RLC_H()
{
}

void CPU::RLC_L()
{
}

void CPU::RLC_dHL()
{
}

void CPU::RLC_A()
{
}

void CPU::RRC_B()
{
}

void CPU::RRC_C()
{
}

void CPU::RRC_D()
{
}

void CPU::RRC_E()
{
}

void CPU::RRC_H()
{
}

void CPU::RRC_L()
{
}

void CPU::RRC_dHL()
{
}

void CPU::RRC_A()
{
}

void CPU::RL_B()
{
}

void CPU::RL_C()
{
}

void CPU::RL_D()
{
}

void CPU::RL_E()
{
}

void CPU::RL_H()
{
}

void CPU::RL_L()
{
}

void CPU::RL_dHL()
{
}

void CPU::RL_A()
{
}

void CPU::RR_B()
{
}

void CPU::RR_C()
{
}

void CPU::RR_D()
{
}

void CPU::RR_E()
{
}

void CPU::RR_H()
{
}

void CPU::RR_L()
{
}

void CPU::RR_dHL()
{
}

void CPU::RR_A()
{
}

void CPU::SLA_B()
{
}

void CPU::SLA_C()
{
}

void CPU::SLA_D()
{
}

void CPU::SLA_E()
{
}

void CPU::SLA_H()
{
}

void CPU::SLA_L()
{
}

void CPU::SLA_dHL()
{
}

void CPU::SLA_A()
{
}

void CPU::SRA_B()
{
}

void CPU::SRA_C()
{
}

void CPU::SRA_D()
{
}

void CPU::SRA_E()
{
}

void CPU::SRA_H()
{
}

void CPU::SRA_L()
{
}

void CPU::SRA_dHL()
{
}

void CPU::SRA_A()
{
}

void CPU::SWAP_B()
{
}

void CPU::SWAP_C()
{
}

void CPU::SWAP_D()
{
}

void CPU::SWAP_E()
{
}

void CPU::SWAP_H()
{
}

void CPU::SWAP_L()
{
}

void CPU::SWAP_dHL()
{
}

void CPU::SWAP_A()
{
}

void CPU::SRL_B()
{
}

void CPU::SRL_C()
{
}

void CPU::SRL_D()
{
}

void CPU::SRL_E()
{
}

void CPU::SRL_H()
{
}

void CPU::SRL_L()
{
}

void CPU::SRL_dHL()
{
}

void CPU::SRL_A()
{
}

void CPU::BIT_0_B()
{
}

void CPU::BIT_0_C()
{
}

void CPU::BIT_0_D()
{
}

void CPU::BIT_0_E()
{
}

void CPU::BIT_0_H()
{
}

void CPU::BIT_0_L()
{
}

void CPU::BIT_0_dHL()
{
}

void CPU::BIT_0_A()
{
}

void CPU::BIT_1_B()
{
}

void CPU::BIT_1_C()
{
}

void CPU::BIT_1_D()
{
}

void CPU::BIT_1_E()
{
}

void CPU::BIT_1_H()
{
}

void CPU::BIT_1_L()
{
}

void CPU::BIT_1_dHL()
{
}

void CPU::BIT_1_A()
{
}

void CPU::BIT_2_B()
{
}

void CPU::BIT_2_C()
{
}

void CPU::BIT_2_D()
{
}

void CPU::BIT_2_E()
{
}

void CPU::BIT_2_H()
{
}

void CPU::BIT_2_L()
{
}

void CPU::BIT_2_dHL()
{
}

void CPU::BIT_2_A()
{
}

void CPU::BIT_3_B()
{
}

void CPU::BIT_3_C()
{
}

void CPU::BIT_3_D()
{
}

void CPU::BIT_3_E()
{
}

void CPU::BIT_3_H()
{
}

void CPU::BIT_3_L()
{
}

void CPU::BIT_3_dHL()
{
}

void CPU::BIT_3_A()
{
}

void CPU::BIT_4_B()
{
}

void CPU::BIT_4_C()
{
}

void CPU::BIT_4_D()
{
}

void CPU::BIT_4_E()
{
}

void CPU::BIT_4_H()
{
}

void CPU::BIT_4_L()
{
}

void CPU::BIT_4_dHL()
{
}

void CPU::BIT_4_A()
{
}

void CPU::BIT_5_B()
{
}

void CPU::BIT_5_C()
{
}

void CPU::BIT_5_D()
{
}

void CPU::BIT_5_E()
{
}

void CPU::BIT_5_H()
{
}

void CPU::BIT_5_L()
{
}

void CPU::BIT_5_dHL()
{
}

void CPU::BIT_5_A()
{
}

void CPU::BIT_6_B()
{
}

void CPU::BIT_6_C()
{
}

void CPU::BIT_6_D()
{
}

void CPU::BIT_6_E()
{
}

void CPU::BIT_6_H()
{
}

void CPU::BIT_6_L()
{
}

void CPU::BIT_6_dHL()
{
}

void CPU::BIT_6_A()
{
}

void CPU::BIT_7_B()
{
}

void CPU::BIT_7_C()
{
}

void CPU::BIT_7_D()
{
}

void CPU::BIT_7_E()
{
}

void CPU::BIT_7_H()
{
}

void CPU::BIT_7_L()
{
}

void CPU::BIT_7_dHL()
{
}

void CPU::BIT_7_A()
{
}

void CPU::RES_0_B()
{
}

void CPU::RES_0_C()
{
}

void CPU::RES_0_D()
{
}

void CPU::RES_0_E()
{
}

void CPU::RES_0_H()
{
}

void CPU::RES_0_L()
{
}

void CPU::RES_0_dHL()
{
}

void CPU::RES_0_A()
{
}

void CPU::RES_1_B()
{
}

void CPU::RES_1_C()
{
}

void CPU::RES_1_D()
{
}

void CPU::RES_1_E()
{
}

void CPU::RES_1_H()
{
}

void CPU::RES_1_L()
{
}

void CPU::RES_1_dHL()
{
}

void CPU::RES_1_A()
{
}

void CPU::RES_2_B()
{
}

void CPU::RES_2_C()
{
}

void CPU::RES_2_D()
{
}

void CPU::RES_2_E()
{
}

void CPU::RES_2_H()
{
}

void CPU::RES_2_L()
{
}

void CPU::RES_2_dHL()
{
}

void CPU::RES_2_A()
{
}

void CPU::RES_3_B()
{
}

void CPU::RES_3_C()
{
}

void CPU::RES_3_D()
{
}

void CPU::RES_3_E()
{
}

void CPU::RES_3_H()
{
}

void CPU::RES_3_L()
{
}

void CPU::RES_3_dHL()
{
}

void CPU::RES_3_A()
{
}

void CPU::RES_4_B()
{
}

void CPU::RES_4_C()
{
}

void CPU::RES_4_E()
{
}

void CPU::RES_4_H()
{
}

void CPU::RES_4_L()
{
}

void CPU::RES_4_dHL()
{
}

void CPU::RES_4_A()
{
}

void CPU::RES_5_B()
{
}

void CPU::RES_5_C()
{
}

void CPU::RES_5_D()
{
}

void CPU::RES_5_E()
{
}

void CPU::RES_5_H()
{
}

void CPU::RES_5_L()
{
}

void CPU::RES_5_dHL()
{
}

void CPU::RES_5_A()
{
}

void CPU::RES_6_B()
{
}

void CPU::RES_6_C()
{
}

void CPU::RES_6_D()
{
}

void CPU::RES_6_E()
{
}

void CPU::RES_6_H()
{
}

void CPU::RES_6_L()
{
}

void CPU::RES_6_dHL()
{
}

void CPU::RES_6_A()
{
}

void CPU::RES_7_B()
{
}

void CPU::RES_7_C()
{
}

void CPU::RES_7_D()
{
}

void CPU::RES_7_E()
{
}

void CPU::RES_7_H()
{
}

void CPU::RES_7_L()
{
}

void CPU::RES_7_dHL()
{
}

void CPU::RES_7_A()
{
}

void CPU::SET_0_B()
{
}

void CPU::SET_0_C()
{
}

void CPU::SET_0_D()
{
}

void CPU::SET_0_E()
{
}

void CPU::SET_0_H()
{
}

void CPU::SET_0_L()
{
}

void CPU::SET_0_dHL()
{
}

void CPU::SET_0_A()
{
}

void CPU::SET_1_B()
{
}

void CPU::SET_1_C()
{
}

void CPU::SET_1_D()
{
}

void CPU::SET_1_E()
{
}

void CPU::SET_1_H()
{
}

void CPU::SET_1_L()
{
}

void CPU::SET_1_dHL()
{
}

void CPU::SET_1_A()
{
}

void CPU::SET_2_B()
{
}

void CPU::SET_2_C()
{
}

void CPU::SET_2_D()
{
}

void CPU::SET_2_E()
{
}

void CPU::SET_2_H()
{
}

void CPU::SET_2_L()
{
}

void CPU::SET_2_dHL()
{
}

void CPU::SET_2_A()
{
}

void CPU::SET_3_B()
{
}

void CPU::SET_3_C()
{
}

void CPU::SET_3_D()
{
}

void CPU::SET_3_E()
{
}

void CPU::SET_3_H()
{
}

void CPU::SET_3_L()
{
}

void CPU::SET_3_dHL()
{
}

void CPU::SET_3_A()
{
}

void CPU::SET_4_B()
{
}

void CPU::SET_4_C()
{
}

void CPU::SET_4_D()
{
}

void CPU::SET_4_E()
{
}

void CPU::SET_4_H()
{
}

void CPU::SET_4_L()
{
}

void CPU::SET_4_dHL()
{
}

void CPU::SET_4_A()
{
}

void CPU::SET_5_B()
{
}

void CPU::SET_5_C()
{
}

void CPU::SET_5_D()
{
}

void CPU::SET_5_E()
{
}

void CPU::SET_5_H()
{
}

void CPU::SET_5_L()
{
}

void CPU::SET_5_dHL()
{
}

void CPU::SET_5_A()
{
}

void CPU::SET_6_B()
{
}

void CPU::SET_6_C()
{
}

void CPU::SET_6_D()
{
}

void CPU::SET_6_E()
{
}

void CPU::SET_6_H()
{
}

void CPU::SET_6_L()
{
}

void CPU::SET_6_dHL()
{
}

void CPU::SET_6_A()
{
}

void CPU::SET_7_B()
{
}

void CPU::SET_7_C()
{
}

void CPU::SET_7_D()
{
}

void CPU::SET_7_E()
{
}

void CPU::SET_7_H()
{
}

void CPU::SET_7_L()
{
}

void CPU::SET_7_dHL()
{
}

void CPU::SET_7_A()
{
}

#include "cpu.hpp"
#include "../mmu.hpp"
#include "../logger.hpp"

void CPU::ExecuteOpcode(std::uint8_t opcode)
{
  PLOG(plog::info) << "Executing OPCode(PC: " << std::uppercase << std::setfill('0') << std::setw(2) << std::hex
                   << registers.PC << "): 0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex
                   << static_cast<int>(opcode);
  //(this->*opcodeTable[opcode])();
}

void CPU::ExecuteExtendedOpcode(std::uint8_t opcode)
{
  PLOG(plog::info) << "Executing extended OPCode: 0xCB" << std::uppercase << std::setfill('0') << std::setw(2)
                   << std::hex << static_cast<int>(opcode);
  //(this->*extendedOpcodeTable[opcode])();
}

void CPU::Halt()
{
  halted = true;
}

void CPU::Tick()
{
  std::uint8_t opcode = mmu.Get(registers.PC);
  if (opcode == extendedOpcodePrefix)
  {
    ++registers.PC;
    ExecuteExtendedOpcode(opcode);
  }
  else
  {
    ExecuteOpcode(opcode);
  }
  ++registers.PC;

  if (mmu.Get(0xff02) == 0x81)
  {
    char c = mmu.Get(0xff01);
    printf("%c", c);
    mmu.Set(0xff02, 0x0);
  }

  if (registers.PC >= 0x200)
  {
    HALT();
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

void CPU::ADD_A_R8(const std::uint8_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(IsHalfCarryOverflow8(registers.A, reg));
  registers.SetCarryFlag(IsCarryOverflow8(registers.A, reg));

  registers.A += reg;
  registers.SetZeroFlag(registers.A == 0);
}

void CPU::ADD_HL_R16(const std::uint16_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(IsHalfCarryOverflow16(registers.HL, reg));
  registers.SetCarryFlag(IsCarryOverflow16(registers.HL, reg));

  registers.HL += reg;
}

void CPU::ADC_A_R8(const std::uint8_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(IsHalfCarryOverflow8(registers.A, reg + registers.GetCarryFlag()));
  registers.SetCarryFlag(IsCarryOverflow8(registers.A, reg + registers.GetCarryFlag()));

  registers.A += reg + registers.GetCarryFlag();
  registers.SetZeroFlag(registers.A == 0);
}

void CPU::SUB_A_R8(const std::uint8_t& reg)
{
  registers.SetSubtractionFlag(true);
  registers.SetHalfCarryFlag(IsHalfCarryUnderflow8(registers.A, reg));
  registers.SetCarryFlag(IsCarryUnderflow8(registers.A, reg));

  registers.A -= reg;

  registers.SetZeroFlag(registers.A == 0);
}

void CPU::SBC_A_R8(const std::uint8_t& reg)
{
  registers.SetSubtractionFlag(true);
  registers.SetHalfCarryFlag(IsHalfCarryUnderflow8(registers.A, reg + registers.GetCarryFlag()));
  registers.SetCarryFlag(IsCarryUnderflow8(registers.A, reg + registers.GetCarryFlag()));

  registers.A -= reg + registers.GetCarryFlag();

  registers.SetZeroFlag(registers.A == 0);
}

void CPU::AND_A_R8(const std::uint8_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(true);
  registers.SetCarryFlag(false);

  registers.A &= reg;

  registers.SetZeroFlag(registers.A == 0);
}

void CPU::XOR_A_R8(const std::uint8_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);
  registers.SetCarryFlag(false);

  registers.A ^= reg;

  registers.SetZeroFlag(registers.A == 0);
}

void CPU::OR_A_R8(const std::uint8_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);
  registers.SetCarryFlag(false);

  registers.A |= reg;

  registers.SetZeroFlag(registers.A == 0);
}

void CPU::CP_A_R8(const std::uint8_t& reg)
{
  registers.SetSubtractionFlag(true);
  registers.SetHalfCarryFlag(IsHalfCarryUnderflow8(registers.A, reg));
  registers.SetCarryFlag(IsCarryUnderflow8(registers.A, reg));

  registers.SetZeroFlag(registers.A == 0);
}

void CPU::RST_VEC(const std::uint16_t)
{
  // TODO IMPLEMENT INTERRUPTION HANDLING
}

void CPU::RL_R8(std::uint8_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);

  bool msb = reg & (1 << 7);
  reg <<= 1;
  reg &= registers.GetCarryFlag();

  registers.SetCarryFlag(msb);
  registers.SetZeroFlag(reg == 0);
}

void CPU::RR_R8(std::uint8_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);

  bool lsb = reg & 1;
  reg >>= 1;
  reg &= (registers.GetCarryFlag() << 7);

  registers.SetCarryFlag(lsb);
  registers.SetZeroFlag(reg == 0);
}

void CPU::SLA_R8(std::uint8_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);
  registers.SetCarryFlag(reg & (1 << 7));

  reg <<= 1;

  registers.SetZeroFlag(reg == 0);
}

void CPU::SRA_R8(std::uint8_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);
  registers.SetCarryFlag(reg & 1);

  bool msb = reg & (1 << 7);
  reg >>= 1;
  reg &= (msb << 7);

  registers.SetZeroFlag(reg == 0);
}

void CPU::SRL_R8(std::uint8_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);
  registers.SetCarryFlag(reg & 1);

  reg >>= 1;

  registers.SetZeroFlag(reg == 0);
}

void CPU::RLC_R8(std::uint8_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);

  bool msb = reg & (1 << 7);
  reg <<= 1;
  reg |= msb;

  registers.SetCarryFlag(msb);
  registers.SetZeroFlag(reg == 0);
}

void CPU::RRC_R8(std::uint8_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);

  bool lsb = reg & 1;
  reg >>= 1;
  reg |= (lsb << 7);

  registers.SetCarryFlag(lsb);
  registers.SetZeroFlag(reg == 0);
}

void CPU::SWAP_R8(std::uint8_t& reg)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(false);
  registers.SetCarryFlag(false);

  std::uint8_t lowNibble = (reg & 0x0F);
  std::uint8_t highNibble = (reg >> 4);
  reg = ((lowNibble << 4) & highNibble);

  registers.SetZeroFlag(reg == 0);
}

void CPU::BIT_R8(std::uint8_t& reg, int bitNumber)
{
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(true);
  registers.SetZeroFlag((reg & (1 << bitNumber)) == 0);
}

void CPU::RES_R8(std::uint8_t& reg, int bitNumber)
{
  reg &= ~(1 << bitNumber);
}

void CPU::SET_R8(std::uint8_t& reg, int bitNumber)
{
  reg |= (1 << bitNumber);
}

void CPU::PUSH_N16(const std::uint16_t& value)
{
  --registers.SP;
  mmu.Set(registers.SP, (value >> 8) & 0xFF);
  --registers.SP;
  mmu.Set(registers.SP, value & 0xFF);
}

std::uint16_t CPU::POP_N16()
{
  std::uint8_t lowByte = mmu.Get(registers.SP);
  ++registers.SP;
  std::uint8_t highByte = mmu.Get(registers.SP);
  ++registers.SP;

  return ((highByte << 8) & lowByte);
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

bool CPU::IsHalfCarryOverflow8(const std::uint8_t a, const std::uint8_t b)
{
  return ((a & 0x08) + (b & 0x08)) > 0x08;
}

bool CPU::IsCarryOverflow8(const std::uint8_t a, const std::uint8_t b)
{
  return (a + b) > 0xFF;
}

bool CPU::IsHalfCarryOverflow16(const std::uint16_t a, const std::uint16_t b)
{
  // Overflow from bit 11
  return ((a & 0x07FF) + (b & 0x07FF)) > 0x07FF;
}

bool CPU::IsCarryOverflow16(const std::uint16_t a, const std::uint16_t b)
{
  return ((a + b) > 0xFFFF);
}

bool CPU::IsHalfCarryUnderflow8(const std::uint8_t a, const std::uint8_t b)
{
  return ((a & 0x10) < (b & 0x10));
}

bool CPU::IsCarryUnderflow8(const std::uint8_t a, const std::uint8_t b)
{
  return (b > a);
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

void CPU::DEC_E()
{
  DEC_R8(registers.E);
}

void CPU::LD_E_N8()
{
  registers.E = GetN8();
}

void CPU::RRA()
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
  NOP();
}

void CPU::LD_B_C()
{
  registers.B = registers.C;
}

void CPU::LD_B_D()
{
  registers.B = registers.D;
}

void CPU::LD_B_E()
{
  registers.B = registers.E;
}

void CPU::LD_B_H()
{
  registers.B = registers.H;
}

void CPU::LD_B_L()
{
  registers.B = registers.L;
}

void CPU::LD_B_dHL()
{
  registers.B = mmu.Get(registers.HL);
}

void CPU::LD_B_A()
{
  registers.B = registers.A;
}

void CPU::LD_C_B()
{
  registers.C = registers.B;
}

void CPU::LD_C_C()
{
  NOP();
}

void CPU::LD_C_D()
{
  registers.C = registers.D;
}

void CPU::LD_C_E()
{
  registers.C = registers.E;
}

void CPU::LD_C_H()
{
  registers.C = registers.H;
}

void CPU::LD_C_L()
{
  registers.C = registers.L;
}

void CPU::LD_C_dHL()
{
  registers.C = mmu.Get(registers.HL);
}

void CPU::LD_C_A()
{
  registers.C = registers.A;
}

void CPU::LD_D_B()
{
  registers.D = registers.B;
}

void CPU::LD_D_C()
{
  registers.D = registers.C;
}

void CPU::LD_D_D()
{
  NOP();
}

void CPU::LD_D_E()
{
  registers.D = registers.E;
}

void CPU::LD_D_H()
{
  registers.D = registers.H;
}

void CPU::LD_D_L()
{
  registers.D = registers.L;
}

void CPU::LD_D_dHL()
{
  registers.D = mmu.Get(registers.HL);
}

void CPU::LD_D_A()
{
  registers.D = registers.A;
}

void CPU::LD_E_B()
{
  registers.E = registers.B;
}

void CPU::LD_E_C()
{
  registers.E = registers.C;
}

void CPU::LD_E_D()
{
  registers.E = registers.D;
}

void CPU::LD_E_E()
{
  NOP();
}

void CPU::LD_E_H()
{
  registers.E = registers.H;
}

void CPU::LD_E_L()
{
  registers.E = registers.L;
}

void CPU::LD_E_dHL()
{
  registers.E = mmu.Get(registers.HL);
}

void CPU::LD_E_A()
{
  registers.E = registers.A;
}

void CPU::LD_H_B()
{
  registers.H = registers.B;
}

void CPU::LD_H_C()
{
  registers.H = registers.C;
}

void CPU::LD_H_D()
{
  registers.H = registers.D;
}

void CPU::LD_H_E()
{
  registers.H = registers.E;
}

void CPU::LD_H_H()
{
  NOP();
}

void CPU::LD_H_L()
{
  registers.H = registers.L;
}

void CPU::LD_H_dHL()
{
  registers.H = mmu.Get(registers.HL);
}

void CPU::LD_H_A()
{
  registers.H = registers.A;
}

void CPU::LD_L_B()
{
  registers.L = registers.B;
}

void CPU::LD_L_C()
{
  registers.L = registers.C;
}

void CPU::LD_L_D()
{
  registers.L = registers.D;
}

void CPU::LD_L_E()
{
  registers.L = registers.E;
}

void CPU::LD_L_H()
{
  registers.L = registers.H;
}

void CPU::LD_L_L()
{
  NOP();
}

void CPU::LD_L_dHL()
{
  registers.L = mmu.Get(registers.HL);
}

void CPU::LD_L_A()
{
  registers.L = registers.A;
}

void CPU::LD_dHL_B()
{
  mmu.Set(registers.HL, registers.B);
}

void CPU::LD_dHL_C()
{
  mmu.Set(registers.HL, registers.C);
}

void CPU::LD_dHL_D()
{
  mmu.Set(registers.HL, registers.D);
}

void CPU::LD_dHL_E()
{
  mmu.Set(registers.HL, registers.E);
}

void CPU::LD_dHL_H()
{
  mmu.Set(registers.HL, registers.H);
}

void CPU::LD_dHL_L()
{
  mmu.Set(registers.HL, registers.L);
}

void CPU::HALT()
{
  halted = true;
  // TODO IMPLEMENT INTERRUPT HANDLING
}

void CPU::LD_dHL_A()
{
  mmu.Set(registers.HL, registers.A);
}

void CPU::LD_A_B()
{
  registers.A = registers.B;
}

void CPU::LD_A_C()
{
  registers.A = registers.C;
}

void CPU::LD_A_D()
{
  registers.A = registers.D;
}

void CPU::LD_A_E()
{
  registers.A = registers.E;
}

void CPU::LD_A_H()
{
  registers.A = registers.H;
}

void CPU::LD_A_L()
{
  registers.A = registers.L;
}

void CPU::LD_A_dHL()
{
  registers.A = mmu.Get(registers.HL);
}

void CPU::LD_A_A()
{
  NOP();
}

void CPU::ADD_A_B()
{
  ADD_A_R8(registers.B);
}

void CPU::ADD_A_C()
{
  ADD_A_R8(registers.C);
}

void CPU::ADD_A_D()
{
  ADD_A_R8(registers.D);
}

void CPU::ADD_A_E()
{
  ADD_A_R8(registers.E);
}

void CPU::ADD_A_H()
{
  ADD_A_R8(registers.H);
}

void CPU::ADD_A_L()
{
  ADD_A_R8(registers.L);
}

void CPU::ADD_A_dHL()
{
  ADD_A_R8(mmu.Get(registers.HL));
}

void CPU::ADD_A_A()
{
  ADD_A_R8(registers.A);
}

void CPU::ADC_A_B()
{
  ADC_A_R8(registers.B);
}

void CPU::ADC_A_C()
{
  ADC_A_R8(registers.C);
}

void CPU::ADC_A_D()
{
  ADC_A_R8(registers.D);
}

void CPU::ADC_A_E()
{
  ADC_A_R8(registers.E);
}

void CPU::ADC_A_H()
{
  ADC_A_R8(registers.H);
}

void CPU::ADC_A_L()
{
  ADC_A_R8(registers.L);
}

void CPU::ADC_A_dHL()
{
  ADC_A_R8(mmu.Get(registers.HL));
}

void CPU::ADC_A_A()
{
  ADC_A_R8(registers.A);
}

void CPU::SUB_A_B()
{
  SUB_A_R8(registers.B);
}

void CPU::SUB_A_C()
{
  SUB_A_R8(registers.C);
}

void CPU::SUB_A_D()
{
  SUB_A_R8(registers.D);
}

void CPU::SUB_A_E()
{
  SUB_A_R8(registers.E);
}

void CPU::SUB_A_H()
{
  SUB_A_R8(registers.H);
}

void CPU::SUB_A_L()
{
  SUB_A_R8(registers.L);
}

void CPU::SUB_A_dHL()
{
  SUB_A_R8(mmu.Get(registers.HL));
}

void CPU::SUB_A_A()
{
  SUB_A_R8(registers.A);
}

void CPU::SBC_A_B()
{
  SBC_A_R8(registers.B);
}

void CPU::SBC_A_C()
{
  SBC_A_R8(registers.C);
}

void CPU::SBC_A_D()
{
  SBC_A_R8(registers.D);
}

void CPU::SBC_A_E()
{
  SBC_A_R8(registers.E);
}

void CPU::SBC_A_H()
{
  SBC_A_R8(registers.H);
}

void CPU::SBC_A_L()
{
  SBC_A_R8(registers.L);
}

void CPU::SBC_A_dHL()
{
  SBC_A_R8(mmu.Get(registers.HL));
}

void CPU::SBC_A_A()
{
  SBC_A_R8(registers.A);
}

void CPU::AND_A_B()
{
  AND_A_R8(registers.B);
}

void CPU::AND_A_C()
{
  AND_A_R8(registers.C);
}

void CPU::AND_A_D()
{
  AND_A_R8(registers.D);
}

void CPU::AND_A_E()
{
  AND_A_R8(registers.E);
}

void CPU::AND_A_H()
{
  AND_A_R8(registers.H);
}

void CPU::AND_A_L()
{
  AND_A_R8(registers.L);
}

void CPU::AND_A_dHL()
{
  AND_A_R8(mmu.Get(registers.HL));
}

void CPU::AND_A_A()
{
  AND_A_R8(registers.A);
}

void CPU::XOR_A_B()
{
  XOR_A_R8(registers.B);
}

void CPU::XOR_A_C()
{
  XOR_A_R8(registers.C);
}

void CPU::XOR_A_D()
{
  XOR_A_R8(registers.D);
}

void CPU::XOR_A_E()
{
  XOR_A_R8(registers.E);
}

void CPU::XOR_A_H()
{
  XOR_A_R8(registers.H);
}

void CPU::XOR_A_L()
{
  XOR_A_R8(registers.L);
}

void CPU::XOR_A_dHL()
{
  XOR_A_R8(mmu.Get(registers.HL));
}

void CPU::XOR_A_A()
{
  XOR_A_R8(registers.A);
}

void CPU::OR_A_B()
{
  OR_A_R8(registers.B);
}

void CPU::OR_A_C()
{
  OR_A_R8(registers.C);
}

void CPU::OR_A_D()
{
  OR_A_R8(registers.D);
}

void CPU::OR_A_E()
{
  OR_A_R8(registers.E);
}

void CPU::OR_A_H()
{
  OR_A_R8(registers.H);
}

void CPU::OR_A_L()
{
  OR_A_R8(registers.L);
}

void CPU::OR_A_dHL()
{
  OR_A_R8(mmu.Get(registers.HL));
}

void CPU::OR_A_A()
{
  OR_A_R8(registers.A);
}

void CPU::CP_A_B()
{
  CP_A_R8(registers.B);
}

void CPU::CP_A_C()
{
  CP_A_R8(registers.C);
}

void CPU::CP_A_D()
{
  CP_A_R8(registers.D);
}

void CPU::CP_A_E()
{
  CP_A_R8(registers.E);
}

void CPU::CP_A_H()
{
  CP_A_R8(registers.H);
}

void CPU::CP_A_L()
{
  CP_A_R8(registers.L);
}

void CPU::CP_A_dHL()
{
  CP_A_R8(mmu.Get(registers.HL));
}

void CPU::CP_A_A()
{
  CP_A_R8(registers.A);
}

void CPU::RET_NZ()
{
  if (!registers.GetZeroFlag())
  {
    RET();
  }
}

void CPU::POP_BC()
{
  registers.BC = POP_N16();
}

void CPU::JP_NZ_A16()
{
  if (!registers.GetZeroFlag())
  {
    JP_A16();
  }
}

void CPU::JP_A16()
{
  registers.PC = GetN16() - 1;
}

void CPU::CALL_NZ_A16()
{
  if (!registers.GetZeroFlag())
  {
    CALL_A16();
  }
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
  if (registers.GetZeroFlag())
  {
    RET();
  }
}

void CPU::RET()
{
  std::uint16_t returnAddress = POP_N16();
  registers.PC = returnAddress - 1;
}

void CPU::JP_Z_A16()
{
  if (registers.GetZeroFlag())
  {
    JP_A16();
  }
}

void CPU::CALL_Z_A16()
{
  if (registers.GetZeroFlag())
  {
    CALL_A16();
  }
}

void CPU::CALL_A16()
{
  PUSH_N16(registers.PC + 2);
  JP_A16();
}

void CPU::ADC_A_N8()
{
  ADC_A_R8(GetN8());
}

void CPU::RST_08()
{
  RST_VEC(0x08);
}

void CPU::RET_NC()
{
  if (!registers.GetCarryFlag())
  {
    RET();
  }
}

void CPU::POP_DE()
{
  registers.DE = POP_N16();
}

void CPU::JP_NC_A16()
{
  if (!registers.GetCarryFlag())
  {
    JP_A16();
  }
}

void CPU::CALL_NC_A16()
{
  if (!registers.GetCarryFlag())
  {
    CALL_A16();
  }
}

void CPU::PUSH_DE()
{
  PUSH_N16(registers.DE);
}

void CPU::SUB_A_N8()
{
  SUB_A_R8(GetN8());
}

void CPU::RST_10()
{
  RST_VEC(0x10);
}

void CPU::RET_C()
{
  if (registers.GetCarryFlag())
  {
    RET();
  }
}

void CPU::RETI()
{
  EI();
  RET();
}

void CPU::JP_C_A16()
{
  if (registers.GetCarryFlag())
  {
    JP_A16();
  }
}

void CPU::CALL_C_A16()
{
  if (registers.GetCarryFlag())
  {
    CALL_A16();
  }
}

void CPU::SBC_A_N8()
{
  SBC_A_R8(GetN8());
}

void CPU::RST_18()
{
  RST_VEC(0x18);
}

void CPU::LDH_dA8_A()
{
  std::uint16_t highAddress = 0xFF00 + GetN8();
  mmu.Set(highAddress, registers.A);
}

void CPU::POP_HL()
{
  registers.HL = POP_N16();
}

void CPU::LDH_dC_A()
{
  std::uint16_t highAddress = 0xFF00 + registers.C;
  mmu.Set(highAddress, registers.A);
}

void CPU::PUSH_HL()
{
  PUSH_N16(registers.HL);
}

void CPU::AND_A_N8()
{
  AND_A_R8(GetN8());
}

void CPU::RST_20()
{
  RST_VEC(0x20);
}

void CPU::ADD_SP_E8()
{
  std::int8_t offset = GetE8();

  registers.SetZeroFlag(false);
  registers.SetSubtractionFlag(false);
  registers.SetHalfCarryFlag(IsHalfCarryOverflow16(registers.SP, offset));
  registers.SetCarryFlag(IsCarryOverflow16(registers.SP, offset));

  registers.SP += offset;
}

void CPU::JP_HL()
{
  registers.PC = registers.HL - 1;
}

void CPU::LD_dA16_A()
{
  mmu.Set(GetN16(), registers.A);
}

void CPU::XOR_A_N8()
{
  XOR_A_R8(GetN8());
}

void CPU::RST_28()
{
  RST_VEC(0x28);
}

void CPU::LDH_A_dA8()
{
  std::uint16_t highAddress = 0xFF00 + GetN8();
  registers.A = mmu.Get(highAddress);
}

void CPU::POP_AF()
{
  registers.AF = POP_N16();
}

void CPU::LDH_A_dC()
{
  std::uint16_t highAddress = 0xFF00 + registers.C;
  registers.A = mmu.Get(highAddress);
}

void CPU::DI()
{
  registers.IME = false;
}

void CPU::PUSH_AF()
{
  PUSH_N16(registers.AF);
}

void CPU::OR_A_N8()
{
  OR_A_R8(GetN8());
}

void CPU::RST_30()
{
  RST_VEC(0x30);
}

void CPU::LD_HL_SP_p_E8()
{
  registers.HL = registers.SP + GetE8();
}

void CPU::LD_SP_HL()
{
  registers.SP = registers.HL;
}

void CPU::LD_A_dA16()
{
  registers.A = mmu.Get(GetN16());
}

void CPU::EI()
{
  registers.IME = true;
}

void CPU::CP_A_N8()
{
  CP_A_R8(GetN8());
}

void CPU::RST_38()
{
  RST_VEC(0x38);
}

/**********************************************************************************/
/* Prefixed Opcodes                                                               */
/**********************************************************************************/
void CPU::RLC_B()
{
  RLC_R8(registers.B);
}

void CPU::RLC_C()
{
  RLC_R8(registers.C);
}

void CPU::RLC_D()
{
  RLC_R8(registers.D);
}

void CPU::RLC_E()
{
  RLC_R8(registers.E);
}

void CPU::RLC_H()
{
  RLC_R8(registers.H);
}

void CPU::RLC_L()
{
  RLC_R8(registers.L);
}

void CPU::RLC_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  RLC_R8(byte);
  mmu.Set(registers.HL, byte);
}

void CPU::RLC_A()
{
  RLC_R8(registers.A);
}

void CPU::RRC_B()
{
  RRC_R8(registers.B);
}

void CPU::RRC_C()
{
  RRC_R8(registers.C);
}

void CPU::RRC_D()
{
  RRC_R8(registers.D);
}

void CPU::RRC_E()
{
  RRC_R8(registers.E);
}

void CPU::RRC_H()
{
  RRC_R8(registers.H);
}

void CPU::RRC_L()
{
  RRC_R8(registers.L);
}

void CPU::RRC_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  RRC_R8(byte);
  mmu.Set(registers.HL, byte);
}

void CPU::RRC_A()
{
  RRC_R8(registers.A);
}

void CPU::RL_B()
{
  RL_R8(registers.B);
}

void CPU::RL_C()
{
  RL_R8(registers.C);
}

void CPU::RL_D()
{
  RL_R8(registers.D);
}

void CPU::RL_E()
{
  RL_R8(registers.E);
}

void CPU::RL_H()
{
  RL_R8(registers.H);
}

void CPU::RL_L()
{
  RL_R8(registers.L);
}

void CPU::RL_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  RL_R8(byte);
  mmu.Set(registers.HL, byte);
}

void CPU::RL_A()
{
  RL_R8(registers.A);
}

void CPU::RR_B()
{
  RR_R8(registers.B);
}

void CPU::RR_C()
{
  RR_R8(registers.C);
}

void CPU::RR_D()
{
  RR_R8(registers.D);
}

void CPU::RR_E()
{
  RR_R8(registers.E);
}

void CPU::RR_H()
{
  RR_R8(registers.H);
}

void CPU::RR_L()
{
  RR_R8(registers.L);
}

void CPU::RR_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  RR_R8(byte);
  mmu.Set(registers.HL, byte);
}

void CPU::RR_A()
{
  RR_R8(registers.A);
}

void CPU::SLA_B()
{
  SLA_R8(registers.B);
}

void CPU::SLA_C()
{
  SLA_R8(registers.C);
}

void CPU::SLA_D()
{
  SLA_R8(registers.D);
}

void CPU::SLA_E()
{
  SLA_R8(registers.E);
}

void CPU::SLA_H()
{
  SLA_R8(registers.H);
}

void CPU::SLA_L()
{
  SLA_R8(registers.L);
}

void CPU::SLA_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  SLA_R8(byte);
  mmu.Set(registers.HL, byte);
}

void CPU::SLA_A()
{
  SLA_R8(registers.A);
}

void CPU::SRA_B()
{
  SRA_R8(registers.B);
}

void CPU::SRA_C()
{
  SRA_R8(registers.C);
}

void CPU::SRA_D()
{
  SRA_R8(registers.D);
}

void CPU::SRA_E()
{
  SRA_R8(registers.E);
}

void CPU::SRA_H()
{
  SRA_R8(registers.H);
}

void CPU::SRA_L()
{
  SRA_R8(registers.L);
}

void CPU::SRA_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  SRA_R8(byte);
  mmu.Set(registers.HL, byte);
}

void CPU::SRA_A()
{
  SRA_R8(registers.A);
}

void CPU::SWAP_B()
{
  SWAP_R8(registers.B);
}

void CPU::SWAP_C()
{
  SWAP_R8(registers.C);
}

void CPU::SWAP_D()
{
  SWAP_R8(registers.D);
}

void CPU::SWAP_E()
{
  SWAP_R8(registers.E);
}

void CPU::SWAP_H()
{
  SWAP_R8(registers.H);
}

void CPU::SWAP_L()
{
  SWAP_R8(registers.L);
}

void CPU::SWAP_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  SWAP_R8(byte);
  mmu.Set(registers.HL, byte);
}

void CPU::SWAP_A()
{
  SWAP_R8(registers.A);
}

void CPU::SRL_B()
{
  SRL_R8(registers.B);
}

void CPU::SRL_C()
{
  SRL_R8(registers.C);
}

void CPU::SRL_D()
{
  SRL_R8(registers.D);
}

void CPU::SRL_E()
{
  SRL_R8(registers.E);
}

void CPU::SRL_H()
{
  SRL_R8(registers.H);
}

void CPU::SRL_L()
{
  SRL_R8(registers.L);
}

void CPU::SRL_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  SRL_R8(byte);
  mmu.Set(registers.HL, byte);
}

void CPU::SRL_A()
{
  SRL_R8(registers.A);
}

void CPU::BIT_0_B()
{
  BIT_R8(registers.B, 0);
}

void CPU::BIT_0_C()
{
  BIT_R8(registers.C, 0);
}

void CPU::BIT_0_D()
{
  BIT_R8(registers.D, 0);
}

void CPU::BIT_0_E()
{
  BIT_R8(registers.E, 0);
}

void CPU::BIT_0_H()
{
  BIT_R8(registers.H, 0);
}

void CPU::BIT_0_L()
{
  BIT_R8(registers.L, 0);
}

void CPU::BIT_0_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  BIT_R8(byte, 0);
  mmu.Set(registers.HL, byte);
}

void CPU::BIT_0_A()
{
  BIT_R8(registers.A, 0);
}

void CPU::BIT_1_B()
{
  BIT_R8(registers.B, 1);
}

void CPU::BIT_1_C()
{
  BIT_R8(registers.C, 1);
}

void CPU::BIT_1_D()
{
  BIT_R8(registers.D, 1);
}

void CPU::BIT_1_E()
{
  BIT_R8(registers.E, 1);
}

void CPU::BIT_1_H()
{
  BIT_R8(registers.H, 1);
}

void CPU::BIT_1_L()
{
  BIT_R8(registers.L, 1);
}

void CPU::BIT_1_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  BIT_R8(byte, 1);
  mmu.Set(registers.HL, byte);
}

void CPU::BIT_1_A()
{
  BIT_R8(registers.A, 1);
}

void CPU::BIT_2_B()
{
  BIT_R8(registers.B, 2);
}

void CPU::BIT_2_C()
{
  BIT_R8(registers.C, 2);
}

void CPU::BIT_2_D()
{
  BIT_R8(registers.D, 2);
}

void CPU::BIT_2_E()
{
  BIT_R8(registers.E, 2);
}

void CPU::BIT_2_H()
{
  BIT_R8(registers.H, 2);
}

void CPU::BIT_2_L()
{
  BIT_R8(registers.L, 2);
}

void CPU::BIT_2_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  BIT_R8(byte, 2);
  mmu.Set(registers.HL, byte);
}

void CPU::BIT_2_A()
{
  BIT_R8(registers.A, 2);
}

void CPU::BIT_3_B()
{
  BIT_R8(registers.B, 3);
}

void CPU::BIT_3_C()
{
  BIT_R8(registers.C, 3);
}

void CPU::BIT_3_D()
{
  BIT_R8(registers.D, 3);
}

void CPU::BIT_3_E()
{
  BIT_R8(registers.E, 3);
}

void CPU::BIT_3_H()
{
  BIT_R8(registers.H, 3);
}

void CPU::BIT_3_L()
{
  BIT_R8(registers.L, 3);
}

void CPU::BIT_3_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  BIT_R8(byte, 3);
  mmu.Set(registers.HL, byte);
}

void CPU::BIT_3_A()
{
  BIT_R8(registers.A, 3);
}

void CPU::BIT_4_B()
{
  BIT_R8(registers.B, 4);
}

void CPU::BIT_4_C()
{
  BIT_R8(registers.C, 4);
}

void CPU::BIT_4_D()
{
  BIT_R8(registers.D, 4);
}

void CPU::BIT_4_E()
{
  BIT_R8(registers.E, 4);
}

void CPU::BIT_4_H()
{
  BIT_R8(registers.H, 4);
}

void CPU::BIT_4_L()
{
  BIT_R8(registers.L, 4);
}

void CPU::BIT_4_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  BIT_R8(byte, 4);
  mmu.Set(registers.HL, byte);
}

void CPU::BIT_4_A()
{
  BIT_R8(registers.A, 4);
}

void CPU::BIT_5_B()
{
  BIT_R8(registers.B, 5);
}

void CPU::BIT_5_C()
{
  BIT_R8(registers.C, 5);
}

void CPU::BIT_5_D()
{
  BIT_R8(registers.D, 5);
}

void CPU::BIT_5_E()
{
  BIT_R8(registers.E, 5);
}

void CPU::BIT_5_H()
{
  BIT_R8(registers.H, 5);
}

void CPU::BIT_5_L()
{
  BIT_R8(registers.L, 5);
}

void CPU::BIT_5_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  BIT_R8(byte, 5);
  mmu.Set(registers.HL, byte);
}

void CPU::BIT_5_A()
{
  BIT_R8(registers.A, 5);
}

void CPU::BIT_6_B()
{
  BIT_R8(registers.B, 6);
}

void CPU::BIT_6_C()
{
  BIT_R8(registers.C, 6);
}

void CPU::BIT_6_D()
{
  BIT_R8(registers.D, 6);
}

void CPU::BIT_6_E()
{
  BIT_R8(registers.E, 6);
}

void CPU::BIT_6_H()
{
  BIT_R8(registers.H, 6);
}

void CPU::BIT_6_L()
{
  BIT_R8(registers.L, 6);
}

void CPU::BIT_6_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  BIT_R8(byte, 6);
  mmu.Set(registers.HL, byte);
}

void CPU::BIT_6_A()
{
  BIT_R8(registers.A, 6);
}

void CPU::BIT_7_B()
{
  BIT_R8(registers.B, 7);
}

void CPU::BIT_7_C()
{
  BIT_R8(registers.C, 7);
}

void CPU::BIT_7_D()
{
  BIT_R8(registers.D, 7);
}

void CPU::BIT_7_E()
{
  BIT_R8(registers.E, 7);
}

void CPU::BIT_7_H()
{
  BIT_R8(registers.H, 7);
}

void CPU::BIT_7_L()
{
  BIT_R8(registers.L, 7);
}

void CPU::BIT_7_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  BIT_R8(byte, 7);
  mmu.Set(registers.HL, byte);
}

void CPU::BIT_7_A()
{
  BIT_R8(registers.A, 7);
}

void CPU::RES_0_B()
{
  RES_R8(registers.B, 0);
}

void CPU::RES_0_C()
{
  RES_R8(registers.C, 0);
}

void CPU::RES_0_D()
{
  RES_R8(registers.D, 0);
}

void CPU::RES_0_E()
{
  RES_R8(registers.E, 0);
}

void CPU::RES_0_H()
{
  RES_R8(registers.H, 0);
}

void CPU::RES_0_L()
{
  RES_R8(registers.H, 0);
}

void CPU::RES_0_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  RES_R8(byte, 0);
  mmu.Set(registers.HL, byte);
}

void CPU::RES_0_A()
{
  RES_R8(registers.A, 0);
}

void CPU::RES_1_B()
{
  RES_R8(registers.B, 1);
}

void CPU::RES_1_C()
{
  RES_R8(registers.C, 1);
}

void CPU::RES_1_D()
{
  RES_R8(registers.D, 1);
}

void CPU::RES_1_E()
{
  RES_R8(registers.E, 1);
}

void CPU::RES_1_H()
{
  RES_R8(registers.H, 1);
}

void CPU::RES_1_L()
{
  RES_R8(registers.L, 1);
}

void CPU::RES_1_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  RES_R8(byte, 1);
  mmu.Set(registers.HL, byte);
}

void CPU::RES_1_A()
{
  RES_R8(registers.A, 1);
}

void CPU::RES_2_B()
{
  RES_R8(registers.B, 2);
}

void CPU::RES_2_C()
{
  RES_R8(registers.C, 2);
}

void CPU::RES_2_D()
{
  RES_R8(registers.D, 2);
}

void CPU::RES_2_E()
{
  RES_R8(registers.E, 2);
}

void CPU::RES_2_H()
{
  RES_R8(registers.H, 2);
}

void CPU::RES_2_L()
{
  RES_R8(registers.L, 2);
}

void CPU::RES_2_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  RES_R8(byte, 2);
  mmu.Set(registers.HL, byte);
}

void CPU::RES_2_A()
{
  RES_R8(registers.A, 2);
}

void CPU::RES_3_B()
{
  RES_R8(registers.B, 3);
}

void CPU::RES_3_C()
{
  RES_R8(registers.C, 3);
}

void CPU::RES_3_D()
{
  RES_R8(registers.D, 3);
}

void CPU::RES_3_E()
{
  RES_R8(registers.E, 3);
}

void CPU::RES_3_H()
{
  RES_R8(registers.H, 3);
}

void CPU::RES_3_L()
{
  RES_R8(registers.L, 3);
}

void CPU::RES_3_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  RES_R8(byte, 3);
  mmu.Set(registers.HL, byte);
}

void CPU::RES_3_A()
{
  RES_R8(registers.A, 3);
}

void CPU::RES_4_B()
{
  RES_R8(registers.B, 4);
}

void CPU::RES_4_C()
{
  RES_R8(registers.C, 4);
}

void CPU::RES_4_D()
{
  RES_R8(registers.D, 4);
}

void CPU::RES_4_E()
{
  RES_R8(registers.E, 4);
}

void CPU::RES_4_H()
{
  RES_R8(registers.H, 4);
}

void CPU::RES_4_L()
{
  RES_R8(registers.L, 4);
}

void CPU::RES_4_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  RES_R8(byte, 4);
  mmu.Set(registers.HL, byte);
}

void CPU::RES_4_A()
{
  RES_R8(registers.A, 4);
}

void CPU::RES_5_B()
{
  RES_R8(registers.B, 5);
}

void CPU::RES_5_C()
{
  RES_R8(registers.C, 5);
}

void CPU::RES_5_D()
{
  RES_R8(registers.D, 5);
}

void CPU::RES_5_E()
{
  RES_R8(registers.E, 5);
}

void CPU::RES_5_H()
{
  RES_R8(registers.H, 5);
}

void CPU::RES_5_L()
{
  RES_R8(registers.L, 5);
}

void CPU::RES_5_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  RES_R8(byte, 5);
  mmu.Set(registers.HL, byte);
}

void CPU::RES_5_A()
{
  RES_R8(registers.A, 5);
}

void CPU::RES_6_B()
{
  RES_R8(registers.B, 6);
}

void CPU::RES_6_C()
{
  RES_R8(registers.C, 6);
}

void CPU::RES_6_D()
{
  RES_R8(registers.D, 6);
}

void CPU::RES_6_E()
{
  RES_R8(registers.E, 6);
}

void CPU::RES_6_H()
{
  RES_R8(registers.H, 6);
}

void CPU::RES_6_L()
{
  RES_R8(registers.L, 6);
}

void CPU::RES_6_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  RES_R8(byte, 6);
  mmu.Set(registers.HL, byte);
}

void CPU::RES_6_A()
{
  RES_R8(registers.A, 6);
}

void CPU::RES_7_B()
{
  RES_R8(registers.B, 7);
}

void CPU::RES_7_C()
{
  RES_R8(registers.C, 7);
}

void CPU::RES_7_D()
{
  RES_R8(registers.D, 7);
}

void CPU::RES_7_E()
{
  RES_R8(registers.E, 7);
}

void CPU::RES_7_H()
{
  RES_R8(registers.H, 7);
}

void CPU::RES_7_L()
{
  RES_R8(registers.L, 7);
}

void CPU::RES_7_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  RES_R8(byte, 7);
  mmu.Set(registers.HL, byte);
}

void CPU::RES_7_A()
{
  RES_R8(registers.A, 7);
}

void CPU::SET_0_B()
{
  SET_R8(registers.B, 0);
}

void CPU::SET_0_C()
{
  SET_R8(registers.C, 0);
}

void CPU::SET_0_D()
{
  SET_R8(registers.D, 0);
}

void CPU::SET_0_E()
{
  SET_R8(registers.E, 0);
}

void CPU::SET_0_H()
{
  SET_R8(registers.H, 0);
}

void CPU::SET_0_L()
{
  SET_R8(registers.L, 0);
}

void CPU::SET_0_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  SET_R8(byte, 0);
  mmu.Set(registers.HL, byte);
}

void CPU::SET_0_A()
{
  SET_R8(registers.A, 0);
}

void CPU::SET_1_B()
{
  SET_R8(registers.B, 1);
}

void CPU::SET_1_C()
{
  SET_R8(registers.C, 1);
}

void CPU::SET_1_D()
{
  SET_R8(registers.D, 1);
}

void CPU::SET_1_E()
{
  SET_R8(registers.E, 1);
}

void CPU::SET_1_H()
{
  SET_R8(registers.H, 1);
}

void CPU::SET_1_L()
{
  SET_R8(registers.L, 1);
}

void CPU::SET_1_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  SET_R8(byte, 1);
  mmu.Set(registers.HL, byte);
}

void CPU::SET_1_A()
{
  SET_R8(registers.A, 1);
}

void CPU::SET_2_B()
{
  SET_R8(registers.B, 2);
}

void CPU::SET_2_C()
{
  SET_R8(registers.C, 2);
}

void CPU::SET_2_D()
{
  SET_R8(registers.D, 2);
}

void CPU::SET_2_E()
{
  SET_R8(registers.E, 2);
}

void CPU::SET_2_H()
{
  SET_R8(registers.H, 2);
}

void CPU::SET_2_L()
{
  SET_R8(registers.L, 2);
}

void CPU::SET_2_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  SET_R8(byte, 2);
  mmu.Set(registers.HL, byte);
}

void CPU::SET_2_A()
{
  SET_R8(registers.A, 2);
}

void CPU::SET_3_B()
{
  SET_R8(registers.B, 3);
}

void CPU::SET_3_C()
{
  SET_R8(registers.C, 3);
}

void CPU::SET_3_D()
{
  SET_R8(registers.D, 3);
}

void CPU::SET_3_E()
{
  SET_R8(registers.E, 3);
}

void CPU::SET_3_H()
{
  SET_R8(registers.H, 3);
}

void CPU::SET_3_L()
{
  SET_R8(registers.L, 3);
}

void CPU::SET_3_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  SET_R8(byte, 3);
  mmu.Set(registers.HL, byte);
}

void CPU::SET_3_A()
{
  SET_R8(registers.A, 3);
}

void CPU::SET_4_B()
{
  SET_R8(registers.B, 4);
}

void CPU::SET_4_C()
{
  SET_R8(registers.C, 4);
}

void CPU::SET_4_D()
{
  SET_R8(registers.D, 4);
}

void CPU::SET_4_E()
{
  SET_R8(registers.E, 4);
}

void CPU::SET_4_H()
{
  SET_R8(registers.H, 4);
}

void CPU::SET_4_L()
{
  SET_R8(registers.L, 4);
}

void CPU::SET_4_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  SET_R8(byte, 4);
  mmu.Set(registers.HL, byte);
}

void CPU::SET_4_A()
{
  SET_R8(registers.A, 4);
}

void CPU::SET_5_B()
{
  SET_R8(registers.B, 5);
}

void CPU::SET_5_C()
{
  SET_R8(registers.C, 5);
}

void CPU::SET_5_D()
{
  SET_R8(registers.D, 5);
}

void CPU::SET_5_E()
{
  SET_R8(registers.E, 5);
}

void CPU::SET_5_H()
{
  SET_R8(registers.H, 5);
}

void CPU::SET_5_L()
{
  SET_R8(registers.L, 5);
}

void CPU::SET_5_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  SET_R8(byte, 5);
  mmu.Set(registers.HL, byte);
}

void CPU::SET_5_A()
{
  SET_R8(registers.A, 5);
}

void CPU::SET_6_B()
{
  SET_R8(registers.B, 6);
}

void CPU::SET_6_C()
{
  SET_R8(registers.C, 6);
}

void CPU::SET_6_D()
{
  SET_R8(registers.D, 6);
}

void CPU::SET_6_E()
{
  SET_R8(registers.E, 6);
}

void CPU::SET_6_H()
{
  SET_R8(registers.H, 6);
}

void CPU::SET_6_L()
{
  SET_R8(registers.L, 6);
}

void CPU::SET_6_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  SET_R8(byte, 6);
  mmu.Set(registers.HL, byte);
}

void CPU::SET_6_A()
{
  SET_R8(registers.A, 6);
}

void CPU::SET_7_B()
{
  SET_R8(registers.B, 7);
}

void CPU::SET_7_C()
{
  SET_R8(registers.C, 7);
}

void CPU::SET_7_D()
{
  SET_R8(registers.D, 7);
}

void CPU::SET_7_E()
{
  SET_R8(registers.E, 7);
}

void CPU::SET_7_H()
{
  SET_R8(registers.H, 7);
}

void CPU::SET_7_L()
{
  SET_R8(registers.L, 7);
}

void CPU::SET_7_dHL()
{
  std::uint8_t byte = mmu.Get(registers.HL);
  SET_R8(byte, 7);
  mmu.Set(registers.HL, byte);
}

void CPU::SET_7_A()
{
  SET_R8(registers.A, 7);
}

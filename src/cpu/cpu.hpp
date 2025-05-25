#pragma once

#include <cstdint>
#include <array>

class MMU;

class CPU
{
  MMU& mmu;

  struct Registers
  {
    struct
    {
      union
      {
        struct
        {
          std::uint8_t A;
          std::uint8_t F;
        };
        std::uint16_t AF;
      };
      union
      {
        struct
        {
          std::uint8_t B;
          std::uint8_t C;
        };
        std::uint16_t BC;
      };
      union
      {
        struct
        {
          std::uint8_t D;
          std::uint8_t E;
        };
        std::uint16_t DE;
      };
      union
      {
        struct
        {
          std::uint8_t H;
          std::uint8_t L;
        };
        std::uint16_t HL;
      };
    };
    std::uint16_t SP;
    std::uint16_t PC;
    bool IME;

    void SetFlag(int bit, bool value);
    bool GetFlag(int bit);

    static constexpr int BITPOS_ZERO_FLAG = 7;
    static constexpr int BITPOS_SUBTRACTION_FLAG = 6;
    static constexpr int BITPOS_HALF_CARRY_FLAG = 5;
    static constexpr int BITPOS_CARRY_FLAG = 4;

    void SetZeroFlag(bool value);
    void SetSubtractionFlag(bool value);
    void SetHalfCarryFlag(bool value);
    void SetCarryFlag(bool value);

    bool GetZeroFlag();
    bool GetSubtractionFlag();
    bool GetHalfCarryFlag();
    bool GetCarryFlag();

  } registers;

  bool halted = false;

  void ExecuteOpcode(std::uint8_t opcode);
  void Halt();

  // operations
  void INC_R8(std::uint8_t& reg);
  void DEC_R8(std::uint8_t& reg);

  void ADD_A_R8(const std::uint8_t& reg);
  void ADD_HL_R16(const std::uint16_t& reg);

  void ADC_A_R8(const std::uint8_t& reg);

  void SUB_A_R8(const std::uint8_t& reg);
  void SBC_A_R8(const std::uint8_t& reg);

  void AND_A_R8(const std::uint8_t& reg);
  void XOR_A_R8(const std::uint8_t& reg);
  void OR_A_R8(const std::uint8_t& reg);
  void CP_A_R8(const std::uint8_t& reg);

  void RST_VEC(const std::uint16_t);

  void RL_R8(std::uint8_t& reg);
  void RR_R8(std::uint8_t& reg);

  void SLA_R8(std::uint8_t& reg);
  void SRA_R8(std::uint8_t& reg);
  void SRL_R8(std::uint8_t& reg);

  void RLC_R8(std::uint8_t& reg);
  void RRC_R8(std::uint8_t& reg);

  void SWAP_R8(std::uint8_t& reg);
  void BIT_R8(std::uint8_t& reg, int bitNumber);
  void RES_R8(std::uint8_t& reg, int bitNumber);
  void SET_R8(std::uint8_t& reg, int bitNumber);

  void PUSH_N16(const std::uint16_t& value);
  std::uint16_t POP_N16();

  std::uint8_t GetN8();
  std::uint16_t GetN16();

  std::int8_t GetE8();

  void JR_CC_E8(bool condition);

  bool IsHalfCarryOverflow8(const std::uint8_t a, const std::uint8_t b);
  bool IsCarryOverflow8(const std::uint8_t a, const std::uint8_t b);

  bool IsHalfCarryOverflow16(const std::uint16_t a, const std::uint16_t b);
  bool IsCarryOverflow16(const std::uint16_t a, const std::uint16_t b);

  bool IsHalfCarryUnderflow8(const std::uint8_t a, const std::uint8_t b);
  bool IsCarryUnderflow8(const std::uint8_t a, const std::uint8_t b);

  // opcodes
  void NOP();
  void LD_BC_N16();
  void LD_dBC_A();
  void INC_BC();
  void INC_B();
  void DEC_B();
  void LD_B_N8();
  void RLCA();
  void LD_DN16_SP();
  void ADD_HL_BC();
  void LD_A_DBC();
  void DEC_BC();
  void INC_C();
  void DEC_C();
  void LD_C_N8();
  void RRCA();

  void STOP_N8();
  void LD_DE_N16();
  void LD_dDE_A();
  void INC_DE();
  void INC_D();
  void DEC_D();
  void LD_D_N8();
  void RLA();
  void JR_E8();
  void ADD_HL_DE();
  void LD_A_dDE();
  void DEC_DE();
  void INC_E();
  void DEC_E();
  void LD_E_N8();
  void RRA();

  void JR_NZ_E8();
  void LD_HL_N16();
  void LD_dHLi_A();
  void INC_HL();
  void INC_H();
  void DEC_H();
  void LD_H_N8();
  void DAA();
  void JR_Z_E8();
  void ADD_HL_HL();
  void LD_A_dHLi();
  void DEC_HL();
  void INC_L();
  void DEC_L();
  void LD_L_N8();
  void CPL();

  void JR_NC_E8();
  void LD_SP_N16();
  void LD_dHLd_A();
  void INC_SP();
  void INC_dHL();
  void DEC_dHL();
  void LD_dHL_N8();
  void SCF();
  void JR_C_E8();
  void ADD_HL_SP();
  void LD_A_dHLd();
  void DEC_SP();
  void INC_A();
  void DEC_A();
  void LD_A_N8();
  void CCF();

  void LD_B_B();
  void LD_B_C();
  void LD_B_D();
  void LD_B_E();
  void LD_B_H();
  void LD_B_L();
  void LD_B_dHL();
  void LD_B_A();
  void LD_C_B();
  void LD_C_C();
  void LD_C_D();
  void LD_C_E();
  void LD_C_H();
  void LD_C_L();
  void LD_C_dHL();
  void LD_C_A();

  void LD_D_B();
  void LD_D_C();
  void LD_D_D();
  void LD_D_E();
  void LD_D_H();
  void LD_D_L();
  void LD_D_dHL();
  void LD_D_A();
  void LD_E_B();
  void LD_E_C();
  void LD_E_D();
  void LD_E_E();
  void LD_E_H();
  void LD_E_L();
  void LD_E_dHL();
  void LD_E_A();

  void LD_H_B();
  void LD_H_C();
  void LD_H_D();
  void LD_H_E();
  void LD_H_H();
  void LD_H_L();
  void LD_H_dHL();
  void LD_H_A();
  void LD_L_B();
  void LD_L_C();
  void LD_L_D();
  void LD_L_E();
  void LD_L_H();
  void LD_L_L();
  void LD_L_dHL();
  void LD_L_A();

  void LD_dHL_B();
  void LD_dHL_C();
  void LD_dHL_D();
  void LD_dHL_E();
  void LD_dHL_H();
  void LD_dHL_L();
  void HALT();
  void LD_dHL_A();
  void LD_A_B();
  void LD_A_C();
  void LD_A_D();
  void LD_A_E();
  void LD_A_H();
  void LD_A_L();
  void LD_A_dHL();
  void LD_A_A();

  void ADD_A_B();
  void ADD_A_C();
  void ADD_A_D();
  void ADD_A_E();
  void ADD_A_H();
  void ADD_A_L();
  void ADD_A_dHL();
  void ADD_A_A();
  void ADC_A_B();
  void ADC_A_C();
  void ADC_A_D();
  void ADC_A_E();
  void ADC_A_H();
  void ADC_A_L();
  void ADC_A_dHL();
  void ADC_A_A();

  void SUB_A_B();
  void SUB_A_C();
  void SUB_A_D();
  void SUB_A_E();
  void SUB_A_H();
  void SUB_A_L();
  void SUB_A_dHL();
  void SUB_A_A();
  void SBC_A_B();
  void SBC_A_C();
  void SBC_A_D();
  void SBC_A_E();
  void SBC_A_H();
  void SBC_A_L();
  void SBC_A_dHL();
  void SBC_A_A();

  void AND_A_B();
  void AND_A_C();
  void AND_A_D();
  void AND_A_E();
  void AND_A_H();
  void AND_A_L();
  void AND_A_dHL();
  void AND_A_A();
  void XOR_A_B();
  void XOR_A_C();
  void XOR_A_D();
  void XOR_A_E();
  void XOR_A_H();
  void XOR_A_L();
  void XOR_A_dHL();
  void XOR_A_A();

  void OR_A_B();
  void OR_A_C();
  void OR_A_D();
  void OR_A_E();
  void OR_A_H();
  void OR_A_L();
  void OR_A_dHL();
  void OR_A_A();
  void CP_A_B();
  void CP_A_C();
  void CP_A_D();
  void CP_A_E();
  void CP_A_H();
  void CP_A_L();
  void CP_A_dHL();
  void CP_A_A();

  void RET_NZ();
  void POP_BC();
  void JP_NZ_A16();
  void JP_A16();
  void CALL_NZ_A16();
  void PUSH_BC();
  void ADD_A_N8();
  void RST_00();
  void RET_Z();
  void RET();
  void JP_Z_A16();
  void CALL_Z_A16();
  void CALL_A16();
  void ADC_A_N8();
  void RST_08();
  void RET_NC();

  void POP_DE();
  void JP_NC_A16();
  void CALL_NC_A16();
  void PUSH_DE();
  void SUB_A_N8();
  void RST_10();
  void RET_C();
  void RETI();
  void JP_C_A16();
  void CALL_C_A16();
  void SBC_A_N8();
  void RST_18();

  void LDH_dA8_A();
  void POP_HL();
  void LDH_dC_A();
  void PUSH_HL();
  void AND_A_N8();
  void RST_20();
  void ADD_SP_E8();
  void JP_HL();
  void LD_dA16_A();
  void XOR_A_N8();
  void RST_28();

  void LDH_A_dA8();
  void POP_AF();
  void LDH_A_dC();
  void DI();
  void PUSH_AF();
  void OR_A_N8();
  void RST_30();
  void LD_HL_SP_p_E8();
  void LD_SP_HL();
  void LD_A_dA16();
  void EI();
  void CP_A_N8();
  void RST_38();

  // Prefixed opcodes
  void RLC_B();
  void RLC_C();
  void RLC_D();
  void RLC_E();
  void RLC_H();
  void RLC_L();
  void RLC_dHL();
  void RLC_A();
  void RRC_B();
  void RRC_C();
  void RRC_D();
  void RRC_E();
  void RRC_H();
  void RRC_L();
  void RRC_dHL();
  void RRC_A();
  void RL_B();
  void RL_C();
  void RL_D();
  void RL_E();
  void RL_H();
  void RL_L();
  void RL_dHL();
  void RL_A();
  void RR_B();
  void RR_C();
  void RR_D();
  void RR_E();
  void RR_H();
  void RR_L();
  void RR_dHL();
  void RR_A();
  void SLA_B();
  void SLA_C();
  void SLA_D();
  void SLA_E();
  void SLA_H();
  void SLA_L();
  void SLA_dHL();
  void SLA_A();
  void SRA_B();
  void SRA_C();
  void SRA_D();
  void SRA_E();
  void SRA_H();
  void SRA_L();
  void SRA_dHL();
  void SRA_A();
  void SWAP_B();
  void SWAP_C();
  void SWAP_D();
  void SWAP_E();
  void SWAP_H();
  void SWAP_L();
  void SWAP_dHL();
  void SWAP_A();
  void SRL_B();
  void SRL_C();
  void SRL_D();
  void SRL_E();
  void SRL_H();
  void SRL_L();
  void SRL_dHL();
  void SRL_A();
  void BIT_0_B();
  void BIT_0_C();
  void BIT_0_D();
  void BIT_0_E();
  void BIT_0_H();
  void BIT_0_L();
  void BIT_0_dHL();
  void BIT_0_A();
  void BIT_1_B();
  void BIT_1_C();
  void BIT_1_D();
  void BIT_1_E();
  void BIT_1_H();
  void BIT_1_L();
  void BIT_1_dHL();
  void BIT_1_A();
  void BIT_2_B();
  void BIT_2_C();
  void BIT_2_D();
  void BIT_2_E();
  void BIT_2_H();
  void BIT_2_L();
  void BIT_2_dHL();
  void BIT_2_A();
  void BIT_3_B();
  void BIT_3_C();
  void BIT_3_D();
  void BIT_3_E();
  void BIT_3_H();
  void BIT_3_L();
  void BIT_3_dHL();
  void BIT_3_A();
  void BIT_4_B();
  void BIT_4_C();
  void BIT_4_D();
  void BIT_4_E();
  void BIT_4_H();
  void BIT_4_L();
  void BIT_4_dHL();
  void BIT_4_A();
  void BIT_5_B();
  void BIT_5_C();
  void BIT_5_D();
  void BIT_5_E();
  void BIT_5_H();
  void BIT_5_L();
  void BIT_5_dHL();
  void BIT_5_A();
  void BIT_6_B();
  void BIT_6_C();
  void BIT_6_D();
  void BIT_6_E();
  void BIT_6_H();
  void BIT_6_L();
  void BIT_6_dHL();
  void BIT_6_A();
  void BIT_7_B();
  void BIT_7_C();
  void BIT_7_D();
  void BIT_7_E();
  void BIT_7_H();
  void BIT_7_L();
  void BIT_7_dHL();
  void BIT_7_A();
  void RES_0_B();
  void RES_0_C();
  void RES_0_D();
  void RES_0_E();
  void RES_0_H();
  void RES_0_L();
  void RES_0_dHL();
  void RES_0_A();
  void RES_1_B();
  void RES_1_C();
  void RES_1_D();
  void RES_1_E();
  void RES_1_H();
  void RES_1_L();
  void RES_1_dHL();
  void RES_1_A();
  void RES_2_B();
  void RES_2_C();
  void RES_2_D();
  void RES_2_E();
  void RES_2_H();
  void RES_2_L();
  void RES_2_dHL();
  void RES_2_A();
  void RES_3_B();
  void RES_3_C();
  void RES_3_D();
  void RES_3_E();
  void RES_3_H();
  void RES_3_L();
  void RES_3_dHL();
  void RES_3_A();
  void RES_4_B();
  void RES_4_C();
  void RES_4_D();
  void RES_4_E();
  void RES_4_H();
  void RES_4_L();
  void RES_4_dHL();
  void RES_4_A();
  void RES_5_B();
  void RES_5_C();
  void RES_5_D();
  void RES_5_E();
  void RES_5_H();
  void RES_5_L();
  void RES_5_dHL();
  void RES_5_A();
  void RES_6_B();
  void RES_6_C();
  void RES_6_D();
  void RES_6_E();
  void RES_6_H();
  void RES_6_L();
  void RES_6_dHL();
  void RES_6_A();
  void RES_7_B();
  void RES_7_C();
  void RES_7_D();
  void RES_7_E();
  void RES_7_H();
  void RES_7_L();
  void RES_7_dHL();
  void RES_7_A();
  void SET_0_B();
  void SET_0_C();
  void SET_0_D();
  void SET_0_E();
  void SET_0_H();
  void SET_0_L();
  void SET_0_dHL();
  void SET_0_A();
  void SET_1_B();
  void SET_1_C();
  void SET_1_D();
  void SET_1_E();
  void SET_1_H();
  void SET_1_L();
  void SET_1_dHL();
  void SET_1_A();
  void SET_2_B();
  void SET_2_C();
  void SET_2_D();
  void SET_2_E();
  void SET_2_H();
  void SET_2_L();
  void SET_2_dHL();
  void SET_2_A();
  void SET_3_B();
  void SET_3_C();
  void SET_3_D();
  void SET_3_E();
  void SET_3_H();
  void SET_3_L();
  void SET_3_dHL();
  void SET_3_A();
  void SET_4_B();
  void SET_4_C();
  void SET_4_D();
  void SET_4_E();
  void SET_4_H();
  void SET_4_L();
  void SET_4_dHL();
  void SET_4_A();
  void SET_5_B();
  void SET_5_C();
  void SET_5_D();
  void SET_5_E();
  void SET_5_H();
  void SET_5_L();
  void SET_5_dHL();
  void SET_5_A();
  void SET_6_B();
  void SET_6_C();
  void SET_6_D();
  void SET_6_E();
  void SET_6_H();
  void SET_6_L();
  void SET_6_dHL();
  void SET_6_A();
  void SET_7_B();
  void SET_7_C();
  void SET_7_D();
  void SET_7_E();
  void SET_7_H();
  void SET_7_L();
  void SET_7_dHL();
  void SET_7_A();

  using OpcodeFunction = void (CPU::*)();
  static constexpr std::uint8_t ExtendedOpcodePrefix = 0xCB;

  std::array<OpcodeFunction, 256> opcodeTable = {
      &CPU::NOP,        &CPU::LD_BC_N16, &CPU::LD_dBC_A,      &CPU::INC_BC,    &CPU::INC_B,       &CPU::DEC_B,
      &CPU::LD_B_N8,    &CPU::RLCA,      &CPU::LD_DN16_SP,    &CPU::ADD_HL_BC, &CPU::LD_A_DBC,    &CPU::DEC_BC,
      &CPU::INC_C,      &CPU::DEC_C,     &CPU::LD_C_N8,       &CPU::RRCA,      &CPU::STOP_N8,     &CPU::LD_DE_N16,
      &CPU::LD_dDE_A,   &CPU::INC_DE,    &CPU::INC_D,         &CPU::DEC_D,     &CPU::LD_D_N8,     &CPU::RLA,
      &CPU::JR_E8,      &CPU::ADD_HL_DE, &CPU::LD_A_dDE,      &CPU::DEC_DE,    &CPU::INC_E,       &CPU::DEC_E,
      &CPU::LD_E_N8,    &CPU::RRA,       &CPU::JR_NZ_E8,      &CPU::LD_HL_N16, &CPU::LD_dHLi_A,   &CPU::INC_HL,
      &CPU::INC_H,      &CPU::DEC_H,     &CPU::LD_H_N8,       &CPU::DAA,       &CPU::JR_Z_E8,     &CPU::ADD_HL_HL,
      &CPU::LD_A_dHLi,  &CPU::DEC_HL,    &CPU::INC_L,         &CPU::DEC_L,     &CPU::LD_L_N8,     &CPU::CPL,
      &CPU::JR_NC_E8,   &CPU::LD_SP_N16, &CPU::LD_dHLd_A,     &CPU::INC_SP,    &CPU::INC_dHL,     &CPU::DEC_dHL,
      &CPU::LD_dHL_N8,  &CPU::SCF,       &CPU::JR_C_E8,       &CPU::ADD_HL_SP, &CPU::LD_A_dHLd,   &CPU::DEC_SP,
      &CPU::INC_A,      &CPU::DEC_A,     &CPU::LD_A_N8,       &CPU::CCF,       &CPU::LD_B_B,      &CPU::LD_B_C,
      &CPU::LD_B_D,     &CPU::LD_B_E,    &CPU::LD_B_H,        &CPU::LD_B_L,    &CPU::LD_B_dHL,    &CPU::LD_B_A,
      &CPU::LD_C_B,     &CPU::LD_C_C,    &CPU::LD_C_D,        &CPU::LD_C_E,    &CPU::LD_C_H,      &CPU::LD_C_L,
      &CPU::LD_C_dHL,   &CPU::LD_C_A,    &CPU::LD_D_B,        &CPU::LD_D_C,    &CPU::LD_D_D,      &CPU::LD_D_E,
      &CPU::LD_D_H,     &CPU::LD_D_L,    &CPU::LD_D_dHL,      &CPU::LD_D_A,    &CPU::LD_E_B,      &CPU::LD_E_C,
      &CPU::LD_E_D,     &CPU::LD_E_E,    &CPU::LD_E_H,        &CPU::LD_E_L,    &CPU::LD_E_dHL,    &CPU::LD_E_A,
      &CPU::LD_H_B,     &CPU::LD_H_C,    &CPU::LD_H_D,        &CPU::LD_H_E,    &CPU::LD_H_H,      &CPU::LD_H_L,
      &CPU::LD_H_dHL,   &CPU::LD_H_A,    &CPU::LD_L_B,        &CPU::LD_L_C,    &CPU::LD_L_D,      &CPU::LD_L_E,
      &CPU::LD_L_H,     &CPU::LD_L_L,    &CPU::LD_L_dHL,      &CPU::LD_L_A,    &CPU::LD_dHL_B,    &CPU::LD_dHL_C,
      &CPU::LD_dHL_D,   &CPU::LD_dHL_E,  &CPU::LD_dHL_H,      &CPU::LD_dHL_L,  &CPU::HALT,        &CPU::LD_dHL_A,
      &CPU::LD_A_B,     &CPU::LD_A_C,    &CPU::LD_A_D,        &CPU::LD_A_E,    &CPU::LD_A_H,      &CPU::LD_A_L,
      &CPU::LD_A_dHL,   &CPU::LD_A_A,    &CPU::ADD_A_B,       &CPU::ADD_A_C,   &CPU::ADD_A_D,     &CPU::ADD_A_E,
      &CPU::ADD_A_H,    &CPU::ADD_A_L,   &CPU::ADD_A_dHL,     &CPU::ADD_A_A,   &CPU::ADC_A_B,     &CPU::ADC_A_C,
      &CPU::ADC_A_D,    &CPU::ADC_A_E,   &CPU::ADC_A_H,       &CPU::ADC_A_L,   &CPU::ADC_A_dHL,   &CPU::ADC_A_A,
      &CPU::SUB_A_B,    &CPU::SUB_A_C,   &CPU::SUB_A_D,       &CPU::SUB_A_E,   &CPU::SUB_A_H,     &CPU::SUB_A_L,
      &CPU::SUB_A_dHL,  &CPU::SUB_A_A,   &CPU::SBC_A_B,       &CPU::SBC_A_C,   &CPU::SBC_A_D,     &CPU::SBC_A_E,
      &CPU::SBC_A_H,    &CPU::SBC_A_L,   &CPU::SBC_A_dHL,     &CPU::SBC_A_A,   &CPU::AND_A_B,     &CPU::AND_A_C,
      &CPU::AND_A_D,    &CPU::AND_A_E,   &CPU::AND_A_H,       &CPU::AND_A_L,   &CPU::AND_A_dHL,   &CPU::AND_A_A,
      &CPU::XOR_A_B,    &CPU::XOR_A_C,   &CPU::XOR_A_D,       &CPU::XOR_A_E,   &CPU::XOR_A_H,     &CPU::XOR_A_L,
      &CPU::XOR_A_dHL,  &CPU::XOR_A_A,   &CPU::OR_A_B,        &CPU::OR_A_C,    &CPU::OR_A_D,      &CPU::OR_A_E,
      &CPU::OR_A_H,     &CPU::OR_A_L,    &CPU::OR_A_dHL,      &CPU::OR_A_A,    &CPU::CP_A_B,      &CPU::CP_A_C,
      &CPU::CP_A_D,     &CPU::CP_A_E,    &CPU::CP_A_H,        &CPU::CP_A_L,    &CPU::CP_A_dHL,    &CPU::CP_A_A,
      &CPU::RET_NZ,     &CPU::POP_BC,    &CPU::JP_NZ_A16,     &CPU::JP_A16,    &CPU::CALL_NZ_A16, &CPU::PUSH_BC,
      &CPU::ADD_A_N8,   &CPU::RST_00,    &CPU::RET_Z,         &CPU::RET,       &CPU::JP_Z_A16,    &CPU::NOP,
      &CPU::CALL_Z_A16, &CPU::CALL_A16,  &CPU::ADC_A_N8,      &CPU::RST_08,    &CPU::RET_NC,      &CPU::POP_DE,
      &CPU::JP_NC_A16,  &CPU::NOP,       &CPU::CALL_NC_A16,   &CPU::PUSH_DE,   &CPU::SUB_A_N8,    &CPU::RST_10,
      &CPU::RET_C,      &CPU::RETI,      &CPU::JP_C_A16,      &CPU::NOP,       &CPU::CALL_C_A16,  &CPU::NOP,
      &CPU::SBC_A_N8,   &CPU::RST_18,    &CPU::LDH_dA8_A,     &CPU::POP_HL,    &CPU::LDH_dC_A,    &CPU::NOP,
      &CPU::NOP,        &CPU::PUSH_HL,   &CPU::AND_A_N8,      &CPU::RST_20,    &CPU::ADD_SP_E8,   &CPU::JP_HL,
      &CPU::LD_dA16_A,  &CPU::NOP,       &CPU::NOP,           &CPU::NOP,       &CPU::XOR_A_N8,    &CPU::RST_28,
      &CPU::LDH_A_dA8,  &CPU::POP_AF,    &CPU::LDH_A_dC,      &CPU::DI,        &CPU::NOP,         &CPU::PUSH_AF,
      &CPU::OR_A_N8,    &CPU::RST_30,    &CPU::LD_HL_SP_p_E8, &CPU::LD_SP_HL,  &CPU::LD_A_dA16,   &CPU::EI,
      &CPU::NOP,        &CPU::NOP,       &CPU::CP_A_N8,       &CPU::RST_38};

  std::array<OpcodeFunction, 256> extendedOpcodeTable = {
      &CPU::RLC_B,     &CPU::RLC_C,   &CPU::RLC_D,     &CPU::RLC_E,   &CPU::RLC_H,     &CPU::RLC_L,
      &CPU::RLC_dHL,   &CPU::RLC_A,   &CPU::RRC_B,     &CPU::RRC_C,   &CPU::RRC_D,     &CPU::RRC_E,
      &CPU::RRC_H,     &CPU::RRC_L,   &CPU::RRC_dHL,   &CPU::RRC_A,   &CPU::RL_B,      &CPU::RL_C,
      &CPU::RL_D,      &CPU::RL_E,    &CPU::RL_H,      &CPU::RL_L,    &CPU::RL_dHL,    &CPU::RL_A,
      &CPU::RR_B,      &CPU::RR_C,    &CPU::RR_D,      &CPU::RR_E,    &CPU::RR_H,      &CPU::RR_L,
      &CPU::RR_dHL,    &CPU::RR_A,    &CPU::SLA_B,     &CPU::SLA_C,   &CPU::SLA_D,     &CPU::SLA_E,
      &CPU::SLA_H,     &CPU::SLA_L,   &CPU::SLA_dHL,   &CPU::SLA_A,   &CPU::SRA_B,     &CPU::SRA_C,
      &CPU::SRA_D,     &CPU::SRA_E,   &CPU::SRA_H,     &CPU::SRA_L,   &CPU::SRA_dHL,   &CPU::SRA_A,
      &CPU::SWAP_B,    &CPU::SWAP_C,  &CPU::SWAP_D,    &CPU::SWAP_E,  &CPU::SWAP_H,    &CPU::SWAP_L,
      &CPU::SWAP_dHL,  &CPU::SWAP_A,  &CPU::SRL_B,     &CPU::SRL_C,   &CPU::SRL_D,     &CPU::SRL_E,
      &CPU::SRL_H,     &CPU::SRL_L,   &CPU::SRL_dHL,   &CPU::SRL_A,   &CPU::BIT_0_B,   &CPU::BIT_0_C,
      &CPU::BIT_0_D,   &CPU::BIT_0_E, &CPU::BIT_0_H,   &CPU::BIT_0_L, &CPU::BIT_0_dHL, &CPU::BIT_0_A,
      &CPU::BIT_1_B,   &CPU::BIT_1_C, &CPU::BIT_1_D,   &CPU::BIT_1_E, &CPU::BIT_1_H,   &CPU::BIT_1_L,
      &CPU::BIT_1_dHL, &CPU::BIT_1_A, &CPU::BIT_2_B,   &CPU::BIT_2_C, &CPU::BIT_2_D,   &CPU::BIT_2_E,
      &CPU::BIT_2_H,   &CPU::BIT_2_L, &CPU::BIT_2_dHL, &CPU::BIT_2_A, &CPU::BIT_3_B,   &CPU::BIT_3_C,
      &CPU::BIT_3_D,   &CPU::BIT_3_E, &CPU::BIT_3_H,   &CPU::BIT_3_L, &CPU::BIT_3_dHL, &CPU::BIT_3_A,
      &CPU::BIT_4_B,   &CPU::BIT_4_C, &CPU::BIT_4_D,   &CPU::BIT_4_E, &CPU::BIT_4_H,   &CPU::BIT_4_L,
      &CPU::BIT_4_dHL, &CPU::BIT_4_A, &CPU::BIT_5_B,   &CPU::BIT_5_C, &CPU::BIT_5_D,   &CPU::BIT_5_E,
      &CPU::BIT_5_H,   &CPU::BIT_5_L, &CPU::BIT_5_dHL, &CPU::BIT_5_A, &CPU::BIT_6_B,   &CPU::BIT_6_C,
      &CPU::BIT_6_D,   &CPU::BIT_6_E, &CPU::BIT_6_H,   &CPU::BIT_6_L, &CPU::BIT_6_dHL, &CPU::BIT_6_A,
      &CPU::BIT_7_B,   &CPU::BIT_7_C, &CPU::BIT_7_D,   &CPU::BIT_7_E, &CPU::BIT_7_H,   &CPU::BIT_7_L,
      &CPU::BIT_7_dHL, &CPU::BIT_7_A, &CPU::RES_0_B,   &CPU::RES_0_C, &CPU::RES_0_D,   &CPU::RES_0_E,
      &CPU::RES_0_H,   &CPU::RES_0_L, &CPU::RES_0_dHL, &CPU::RES_0_A, &CPU::RES_1_B,   &CPU::RES_1_C,
      &CPU::RES_1_D,   &CPU::RES_1_E, &CPU::RES_1_H,   &CPU::RES_1_L, &CPU::RES_1_dHL, &CPU::RES_1_A,
      &CPU::RES_2_B,   &CPU::RES_2_C, &CPU::RES_2_D,   &CPU::RES_2_E, &CPU::RES_2_H,   &CPU::RES_2_L,
      &CPU::RES_2_dHL, &CPU::RES_2_A, &CPU::RES_3_B,   &CPU::RES_3_C, &CPU::RES_3_D,   &CPU::RES_3_E,
      &CPU::RES_3_H,   &CPU::RES_3_L, &CPU::RES_3_dHL, &CPU::RES_3_A, &CPU::RES_4_B,   &CPU::RES_4_C,
      &CPU::RES_4_D,   &CPU::RES_4_E, &CPU::RES_4_H,   &CPU::RES_4_L, &CPU::RES_4_dHL, &CPU::RES_4_A,
      &CPU::RES_5_B,   &CPU::RES_5_C, &CPU::RES_5_D,   &CPU::RES_5_E, &CPU::RES_5_H,   &CPU::RES_5_L,
      &CPU::RES_5_dHL, &CPU::RES_5_A, &CPU::RES_6_B,   &CPU::RES_6_C, &CPU::RES_6_D,   &CPU::RES_6_E,
      &CPU::RES_6_H,   &CPU::RES_6_L, &CPU::RES_6_dHL, &CPU::RES_6_A, &CPU::RES_7_B,   &CPU::RES_7_C,
      &CPU::RES_7_D,   &CPU::RES_7_E, &CPU::RES_7_H,   &CPU::RES_7_L, &CPU::RES_7_dHL, &CPU::RES_7_A,
      &CPU::SET_0_B,   &CPU::SET_0_C, &CPU::SET_0_D,   &CPU::SET_0_E, &CPU::SET_0_H,   &CPU::SET_0_L,
      &CPU::SET_0_dHL, &CPU::SET_0_A, &CPU::SET_1_B,   &CPU::SET_1_C, &CPU::SET_1_D,   &CPU::SET_1_E,
      &CPU::SET_1_H,   &CPU::SET_1_L, &CPU::SET_1_dHL, &CPU::SET_1_A, &CPU::SET_2_B,   &CPU::SET_2_C,
      &CPU::SET_2_D,   &CPU::SET_2_E, &CPU::SET_2_H,   &CPU::SET_2_L, &CPU::SET_2_dHL, &CPU::SET_2_A,
      &CPU::SET_3_B,   &CPU::SET_3_C, &CPU::SET_3_D,   &CPU::SET_3_E, &CPU::SET_3_H,   &CPU::SET_3_L,
      &CPU::SET_3_dHL, &CPU::SET_3_A, &CPU::SET_4_B,   &CPU::SET_4_C, &CPU::SET_4_D,   &CPU::SET_4_E,
      &CPU::SET_4_H,   &CPU::SET_4_L, &CPU::SET_4_dHL, &CPU::SET_4_A, &CPU::SET_5_B,   &CPU::SET_5_C,
      &CPU::SET_5_D,   &CPU::SET_5_E, &CPU::SET_5_H,   &CPU::SET_5_L, &CPU::SET_5_dHL, &CPU::SET_5_A,
      &CPU::SET_6_B,   &CPU::SET_6_C, &CPU::SET_6_D,   &CPU::SET_6_E, &CPU::SET_6_H,   &CPU::SET_6_L,
      &CPU::SET_6_dHL, &CPU::SET_6_A, &CPU::SET_7_B,   &CPU::SET_7_C, &CPU::SET_7_D,   &CPU::SET_7_E,
      &CPU::SET_7_H,   &CPU::SET_7_L, &CPU::SET_7_dHL, &CPU::SET_7_A};

public:
  CPU(MMU& mmu) : mmu(mmu) {}
  void Tick();

  bool IsHalted() { return halted; }
};
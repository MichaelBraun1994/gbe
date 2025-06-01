#pragma once

#include <cstdint>
#include <vector>
#include <array>

class MMU;

namespace interrupts
{
constexpr int IE_ADDRESS = 0xFFFF;
constexpr int IF_ADDRESS = 0xFF0F;

namespace bitpos
{
static constexpr int VBLANK = 0;
static constexpr int LCD = 1;
static constexpr int TIMER = 2;
static constexpr int SERIAL = 3;
static constexpr int JOYPAD = 4;
} // namespace bitpos

namespace vectorAddress
{
static constexpr std::uint16_t VBLANK = 0x40;
static constexpr std::uint16_t LCD = 0x48;
static constexpr std::uint16_t TIMER = 0x50;
static constexpr std::uint16_t SERIAL = 0x58;
static constexpr std::uint16_t JOYPAD = 0x60;
} // namespace vectorAddress

} // namespace interrupts

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
          std::uint8_t F;
          std::uint8_t A;
        };
        std::uint16_t AF;
      };
      union
      {
        struct
        {
          std::uint8_t C;
          std::uint8_t B;
        };
        std::uint16_t BC;
      };
      union
      {
        struct
        {
          std::uint8_t E;
          std::uint8_t D;
        };
        std::uint16_t DE;
      };
      union
      {
        struct
        {
          std::uint8_t L;
          std::uint8_t H;
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

  void SanitizeFlags();

  bool IsInterruptEnabled(int interruptBitpos);
  void EnableInterrupt(int interruptBitpos);
  void DisableInterrupt(int interruptBitpos);

  bool IsInterruptPending(int interruptBitpos);
  void RequestInterrupt(int interruptBitpos);
  void ResetInterrupt(int interruptBitpos);

  std::uint8_t GetIF();
  std::uint8_t GetIE();
  void SetIF(const std::uint8_t value);
  void SetIE(const std::uint8_t value);

  void HandleIMESetting();
  void HandleInterrupts();

  bool halted = false;
  bool jumped = false;

  bool setIMEAfterNextInstruction = false;

  void ExecuteOpcode(std::uint8_t opcode);
  void ExecuteExtendedOpcode(std::uint8_t opcode);
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

  void RST_VEC(const std::uint16_t addr);

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
  static constexpr std::uint8_t extendedOpcodePrefix = 0xCB;

  struct OpcodeDescription
  {
    OpcodeFunction opcode;
    int length;
    std::vector<int> cycles;
  };

  std::array<OpcodeDescription, 256> opcodeTable = {
      {{&CPU::NOP, 1, {4}},
       {&CPU::LD_BC_N16, 3, {12}},
       {&CPU::LD_dBC_A, 1, {8}},
       {&CPU::INC_BC, 1, {8}},
       {&CPU::INC_B, 1, {4}},
       {&CPU::DEC_B, 1, {4}},
       {&CPU::LD_B_N8, 2, {8}},
       {&CPU::RLCA, 1, {4}},
       {&CPU::LD_DN16_SP, 3, {20}},
       {&CPU::ADD_HL_BC, 1, {8}},
       {&CPU::LD_A_DBC, 1, {8}},
       {&CPU::DEC_BC, 1, {8}},
       {&CPU::INC_C, 1, {4}},
       {&CPU::DEC_C, 1, {4}},
       {&CPU::LD_C_N8, 2, {8}},
       {&CPU::RRCA, 1, {4}},
       // 0x10
       {&CPU::STOP_N8, 2, {4}},
       {&CPU::LD_DE_N16, 3, {12}},
       {&CPU::LD_dDE_A, 1, {8}},
       {&CPU::INC_DE, 1, {8}},
       {&CPU::INC_D, 1, {4}},
       {&CPU::DEC_D, 1, {4}},
       {&CPU::LD_D_N8, 2, {8}},
       {&CPU::RLA, 1, {4}},
       {&CPU::JR_E8, 2, {12}},
       {&CPU::ADD_HL_DE, 1, {8}},
       {&CPU::LD_A_dDE, 1, {8}},
       {&CPU::DEC_DE, 1, {8}},
       {&CPU::INC_E, 1, {4}},
       {&CPU::DEC_E, 1, {4}},
       {&CPU::LD_E_N8, 2, {8}},
       {&CPU::RRA, 1, {4}},
       // 0x20
       {&CPU::JR_NZ_E8, 2, {12, 8}},
       {&CPU::LD_HL_N16, 3, {12}},
       {&CPU::LD_dHLi_A, 1, {8}},
       {&CPU::INC_HL, 1, {8}},
       {&CPU::INC_H, 1, {4}},
       {&CPU::DEC_H, 1, {4}},
       {&CPU::LD_H_N8, 2, {8}},
       {&CPU::DAA, 1, {4}},
       {&CPU::JR_Z_E8, 2, {12, 8}},
       {&CPU::ADD_HL_HL, 1, {8}},
       {&CPU::LD_A_dHLi, 1, {8}},
       {&CPU::DEC_HL, 1, {8}},
       {&CPU::INC_L, 1, {4}},
       {&CPU::DEC_L, 1, {4}},
       {&CPU::LD_L_N8, 2, {8}},
       {&CPU::CPL, 1, {4}},
       // 0x30
       {&CPU::JR_NC_E8, 2, {12, 8}},
       {&CPU::LD_SP_N16, 3, {12}},
       {&CPU::LD_dHLd_A, 1, {8}},
       {&CPU::INC_SP, 1, {8}},
       {&CPU::INC_dHL, 1, {12}},
       {&CPU::DEC_dHL, 1, {12}},
       {&CPU::LD_dHL_N8, 2, {12}},
       {&CPU::SCF, 1, {4}},
       {&CPU::JR_C_E8, 2, {12, 8}},
       {&CPU::ADD_HL_SP, 1, {8}},
       {&CPU::LD_A_dHLd, 1, {8}},
       {&CPU::DEC_SP, 1, {8}},
       {&CPU::INC_A, 1, {4}},
       {&CPU::DEC_A, 1, {4}},
       {&CPU::LD_A_N8, 2, {8}},
       {&CPU::CCF, 1, {4}},
       // 0x40
       {&CPU::LD_B_B, 1, {4}},
       {&CPU::LD_B_C, 1, {4}},
       {&CPU::LD_B_D, 1, {4}},
       {&CPU::LD_B_E, 1, {4}},
       {&CPU::LD_B_H, 1, {4}},
       {&CPU::LD_B_L, 1, {4}},
       {&CPU::LD_B_dHL, 1, {8}},
       {&CPU::LD_B_A, 1, {4}},
       {&CPU::LD_C_B, 1, {4}},
       {&CPU::LD_C_C, 1, {4}},
       {&CPU::LD_C_D, 1, {4}},
       {&CPU::LD_C_E, 1, {4}},
       {&CPU::LD_C_H, 1, {4}},
       {&CPU::LD_C_L, 1, {4}},
       {&CPU::LD_C_dHL, 1, {8}},
       {&CPU::LD_C_A, 1, {4}},
       // 0x50
       {&CPU::LD_D_B, 1, {4}},
       {&CPU::LD_D_C, 1, {4}},
       {&CPU::LD_D_D, 1, {4}},
       {&CPU::LD_D_E, 1, {4}},
       {&CPU::LD_D_H, 1, {4}},
       {&CPU::LD_D_L, 1, {4}},
       {&CPU::LD_D_dHL, 1, {8}},
       {&CPU::LD_D_A, 1, {4}},
       {&CPU::LD_E_B, 1, {4}},
       {&CPU::LD_E_C, 1, {4}},
       {&CPU::LD_E_D, 1, {4}},
       {&CPU::LD_E_E, 1, {4}},
       {&CPU::LD_E_H, 1, {4}},
       {&CPU::LD_E_L, 1, {4}},
       {&CPU::LD_E_dHL, 1, {8}},
       {&CPU::LD_E_A, 1, {4}},
       // 0x60
       {&CPU::LD_H_B, 1, {4}},
       {&CPU::LD_H_C, 1, {4}},
       {&CPU::LD_H_D, 1, {4}},
       {&CPU::LD_H_E, 1, {4}},
       {&CPU::LD_H_H, 1, {4}},
       {&CPU::LD_H_L, 1, {4}},
       {&CPU::LD_H_dHL, 1, {8}},
       {&CPU::LD_H_A, 1, {4}},
       {&CPU::LD_L_B, 1, {4}},
       {&CPU::LD_L_C, 1, {4}},
       {&CPU::LD_L_D, 1, {4}},
       {&CPU::LD_L_E, 1, {4}},
       {&CPU::LD_L_H, 1, {4}},
       {&CPU::LD_L_L, 1, {4}},
       {&CPU::LD_L_dHL, 1, {8}},
       {&CPU::LD_L_A, 1, {4}},
       // 0x70
       {&CPU::LD_dHL_B, 1, {8}},
       {&CPU::LD_dHL_C, 1, {8}},
       {&CPU::LD_dHL_D, 1, {8}},
       {&CPU::LD_dHL_E, 1, {8}},
       {&CPU::LD_dHL_H, 1, {8}},
       {&CPU::LD_dHL_L, 1, {8}},
       {&CPU::HALT, 1, {4}},
       {&CPU::LD_dHL_A, 1, {8}},
       {&CPU::LD_A_B, 1, {4}},
       {&CPU::LD_A_C, 1, {4}},
       {&CPU::LD_A_D, 1, {4}},
       {&CPU::LD_A_E, 1, {4}},
       {&CPU::LD_A_H, 1, {4}},
       {&CPU::LD_A_L, 1, {4}},
       {&CPU::LD_A_dHL, 1, {8}},
       {&CPU::LD_A_A, 1, {4}},
       // 0x80
       {&CPU::ADD_A_B, 1, {4}},
       {&CPU::ADD_A_C, 1, {4}},
       {&CPU::ADD_A_D, 1, {4}},
       {&CPU::ADD_A_E, 1, {4}},
       {&CPU::ADD_A_H, 1, {4}},
       {&CPU::ADD_A_L, 1, {4}},
       {&CPU::ADD_A_dHL, 1, {8}},
       {&CPU::ADD_A_A, 1, {4}},
       {&CPU::ADC_A_B, 1, {4}},
       {&CPU::ADC_A_C, 1, {4}},
       {&CPU::ADC_A_D, 1, {4}},
       {&CPU::ADC_A_E, 1, {4}},
       {&CPU::ADC_A_H, 1, {4}},
       {&CPU::ADC_A_L, 1, {4}},
       {&CPU::ADC_A_dHL, 1, {8}},
       {&CPU::ADC_A_A, 1, {4}},
       // 0x90
       {&CPU::SUB_A_B, 1, {4}},
       {&CPU::SUB_A_C, 1, {4}},
       {&CPU::SUB_A_D, 1, {4}},
       {&CPU::SUB_A_E, 1, {4}},
       {&CPU::SUB_A_H, 1, {4}},
       {&CPU::SUB_A_L, 1, {4}},
       {&CPU::SUB_A_dHL, 1, {8}},
       {&CPU::SUB_A_A, 1, {4}},
       {&CPU::SBC_A_B, 1, {4}},
       {&CPU::SBC_A_C, 1, {4}},
       {&CPU::SBC_A_D, 1, {4}},
       {&CPU::SBC_A_E, 1, {4}},
       {&CPU::SBC_A_H, 1, {4}},
       {&CPU::SBC_A_L, 1, {4}},
       {&CPU::SBC_A_dHL, 1, {8}},
       {&CPU::SBC_A_A, 1, {4}},
       // 0xA0
       {&CPU::AND_A_B, 1, {4}},
       {&CPU::AND_A_C, 1, {4}},
       {&CPU::AND_A_D, 1, {4}},
       {&CPU::AND_A_E, 1, {4}},
       {&CPU::AND_A_H, 1, {4}},
       {&CPU::AND_A_L, 1, {4}},
       {&CPU::AND_A_dHL, 1, {8}},
       {&CPU::AND_A_A, 1, {4}},
       {&CPU::XOR_A_B, 1, {4}},
       {&CPU::XOR_A_C, 1, {4}},
       {&CPU::XOR_A_D, 1, {4}},
       {&CPU::XOR_A_E, 1, {4}},
       {&CPU::XOR_A_H, 1, {4}},
       {&CPU::XOR_A_L, 1, {4}},
       {&CPU::XOR_A_dHL, 1, {8}},
       {&CPU::XOR_A_A, 1, {4}},
       // 0xB0
       {&CPU::OR_A_B, 1, {4}},
       {&CPU::OR_A_C, 1, {4}},
       {&CPU::OR_A_D, 1, {4}},
       {&CPU::OR_A_E, 1, {4}},
       {&CPU::OR_A_H, 1, {4}},
       {&CPU::OR_A_L, 1, {4}},
       {&CPU::OR_A_dHL, 1, {8}},
       {&CPU::OR_A_A, 1, {4}},
       {&CPU::CP_A_B, 1, {4}},
       {&CPU::CP_A_C, 1, {4}},
       {&CPU::CP_A_D, 1, {4}},
       {&CPU::CP_A_E, 1, {4}},
       {&CPU::CP_A_H, 1, {4}},
       {&CPU::CP_A_L, 1, {4}},
       {&CPU::CP_A_dHL, 1, {8}},
       {&CPU::CP_A_A, 1, {4}},
       // 0xC0
       {&CPU::RET_NZ, 1, {20, 8}},
       {&CPU::POP_BC, 1, {12}},
       {&CPU::JP_NZ_A16, 3, {16, 12}},
       {&CPU::JP_A16, 3, {16}},
       {&CPU::CALL_NZ_A16, 3, {24, 12}},
       {&CPU::PUSH_BC, 1, {16}},
       {&CPU::ADD_A_N8, 2, {8}},
       {&CPU::RST_00, 1, {16}},
       {&CPU::RET_Z, 1, {20, 8}},
       {&CPU::RET, 1, {16}},
       {&CPU::JP_Z_A16, 3, {16, 12}},
       {&CPU::NOP, 1, {4}}, // This is the CB prefix opcode, not a NOP.
       {&CPU::CALL_Z_A16, 3, {24, 12}},
       {&CPU::CALL_A16, 3, {24}},
       {&CPU::ADC_A_N8, 2, {8}},
       {&CPU::RST_08, 1, {16}},
       // 0xD0
       {&CPU::RET_NC, 1, {20, 8}},
       {&CPU::POP_DE, 1, {12}},
       {&CPU::JP_NC_A16, 3, {16, 12}},
       {&CPU::NOP, 0, {}}, // Invalid opcode, usually treated as NOP or unhandled
       {&CPU::CALL_NC_A16, 3, {24, 12}},
       {&CPU::PUSH_DE, 1, {16}},
       {&CPU::SUB_A_N8, 2, {8}},
       {&CPU::RST_10, 1, {16}},
       {&CPU::RET_C, 1, {20, 8}},
       {&CPU::RETI, 1, {16}},
       {&CPU::JP_C_A16, 3, {16, 12}},
       {&CPU::NOP, 0, {}}, // Invalid opcode
       {&CPU::CALL_C_A16, 3, {24, 12}},
       {&CPU::NOP, 0, {}}, // Invalid opcode
       {&CPU::SBC_A_N8, 2, {8}},
       {&CPU::RST_18, 1, {16}},
       // 0xE0
       {&CPU::LDH_dA8_A, 2, {12}},
       {&CPU::POP_HL, 1, {12}},
       {&CPU::LDH_dC_A, 2, {8}},
       {&CPU::NOP, 0, {}}, // Invalid opcode
       {&CPU::NOP, 0, {}}, // Invalid opcode
       {&CPU::PUSH_HL, 1, {16}},
       {&CPU::AND_A_N8, 2, {8}},
       {&CPU::RST_20, 1, {16}},
       {&CPU::ADD_SP_E8, 2, {16}},
       {&CPU::JP_HL, 1, {4}},
       {&CPU::LD_dA16_A, 3, {16}},
       {&CPU::NOP, 0, {}}, // Invalid opcode
       {&CPU::NOP, 0, {}}, // Invalid opcode
       {&CPU::NOP, 0, {}}, // Invalid opcode
       {&CPU::XOR_A_N8, 2, {8}},
       {&CPU::RST_28, 1, {16}},
       // 0xF0
       {&CPU::LDH_A_dA8, 2, {12}},
       {&CPU::POP_AF, 1, {12}},
       {&CPU::LDH_A_dC, 2, {8}},
       {&CPU::DI, 1, {4}},
       {&CPU::NOP, 0, {}}, // Invalid opcode
       {&CPU::PUSH_AF, 1, {16}},
       {&CPU::OR_A_N8, 2, {8}},
       {&CPU::RST_30, 1, {16}},
       {&CPU::LD_HL_SP_p_E8, 2, {12}},
       {&CPU::LD_SP_HL, 1, {8}},
       {&CPU::LD_A_dA16, 3, {16}},
       {&CPU::EI, 1, {4}},
       {&CPU::NOP, 0, {}}, // Invalid opcode
       {&CPU::NOP, 0, {}}, // Invalid opcode
       {&CPU::CP_A_N8, 2, {8}},
       {&CPU::RST_38, 1, {16}}}};

  std::array<OpcodeDescription, 256> extendedOpcodeTable = {
      // 0x00
      {{&CPU::RLC_B, 2, {8}},
       {&CPU::RLC_C, 2, {8}},
       {&CPU::RLC_D, 2, {8}},
       {&CPU::RLC_E, 2, {8}},
       {&CPU::RLC_H, 2, {8}},
       {&CPU::RLC_L, 2, {8}},
       {&CPU::RLC_dHL, 2, {16}},
       {&CPU::RLC_A, 2, {8}},
       {&CPU::RRC_B, 2, {8}},
       {&CPU::RRC_C, 2, {8}},
       {&CPU::RRC_D, 2, {8}},
       {&CPU::RRC_E, 2, {8}},
       {&CPU::RRC_H, 2, {8}},
       {&CPU::RRC_L, 2, {8}},
       {&CPU::RRC_dHL, 2, {16}},
       {&CPU::RRC_A, 2, {8}},
       // 0x10
       {&CPU::RL_B, 2, {8}},
       {&CPU::RL_C, 2, {8}},
       {&CPU::RL_D, 2, {8}},
       {&CPU::RL_E, 2, {8}},
       {&CPU::RL_H, 2, {8}},
       {&CPU::RL_L, 2, {8}},
       {&CPU::RL_dHL, 2, {16}},
       {&CPU::RL_A, 2, {8}},
       {&CPU::RR_B, 2, {8}},
       {&CPU::RR_C, 2, {8}},
       {&CPU::RR_D, 2, {8}},
       {&CPU::RR_E, 2, {8}},
       {&CPU::RR_H, 2, {8}},
       {&CPU::RR_L, 2, {8}},
       {&CPU::RR_dHL, 2, {16}},
       {&CPU::RR_A, 2, {8}},
       // 0x20
       {&CPU::SLA_B, 2, {8}},
       {&CPU::SLA_C, 2, {8}},
       {&CPU::SLA_D, 2, {8}},
       {&CPU::SLA_E, 2, {8}},
       {&CPU::SLA_H, 2, {8}},
       {&CPU::SLA_L, 2, {8}},
       {&CPU::SLA_dHL, 2, {16}},
       {&CPU::SLA_A, 2, {8}},
       {&CPU::SRA_B, 2, {8}},
       {&CPU::SRA_C, 2, {8}},
       {&CPU::SRA_D, 2, {8}},
       {&CPU::SRA_E, 2, {8}},
       {&CPU::SRA_H, 2, {8}},
       {&CPU::SRA_L, 2, {8}},
       {&CPU::SRA_dHL, 2, {16}},
       {&CPU::SRA_A, 2, {8}},
       // 0x30
       {&CPU::SWAP_B, 2, {8}},
       {&CPU::SWAP_C, 2, {8}},
       {&CPU::SWAP_D, 2, {8}},
       {&CPU::SWAP_E, 2, {8}},
       {&CPU::SWAP_H, 2, {8}},
       {&CPU::SWAP_L, 2, {8}},
       {&CPU::SWAP_dHL, 2, {16}},
       {&CPU::SWAP_A, 2, {8}},
       {&CPU::SRL_B, 2, {8}},
       {&CPU::SRL_C, 2, {8}},
       {&CPU::SRL_D, 2, {8}},
       {&CPU::SRL_E, 2, {8}},
       {&CPU::SRL_H, 2, {8}},
       {&CPU::SRL_L, 2, {8}},
       {&CPU::SRL_dHL, 2, {16}},
       {&CPU::SRL_A, 2, {8}},
       // 0x40
       {&CPU::BIT_0_B, 2, {8}},
       {&CPU::BIT_0_C, 2, {8}},
       {&CPU::BIT_0_D, 2, {8}},
       {&CPU::BIT_0_E, 2, {8}},
       {&CPU::BIT_0_H, 2, {8}},
       {&CPU::BIT_0_L, 2, {8}},
       {&CPU::BIT_0_dHL, 2, {16}},
       {&CPU::BIT_0_A, 2, {8}},
       {&CPU::BIT_1_B, 2, {8}},
       {&CPU::BIT_1_C, 2, {8}},
       {&CPU::BIT_1_D, 2, {8}},
       {&CPU::BIT_1_E, 2, {8}},
       {&CPU::BIT_1_H, 2, {8}},
       {&CPU::BIT_1_L, 2, {8}},
       {&CPU::BIT_1_dHL, 2, {16}},
       {&CPU::BIT_1_A, 2, {8}},
       // 0x50
       {&CPU::BIT_2_B, 2, {8}},
       {&CPU::BIT_2_C, 2, {8}},
       {&CPU::BIT_2_D, 2, {8}},
       {&CPU::BIT_2_E, 2, {8}},
       {&CPU::BIT_2_H, 2, {8}},
       {&CPU::BIT_2_L, 2, {8}},
       {&CPU::BIT_2_dHL, 2, {16}},
       {&CPU::BIT_2_A, 2, {8}},
       {&CPU::BIT_3_B, 2, {8}},
       {&CPU::BIT_3_C, 2, {8}},
       {&CPU::BIT_3_D, 2, {8}},
       {&CPU::BIT_3_E, 2, {8}},
       {&CPU::BIT_3_H, 2, {8}},
       {&CPU::BIT_3_L, 2, {8}},
       {&CPU::BIT_3_dHL, 2, {16}},
       {&CPU::BIT_3_A, 2, {8}},
       // 0x60
       {&CPU::BIT_4_B, 2, {8}},
       {&CPU::BIT_4_C, 2, {8}},
       {&CPU::BIT_4_D, 2, {8}},
       {&CPU::BIT_4_E, 2, {8}},
       {&CPU::BIT_4_H, 2, {8}},
       {&CPU::BIT_4_L, 2, {8}},
       {&CPU::BIT_4_dHL, 2, {16}},
       {&CPU::BIT_4_A, 2, {8}},
       {&CPU::BIT_5_B, 2, {8}},
       {&CPU::BIT_5_C, 2, {8}},
       {&CPU::BIT_5_D, 2, {8}},
       {&CPU::BIT_5_E, 2, {8}},
       {&CPU::BIT_5_H, 2, {8}},
       {&CPU::BIT_5_L, 2, {8}},
       {&CPU::BIT_5_dHL, 2, {16}},
       {&CPU::BIT_5_A, 2, {8}},
       // 0x70
       {&CPU::BIT_6_B, 2, {8}},
       {&CPU::BIT_6_C, 2, {8}},
       {&CPU::BIT_6_D, 2, {8}},
       {&CPU::BIT_6_E, 2, {8}},
       {&CPU::BIT_6_H, 2, {8}},
       {&CPU::BIT_6_L, 2, {8}},
       {&CPU::BIT_6_dHL, 2, {16}},
       {&CPU::BIT_6_A, 2, {8}},
       {&CPU::BIT_7_B, 2, {8}},
       {&CPU::BIT_7_C, 2, {8}},
       {&CPU::BIT_7_D, 2, {8}},
       {&CPU::BIT_7_E, 2, {8}},
       {&CPU::BIT_7_H, 2, {8}},
       {&CPU::BIT_7_L, 2, {8}},
       {&CPU::BIT_7_dHL, 2, {16}},
       {&CPU::BIT_7_A, 2, {8}},
       // 0x80
       {&CPU::RES_0_B, 2, {8}},
       {&CPU::RES_0_C, 2, {8}},
       {&CPU::RES_0_D, 2, {8}},
       {&CPU::RES_0_E, 2, {8}},
       {&CPU::RES_0_H, 2, {8}},
       {&CPU::RES_0_L, 2, {8}},
       {&CPU::RES_0_dHL, 2, {16}},
       {&CPU::RES_0_A, 2, {8}},
       {&CPU::RES_1_B, 2, {8}},
       {&CPU::RES_1_C, 2, {8}},
       {&CPU::RES_1_D, 2, {8}},
       {&CPU::RES_1_E, 2, {8}},
       {&CPU::RES_1_H, 2, {8}},
       {&CPU::RES_1_L, 2, {8}},
       {&CPU::RES_1_dHL, 2, {16}},
       {&CPU::RES_1_A, 2, {8}},
       // 0x90
       {&CPU::RES_2_B, 2, {8}},
       {&CPU::RES_2_C, 2, {8}},
       {&CPU::RES_2_D, 2, {8}},
       {&CPU::RES_2_E, 2, {8}},
       {&CPU::RES_2_H, 2, {8}},
       {&CPU::RES_2_L, 2, {8}},
       {&CPU::RES_2_dHL, 2, {16}},
       {&CPU::RES_2_A, 2, {8}},
       {&CPU::RES_3_B, 2, {8}},
       {&CPU::RES_3_C, 2, {8}},
       {&CPU::RES_3_D, 2, {8}},
       {&CPU::RES_3_E, 2, {8}},
       {&CPU::RES_3_H, 2, {8}},
       {&CPU::RES_3_L, 2, {8}},
       {&CPU::RES_3_dHL, 2, {16}},
       {&CPU::RES_3_A, 2, {8}},
       // 0xA0
       {&CPU::RES_4_B, 2, {8}},
       {&CPU::RES_4_C, 2, {8}},
       {&CPU::RES_4_D, 2, {8}},
       {&CPU::RES_4_E, 2, {8}},
       {&CPU::RES_4_H, 2, {8}},
       {&CPU::RES_4_L, 2, {8}},
       {&CPU::RES_4_dHL, 2, {16}},
       {&CPU::RES_4_A, 2, {8}},
       {&CPU::RES_5_B, 2, {8}},
       {&CPU::RES_5_C, 2, {8}},
       {&CPU::RES_5_D, 2, {8}},
       {&CPU::RES_5_E, 2, {8}},
       {&CPU::RES_5_H, 2, {8}},
       {&CPU::RES_5_L, 2, {8}},
       {&CPU::RES_5_dHL, 2, {16}},
       {&CPU::RES_5_A, 2, {8}},
       // 0xB0
       {&CPU::RES_6_B, 2, {8}},
       {&CPU::RES_6_C, 2, {8}},
       {&CPU::RES_6_D, 2, {8}},
       {&CPU::RES_6_E, 2, {8}},
       {&CPU::RES_6_H, 2, {8}},
       {&CPU::RES_6_L, 2, {8}},
       {&CPU::RES_6_dHL, 2, {16}},
       {&CPU::RES_6_A, 2, {8}},
       {&CPU::RES_7_B, 2, {8}},
       {&CPU::RES_7_C, 2, {8}},
       {&CPU::RES_7_D, 2, {8}},
       {&CPU::RES_7_E, 2, {8}},
       {&CPU::RES_7_H, 2, {8}},
       {&CPU::RES_7_L, 2, {8}},
       {&CPU::RES_7_dHL, 2, {16}},
       {&CPU::RES_7_A, 2, {8}},
       // 0xC0
       {&CPU::SET_0_B, 2, {8}},
       {&CPU::SET_0_C, 2, {8}},
       {&CPU::SET_0_D, 2, {8}},
       {&CPU::SET_0_E, 2, {8}},
       {&CPU::SET_0_H, 2, {8}},
       {&CPU::SET_0_L, 2, {8}},
       {&CPU::SET_0_dHL, 2, {16}},
       {&CPU::SET_0_A, 2, {8}},
       {&CPU::SET_1_B, 2, {8}},
       {&CPU::SET_1_C, 2, {8}},
       {&CPU::SET_1_D, 2, {8}},
       {&CPU::SET_1_E, 2, {8}},
       {&CPU::SET_1_H, 2, {8}},
       {&CPU::SET_1_L, 2, {8}},
       {&CPU::SET_1_dHL, 2, {16}},
       {&CPU::SET_1_A, 2, {8}},
       // 0xD0
       {&CPU::SET_2_B, 2, {8}},
       {&CPU::SET_2_C, 2, {8}},
       {&CPU::SET_2_D, 2, {8}},
       {&CPU::SET_2_E, 2, {8}},
       {&CPU::SET_2_H, 2, {8}},
       {&CPU::SET_2_L, 2, {8}},
       {&CPU::SET_2_dHL, 2, {16}},
       {&CPU::SET_2_A, 2, {8}},
       {&CPU::SET_3_B, 2, {8}},
       {&CPU::SET_3_C, 2, {8}},
       {&CPU::SET_3_D, 2, {8}},
       {&CPU::SET_3_E, 2, {8}},
       {&CPU::SET_3_H, 2, {8}},
       {&CPU::SET_3_L, 2, {8}},
       {&CPU::SET_3_dHL, 2, {16}},
       {&CPU::SET_3_A, 2, {8}},
       // 0xE0
       {&CPU::SET_4_B, 2, {8}},
       {&CPU::SET_4_C, 2, {8}},
       {&CPU::SET_4_D, 2, {8}},
       {&CPU::SET_4_E, 2, {8}},
       {&CPU::SET_4_H, 2, {8}},
       {&CPU::SET_4_L, 2, {8}},
       {&CPU::SET_4_dHL, 2, {16}},
       {&CPU::SET_4_A, 2, {8}},
       {&CPU::SET_5_B, 2, {8}},
       {&CPU::SET_5_C, 2, {8}},
       {&CPU::SET_5_D, 2, {8}},
       {&CPU::SET_5_E, 2, {8}},
       {&CPU::SET_5_H, 2, {8}},
       {&CPU::SET_5_L, 2, {8}},
       {&CPU::SET_5_dHL, 2, {16}},
       {&CPU::SET_5_A, 2, {8}},
       // 0xF0
       {&CPU::SET_6_B, 2, {8}},
       {&CPU::SET_6_C, 2, {8}},
       {&CPU::SET_6_D, 2, {8}},
       {&CPU::SET_6_E, 2, {8}},
       {&CPU::SET_6_H, 2, {8}},
       {&CPU::SET_6_L, 2, {8}},
       {&CPU::SET_6_dHL, 2, {16}},
       {&CPU::SET_6_A, 2, {8}},
       {&CPU::SET_7_B, 2, {8}},
       {&CPU::SET_7_C, 2, {8}},
       {&CPU::SET_7_D, 2, {8}},
       {&CPU::SET_7_E, 2, {8}},
       {&CPU::SET_7_H, 2, {8}},
       {&CPU::SET_7_L, 2, {8}},
       {&CPU::SET_7_dHL, 2, {16}},
       {&CPU::SET_7_A, 2, {8}}}};

  void PrintCPUState();
  void PrintBLARGGSerial();

public:
  CPU(MMU& mmu) : mmu(mmu)
  {
    registers.A = 0x01;
    registers.F = 0xB0;
    registers.B = 0x00;
    registers.C = 0x13;
    registers.D = 0x00;
    registers.E = 0xD8;
    registers.H = 0x01;
    registers.L = 0x4D;
    registers.SP = 0xFFFE;
    registers.PC = 0x100;
    registers.IME = false;
  }
  void Tick();

  bool IsHalted() { return halted; }
};
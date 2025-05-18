#pragma once

#include <cstdint>

class MMU;

struct Registers
{
  std::uint8_t A;
  std::uint8_t B;
  std::uint8_t C;
  std::uint8_t D;
  std::uint8_t E;
  std::uint8_t H;
  std::uint8_t L;
};

struct Flags
{
};

class CPU
{
  MMU& mmu;

  Registers registers;
  Flags flags;

  std::uint16_t sp{0};
  std::uint16_t pc{0};

  bool halted = false;

  void executeOPCode(std::uint8_t opcode);

  void halt();

public:
  CPU(MMU& mmu) : mmu(mmu) {}
  void Tick();

  bool isHalted() { return halted; }
};
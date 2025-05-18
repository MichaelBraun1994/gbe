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
  Registers registers;
  Flags flags;
  MMU& mmu;

public:
  CPU(MMU& mmu) : mmu(mmu) {}
  void Tick();
};
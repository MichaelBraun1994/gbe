#pragma once

#include <string>
#include <cstdint>
#include <limits>
#include <array>

using Address = std::uint16_t;

class MMU
{
  static constexpr int memorySize = std::numeric_limits<std::uint16_t>::max() + 1;
  std::array<std::uint8_t, memorySize> memmory;

public:
  MMU();

  void LoadROM(const std::string& filePath);

  void Set(Address address, std::uint8_t value);
  std::uint8_t Get(Address address);
};
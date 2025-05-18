#pragma once

#include <string>
#include <cstdint>
#include <limits>

using Address = std::uint16_t;

class MMU
{
  static constexpr int memorySize = std::numeric_limits<std::uint16_t>::max();

  std::uint8_t memmory[memorySize];

  void AssertAddress(Address address);

public:
  void LoadROM(const std::string& filePath);

  void Set(Address address, std::uint8_t value);
  std::uint8_t Get(Address address);
};
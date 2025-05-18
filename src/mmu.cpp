#include "mmu.hpp"

#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

void MMU::AssertAddress(Address address)
{
  if (address >= memorySize)
  {
    throw std::runtime_error{"Out of bounds memory access: " + std::to_string(address)};
  }
}

MMU::MMU()
{
  std::fill(memmory.begin(), memmory.end(), 0xFF);
}

void MMU::LoadROM(const std::string& filePath)
{
  std::filesystem::path path{filePath};
  if (!std::filesystem::exists(path))
  {
    throw std::runtime_error{"ROM file does not exist."};
  }

  std::fstream romFile{path.c_str(), std::ios::in | std::ios::binary | std::ios::ate};

  if (!romFile)
  {
    throw std::runtime_error{"Unable to open ROM."};
  }

  std::streamsize fileSize = romFile.tellg();
  romFile.seekg(0, std::iostream::beg);

  if (!romFile.read(reinterpret_cast<char*>(memmory.data()), fileSize))
  {
    throw std::runtime_error{"Unable to read ROM."};
  }
}

void MMU::Set(Address address, std::uint8_t value)
{
  AssertAddress(address);
  memmory[address] = value;
}

std::uint8_t MMU::Get(Address address)
{
  AssertAddress(address);
  return memmory[address];
}

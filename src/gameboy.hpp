#pragma once

#include <string>
#include <memory>

class CPU;
class MMU;
class PPU;
class Controls;

class GameBoy
{
  static constexpr int displayWidth = 160;
  static constexpr int displayHeight = 144;

  std::unique_ptr<MMU> mmu;
  std::unique_ptr<CPU> cpu;
  std::unique_ptr<PPU> ppu;
  std::unique_ptr<Controls> controls;

  bool turnedOn = false;

  void HandleInputs();

public:
  GameBoy(std::unique_ptr<MMU> mmu, std::unique_ptr<CPU> cpu, std::unique_ptr<PPU> ppu,
          std::unique_ptr<Controls> controls);
  ~GameBoy();

  static std::unique_ptr<GameBoy> Create();

  void LoadROM(const std::string& path);
  void TurnOn();
  void TurnOff();
};
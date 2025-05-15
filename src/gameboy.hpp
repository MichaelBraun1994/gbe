#pragma once

#include <string>
#include <memory>

class Cpu;
class Renderer;
class Controls;

class GameBoy
{
  static constexpr int displayWidth = 100;
  static constexpr int displayHeight = 100;

  std::unique_ptr<Cpu> cpu;
  std::unique_ptr<Renderer> renderer;
  std::unique_ptr<Controls> controls;

  bool turnedOn = false;

public:
  GameBoy(std::unique_ptr<Cpu> cpu, std::unique_ptr<Renderer> renderer, std::unique_ptr<Controls> controls);
  ~GameBoy();

  static std::unique_ptr<GameBoy> Create();

  void Load(const std::string& path);
  void TurnOn();
  void TurnOff();
};
#include "gameboy.hpp"

#include <plog/Log.h>

#include "cpu/cpu.hpp"
#include "controls.hpp"
#include "renderer.hpp"

std::unique_ptr<GameBoy> GameBoy::Create()
{
  return std::make_unique<GameBoy>(std::make_unique<Cpu>(), std::make_unique<Renderer>(displayWidth, displayHeight),
                                   std::make_unique<Controls>());
}

GameBoy::GameBoy(std::unique_ptr<Cpu> cpu, std::unique_ptr<Renderer> renderer, std::unique_ptr<Controls> controls)
    : cpu(std::move(cpu)), renderer(std::move(renderer)), controls(std::move(controls))
{
}
GameBoy::~GameBoy()
{
  TurnOff();
}

void GameBoy::Load(const std::string& path)
{
  return;
}

void GameBoy::TurnOn()
{
  if (!turnedOn)
  {
    PLOG(plog::info) << "Starting up GameBoy.";
    turnedOn = true;
  }

  while (turnedOn)
  {
    controls->Update();

    if (controls->IsPressed(Button::Right))
    {
      PLOG(plog::info) << "RIGHT";
    }
    if (controls->IsPressed(Button::PowerOff))
    {
      TurnOff();
    }
  }
}

void GameBoy::TurnOff()
{
  if (turnedOn)
  {
    PLOG(plog::info) << "Turning off GameBoy.";
    turnedOn = false;
  }
}
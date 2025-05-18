#include "gameboy.hpp"

#include <plog/Log.h>

#include "cpu/cpu.hpp"
#include "controls.hpp"
#include "mmu.hpp"
#include "ppu.hpp"

std::unique_ptr<GameBoy> GameBoy::Create()
{
  auto mmu = std::make_unique<MMU>();
  return std::make_unique<GameBoy>(std::move(mmu), std::make_unique<CPU>(*mmu),
                                   std::make_unique<PPU>(displayWidth, displayHeight), std::make_unique<Controls>());
}

GameBoy::GameBoy(std::unique_ptr<MMU> mmu, std::unique_ptr<CPU> cpu, std::unique_ptr<PPU> ppu,
                 std::unique_ptr<Controls> controls)
    : mmu(std::move(mmu)), cpu(std::move(cpu)), ppu(std::move(ppu)), controls(std::move(controls))
{
}
GameBoy::~GameBoy()
{
  TurnOff();
}

void GameBoy::LoadROM(const std::string& path)
{
  mmu->LoadROM(path);
  PLOG(plog::info) << "Loaded ROM.";
}

void GameBoy::HandleInputs()
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

void GameBoy::TurnOn()
{
  if (!turnedOn)
  {
    PLOG(plog::info) << "Starting up GameBoy.";
    turnedOn = true;
  }

  while (turnedOn)
  {
    HandleInputs();

    cpu->Tick();
    if (cpu->isHalted())
    {
      TurnOff();
    }

    ppu->Update();

    SDL_Delay(1);
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
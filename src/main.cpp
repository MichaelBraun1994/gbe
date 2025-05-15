#include "logger.hpp"
#include "gameboy.hpp"

int main(int argc, char** argv)
{
  Logger logger{"Logs.txt"};
  PLOG(plog::info) << "Starting application.";

  auto gameBoy = GameBoy::Create();
  gameBoy->Load("Test.gba");
  gameBoy->TurnOn();

  PLOG(plog::info) << "Finished application.";
  return 0;
}
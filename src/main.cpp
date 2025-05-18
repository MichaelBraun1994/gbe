#include "logger.hpp"
#include "gameboy.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: GBE PathToRom." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  Logger logger{"Logs.txt"};
  PLOG(plog::info) << "Starting application.";

  auto gameBoy = GameBoy::Create();
  gameBoy->LoadROM(argv[1]);
  gameBoy->TurnOn();

  PLOG(plog::info) << "Finished application.";
  return 0;
}
#include "cpu.hpp"
#include "../mmu.hpp"
#include "../logger.hpp"

void CPU::executeOPCode(std::uint8_t opcode)
{
  PLOG(plog::info) << "Executing OPCode: 0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex
                   << static_cast<int>(opcode);
}

void CPU::halt()
{
  halted = true;
}

void CPU::Tick()
{
  std::uint8_t opcode = mmu.Get(pc);
  executeOPCode(opcode);
  ++pc;

  if (pc >= 0x100)
  {
    halt();
  }
}

#include <iostream>
#include <vector>

#include "processing.h"
#include "symbol_table.h"
#include "memory.h"
#include "register.h"

int main(int argc, char *argv[])
{
  bool debug_mode = false, step_run = false;

  if (argc < 2)
  {
    std::cout<<"Invalid number of arguments!"<<std::endl;
    std::cout<<"usage: u_sim filename [--step-run] [--debug]"<<std::endl;
    exit(1);
  }

  for(int i = 2; i < argc; ++i)
  {
    auto arg = std::string(argv[i]);

    if (arg == "--step-run")
      step_run = true;
    else if (arg == "--debug")
      debug_mode = true;
    else
      throw std::invalid_argument(argv[i]);
  }

  // Read source file
  auto lines = read_file(argv[1]);

  auto registers = initialize_registers();

  char *text_segment = allocate_memory(registers[gp], registers[sp]);

  // Generate symbol table in the first pass.
  // Static data segments begins at global pointer i.e text_segment + offset
  auto symbol_table = SymbolTable(lines, text_segment + registers[gp]);

  if (debug_mode)
    std::cout<<symbol_table<<std::endl;

  deallocate_memory(text_segment);

  return 0;
}

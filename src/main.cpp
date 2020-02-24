#include <iostream>
#include <vector>
#include <bitset>

#include "processing.h"
#include "symbol_table.h"
#include "memory.h"
#include "register_file.h"
#include "options.h"
#include "instruction.h"

int main(int argc, char *argv[])
{
  char *text_segment = nullptr;

  try 
  {
    // Parse command line arguments
    auto opts = parse_opts(argc, argv);

    // Read source file
    auto lines = read_file(opts.source_file);

    RegisterFile register_file;

    // R2 is the global offset pointer
    text_segment = allocate_memory(register_file.GPR[2]);

    // Generate symbol table in the first pass.
    // Static data segments begins at global pointer i.e text_segment + offset
    auto symbol_table = SymbolTable(lines, text_segment + register_file.GPR[2]);

    if (opts.debug)
    {
      std::cout<<symbol_table<<std::endl;
      std::cout<<"Text segment base (0x0040 0000): "<<0x00400000<<std::endl;
      std::cout<<"Data segment base (0x1000 0000): "<<0x10000000<<std::endl;
      std::cout<<std::endl;
    }

    auto instructions  = translate_instructions(lines, symbol_table);

    if (opts.debug)
    {
      for (const auto &i: instructions)
      {
        std::cout<<i<<std::endl<<" -> ";
        std::cout<<std::bitset<8*sizeof(int)>(i.encode())<<std::endl;
      }
    }
  }
  catch (std::string error_msg) 
  {
    std::cout<<"Error: "<<error_msg<<std::endl;
  }

  if (text_segment)
    deallocate_memory(text_segment);

  return 0;
}

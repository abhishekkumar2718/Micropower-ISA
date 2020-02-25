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

    RegisterFile rf;

    // R2 is the global offset pointer
    text_segment = allocate_memory(rf.GPR[2]);

    // Generate symbol table in the first pass.
    // Static data segments begins at global pointer i.e text_segment + offset
    auto symbol_table = SymbolTable(lines, text_segment + rf.GPR[2]);

    if (opts.debug)
      std::cout<<symbol_table<<std::endl;

    auto instructions  = translate_instructions(lines, symbol_table);

    if (opts.debug)
    {
      for (const auto &i: instructions)
      {
        std::cout<<i<<std::endl<<" -> ";
        std::cout<<std::bitset<8*sizeof(int)>(i.encode())<<std::endl;
      }
      std::cout<<std::endl;
    }

    // Use CIA = 0x0 as exit condition
    // syscall 10 => CIA := 0
    while (rf.CIA)
    {
      auto i = instructions[(rf.CIA - text_segment_base)/4];

      if (opts.step_run)
      {
        std::cout<<std::hex<<rf.CIA<<": "<<i<<std::endl;
        i.execute(rf);
        std::getchar();
      }

      rf.CIA = rf.NIA;
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

#include <iostream>
#include <vector>
#include <fstream>

#include "processing.h"
#include "symbol_table.h"

int main(int argc, char *argv[])
{
  bool DEBUG = false, STEP_RUN = false;

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
      STEP_RUN = true;
    else if (arg == "--debug")
      DEBUG = true;
    else
      throw std::invalid_argument(argv[i]);
  }

  std::ifstream asm_filestream(argv[1]);

  if (!asm_filestream)
  {
    std::cout<<"Unable to open source file!"<<std::endl;
    exit(1);
  }

  // TODO: Add more validations for file

  std::vector<std::string>lines;
  std::string line;

  while (getline(asm_filestream, line))
    lines.push_back(line);


  preprocess_lines(lines);

  char *gp = new char[50];

  // Generate symbol table in the first pass.
  auto symbol_table = SymbolTable(lines, gp);

  if (DEBUG)
    std::cout<<symbol_table<<std::endl;

  return 0;
}

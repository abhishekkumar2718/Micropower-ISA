#include <iostream>
#include <vector>
#include <fstream>

#include "processing.h"
#include "label.h"
#include "symbol_table.h"

const int DEBUG = 1;

int main(int argc, char *argv[])
{
  if (argc != 2){
    std::cout<<"Invalid number of arguments!"<<std::endl;
    std::cout<<"usage: u_asm filename"<<std::endl;
    exit(1);
  }

  std::ifstream asm_filestream(argv[1]);

  if (!asm_filestream){
    std::cout<<"Unable to open source file!"<<std::endl;
    exit(1);
  }

  // TODO: Add more validations for file

  std::vector<std::string>lines;
  std::string line;

  std::vector<Label> labels;

  while (getline(asm_filestream, line))
    lines.push_back(line);

  // Process lines into labels, directives, and instructions
  process_lines(lines, labels);

  auto symbol_table = SymbolTable(labels);

  if (DEBUG)
    std::cout<<symbol_table<<std::endl;

  return 0;
}

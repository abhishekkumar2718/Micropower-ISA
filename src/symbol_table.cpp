#include <iostream>
#include <iomanip>
#include <algorithm>

#include "symbol_table.h"
#include "memory.h"
#include "processing.h"

// Generate the symbol table - Initializing values if given expressions
SymbolTable::SymbolTable(const std::vector<std::string>& lines, char* gp)
  : base(gp)
{
  size_t offset = 0;
  unsigned int instruction_count = 0;

  for (auto line: lines)
  {
    if (is_data_label(line))
    {
      // Format - symbol: .data_type [intialisation_expression]

      // Initialize a label
      Label l(line, gp + offset, offset);
      labels.push_back(l);

      offset += l.size;
    }
    else if (is_text_label(line))
    {
      // Format - symbol:

      // Copy upto colon
      auto symbol = line.substr(0, line.size() - 1);

      labels.push_back(Label(symbol, instruction_count));
    }
    else if (!is_assembly_directive(line))
      // Neither label or directive - Must be an instruction
      ++instruction_count;
  }
}

std::ostream& operator<<(std::ostream& os, const SymbolTable& symbol_table)
{
  if (symbol_table.empty())
    return os;

  os << std::left << std::setw(30) << "Label" << std::internal
    << "Offset" << " " << "Section" << " " << "Size" << std::endl;

  char* base;
  for (auto const& label: symbol_table.labels)
  {
    os << std::left << std::setw(30) << label.symbol << std::setw(7) <<
      label.offset << std::setw(7);

    if (label.section == Section::Data)
    {
      base = symbol_table.base + label.offset;

      os << "Data" << " " << label.size << std::endl;
      for (int i = 0; i < label.size; ++i)
        os << std::hex << int(*(base + i)) << " ";

      os << std::dec << std::endl;
    }
    else if (label.section == Section::Text)
      os << "Text" << std :: endl;
  }

  return os;
}

int SymbolTable::address(const std::string &label) const
{
  for (const auto &l: labels)
  {
    if (l.symbol == label)
    {
      if (l.section == Section::Data)
        return data_segment_base + l.offset;
      else
        return text_segment_base + l.offset * 4;
    }
  }

  return 0;
}

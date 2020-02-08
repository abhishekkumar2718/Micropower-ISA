#include <iostream>
#include <utility> // make_pair()

#include "symbol_table.h"

int SymbolTable::table_size(const std::vector<Label>& labels)
{
  unsigned int table_size = 0;
  for (auto label: labels)
    table_size += label.element_size() * label.n_elements();

  return table_size;
}

SymbolTable::SymbolTable(const std::vector<Label>& labels)
{
  unsigned int offset = 0;

  base_address = new char[table_size(labels)];

  for (auto label: labels)
  {
    // Make an entry in symbol table
    records.push_back(Record(label.symbol, offset, label.size()));

    // Fill the values into base address
    label.fill(base_address + offset);

    // Adjust the offset
    offset += label.size();
  }
}

std::ostream& operator<<(std::ostream& os, const SymbolTable& symbol_table)
{
  if (symbol_table.empty())
    return os;

  char *base;
  os << "Label" << '\t' << "Offset" << '\t' << "Size" << std::endl;
  for (auto const& record: symbol_table.records)
  {
    base = symbol_table.base_address + record.offset;

    os << record.symbol << '\t' << record.offset << '\t' << record.size << std::endl;
    for (int i = 0; i < record.size; ++i)
      os << std::hex << int(*(base + i)) << " ";
    os << std::endl;
  }

  return os;
}

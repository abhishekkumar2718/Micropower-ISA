#include <iostream>
#include <utility> // make_pair()

#include "symbol_table.h"

void SymbolTable::initialize_data_labels(const std::vector<Label>& labels)
{
  unsigned int offset = 0;

  data_base_address = new char[table_size(labels)];

  for (auto label: labels)
  {
    // Make an entry in symbol table
    records.push_back(Record(label.symbol, offset, Section::Data, label.size()));

    // Fill the values into base address
    label.fill(data_base_address + offset);

    // Adjust the offset
    offset += label.size();
  }
}

std::ostream& operator<<(std::ostream& os, const SymbolTable& symbol_table)
{
  if (symbol_table.empty())
    return os;

  char *base;
  os << "Label" << '\t' << "Offset" << '\t' << "Section" << '\t' << "Size"
    << std::endl;

  for (auto const& record: symbol_table.records)
  {

    os << record;

    // Print hex values stored if record is a data label
    if (record.section == Section::Data)
    {
      base = symbol_table.data_base_address + record.offset;
      for (int i = 0; i < record.size; ++i)
        os << std::hex << int(*(base + i)) << " ";
      os << std::endl;
    }
  }

  return os;
}

int SymbolTable::table_size(const std::vector<Label>& labels)
{
  unsigned int table_size = 0;
  for (auto label: labels)
    table_size += label.element_size() * label.n_elements();

  return table_size;
}

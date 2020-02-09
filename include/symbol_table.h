#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include <map>

#include "label.h"
#include "record.h"

class SymbolTable
{
  private:
    std::vector<Record> records;
    char *data_base_address;
    int table_size(const std::vector<Label>&);
  public:
    ~SymbolTable(){ delete[] data_base_address; };

    void initialize_data_labels(const std::vector<Label>&);
    bool empty() const { return records.empty(); };
    friend std::ostream& operator<<(std::ostream&, const SymbolTable&);
};

#endif

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include <map>

#include "label.h"

typedef unsigned int Offset;
typedef unsigned int Size;

struct Record
{
  Symbol symbol;
  Offset offset;
  Size   size;

  Record(Symbol sym, Offset o, Size s): symbol(sym), offset(o), size(s) {}
};

class SymbolTable
{
  private:
    std::vector<Record> records;
    char *base_address;
    int table_size(const std::vector<Label>&);
  public:
    SymbolTable(const std::vector<Label>&);
    friend std::ostream& operator<<(std::ostream&, const SymbolTable&);
    bool empty() const { return records.empty(); };
};

#endif

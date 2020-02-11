/*
 Symbol table stores base address and offsets of labels to calculate
 their address.
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include <map>

#include "label.h"

class SymbolTable
{
  private:
    // TODO: Use map<Symbol, ...> instead of vector for faster search
    std::vector<Label> labels;

    char *data_segment_base;
  public:
    // Generate symbol table - Create label-offset pairs and
    // initialize variables (if given)
    SymbolTable(const std::vector<std::string>&, char*);

    bool empty() const { return labels.empty(); };
    friend std::ostream& operator<<(std::ostream&, const SymbolTable&);
};

#endif

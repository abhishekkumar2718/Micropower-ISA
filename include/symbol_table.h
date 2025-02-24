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
  public:
    // Generate symbol table - Create label-offset pairs and
    // initialize variables (if given)
    char *base;

    SymbolTable(const std::vector<std::string>&, char*);

    bool empty() const { return labels.empty(); };

    // Return the effective address of label stored
    int address (const std::string&) const;

    friend std::ostream& operator<<(std::ostream&, const SymbolTable&);
};

#endif

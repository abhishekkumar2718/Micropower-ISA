/*
 Labels are rows in the symbol table - A nice abstraction to avoid nested pairs.
 */

#ifndef LABEL_H
#define LABEL_H

#include<string>

typedef std::string Symbol;

enum class Section{Data, Text};

struct Label
{
  Symbol  symbol;
  Section section;

  // Offset from data segment base address or address base
  size_t  offset;
  // Memory occupied - 0 for all text labels
  size_t  size;

  // Constructor for data labels
  Label(const std::string&, char*, size_t);

  // Constructor for text labels
  Label(Symbol sym, size_t o): 
    symbol(sym), offset(o), size(0), section(Section::Text) {}
};
#endif

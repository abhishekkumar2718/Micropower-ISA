/*
 Record are rows in the symbol table - A nice abstraction to avoid nested pairs.
 */

#ifndef RECORD_H
#define RECORD_H

#include<string>

typedef std::string  Symbol;
typedef unsigned int Offset;
typedef unsigned int Size;

enum class Section{Data, Text};

struct Record
{
  Symbol  symbol;
  Offset  offset;
  Size    size;
  Section section;

  // Text labels have no concept of size, use 0 as default
  Record(Symbol sym, Offset o, Section section, Size s = 0):
    symbol(sym), offset(o), size(s), section(section) {}

  friend std::ostream& operator<<(std::ostream&, const Record&);
};
#endif

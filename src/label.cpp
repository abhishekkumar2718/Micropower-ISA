#include <iostream>
#include <algorithm>

#include "label.h"

// Format:
// symbol: .data_type [initial_expression]
Label::Label(const std::string line)
{
  // First non whitespace character
  int fchar = -1;

  for(int i = 0; i < line.size(); ++i){
    if(line[i] != ' ' && fchar == -1)
      fchar = i;

    // Copy characters from first non whitespace character upto colon.
    if(line[i] == ':')
      symbol = line.substr(fchar, i - fchar);

    if(line[i] == '.'){
      // Skip the dot
      ++i;

      int j;
      for(j = i; j < line.size() && line[j] != ' '; ++j);

      // Copy characters after dot upto whitespace character
      data_type = line.substr(i, j - i);

      if(!valid_data_type())
        throw std::domain_error("Invalid data_type " 
            + data_type + " for label " + symbol);

      // If there is an expression after datatype, copy it
      if(j < line.size()){
        // Skip the space
        ++j;

        if(data_type == "asciiz")
          // Skip the quotes
          expr = line.substr(j + 1, line.size() - j - 2);
        else
          expr = line.substr(j, line.size() - j);
      }
      break;
    }
  }
};

std::ostream& operator<<(std::ostream& os, const Label& label)
{
  os << label.symbol << ", " << label.data_type << ", " << label.expr << ", "
    << label.size();
  return os;
}

unsigned int Label::element_size() const
{
  if (data_type == "byte" || data_type == "asciiz")
    return 1;
  else if (data_type == "half_word")
    return 2;
  else if (data_type == "word")
    return 4;

  return 0;
}

unsigned int Label::n_elements() const
{
  if (data_type == "byte")
    return 1;
  else if (data_type == "word")
  {
    // Integer arrays are of form:
    // a, b, c, ... -> Number of elements = Comma Count + 1
    return std::count(expr.begin(), expr.end(), ',') + 1;
  }
  else if (data_type == "asciiz")
    return expr.size();

  return 0;
}

unsigned int Label::size() const
{
  return element_size() * n_elements();
}

bool Label::valid_data_type() const
{
  return data_type == "byte" || data_type == "half_word" || data_type == "word"
    || data_type == "asciiz";
}

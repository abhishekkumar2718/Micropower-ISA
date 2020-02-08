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
    return expr.size() + 1;

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

void Label::fill(char* base_address) const
{
  // For empty expressions
  if (!expr.size())
  {
    // Fill the memory with zeroes
    std::fill(base_address, base_address + size(), '\0');
    return ;
  }

  if (data_type == "byte" || data_type == "asciiz")
  {
    // Copy characters from expression
    for (int i = 0; i < expr.size(); ++i)
      base_address[i] = expr[i];

    // ASCII strings are null terminated
    if (data_type == "asciiz")
      base_address[expr.size()] = '\0';
  }
  else if (data_type == "word")
  {
    // Typecast as integer for easy conversion
    int *i_base = (int *) base_address;
    bool neg = false;
    unsigned int offset = 0;
    int sum = 0;

    // Add a comma at the end as sentinel value
    auto expression = expr + ",";

    for (int i = 0; i < expression.size(); ++i)
    {
      // Skip whitespaces
      if (expression[i] == ' ')
        continue;

      sum = 0;
      neg = false;

      // If first character is a minus sign
      if (expression[i] == '-')
      {
        neg = true;
        ++i;
      }

      // Building the number
      while(expression[i] != ' ' && expression[i] != ',')
        sum = sum * 10 + (expression[i++] - '0');

      i_base[offset++] = neg ? -1 * sum : sum;
    }
  }
}

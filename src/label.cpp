#include <iostream>

#include "label.h"

size_t data_type_size(const std::string&);

// Constructor for data labels
Label::Label(const std::string &line, char* data_segment_base, size_t offset)
  : offset(offset), section(Section::Data)
{
  // symbol - Copy upto colon
  auto colon_idx = line.find(':', 0);
  symbol = line.substr(0, colon_idx);

  // data_type - Copy after dot upto first whitespace
  auto dot_idx = line.find('.', 0);

  size_t whitespace_idx = dot_idx + 1;
  for (; whitespace_idx < line.size(); ++whitespace_idx)
  {
    if (std::isspace(line[whitespace_idx]))
        break;
  }
  auto data_type = line.substr(dot_idx + 1, whitespace_idx - 1 - dot_idx);
  size = data_type_size(data_type);

  if (whitespace_idx == line.size())
    return ;

  // Initialize values of expression

  // Move to the first character of expression
  size_t expr_idx = whitespace_idx;
  for (; expr_idx < line.size(); ++expr_idx)
  {
    if (!std::isspace(line[expr_idx]))
      break;
  }

  if (data_type == "byte")
    data_segment_base[0] = line[expr_idx];
  if (data_type == "asciiz")
  {
    // Skip quotes
    ++expr_idx;
    auto expr = line.substr(expr_idx, line.size() - expr_idx);

    size *= expr.size();

    for (size_t i = 0; i < expr.size(); ++i)
      data_segment_base[i] = line[expr_idx + i];

    // ASCII strings are null terminated
    data_segment_base[expr.size()] = '\0';
  }
  else if (data_type == "word")
  {
    // Typecast for assigning integer values
    int* base = (int *) data_segment_base;

    // Add comma at the end of the expression as sentinel
    auto expr = line.substr(expr_idx) + ",";

    bool neg = false;
    int sum = 0;
    size_t element_count = 0;

    for (size_t i = 0; i < expr.size(); ++i)
    {
      // Skip whitespaces
      if (std::isspace(expr[i]))
          continue;

      sum = 0;
      // Is a negative number
      if (expr[i] == '-')
      {
        neg = true;
        ++i;
      }

      // While reading digits
      while (!std::isspace(expr[i]) && expr[i] != ',')
        sum = sum * 10 + (expr[i++] - '0');

      base[element_count++] = neg ? -1 * sum : sum;
    }

    size *= element_count;
  }
}

// Memory occupied by each element of the data type
size_t data_type_size(const std::string& data_type)
{
  if (data_type == "byte" || data_type == "asciiz")
    return 1;
  else if (data_type == "word")
    return 4;
  else
    throw "Invalid data type " + data_type;

  return 0;
}

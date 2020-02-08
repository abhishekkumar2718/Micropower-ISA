#include <iostream>
#include <vector>

#include "processing.h"
#include "label.h"

bool is_comment(std::string);
bool is_label(std::string);
std::vector<std::string> tokenize(std::string);

void process_lines(const std::vector<std::string> lines,
                   std::vector<Label> &labels) {
  for(auto line: lines)
  {
    // Skip empty lines and comments
    if(!line.size() || is_comment(line))
      continue;

    if(is_label(line))
      labels.push_back(Label(line));
  }
}

// Check if given line is a comment
// Format -
// # (*)
bool is_comment(const std::string line)
{
  return line[0] == '#';
}

// Check if given line is a label
// Format - 
// symbol: .datatype (*)
bool is_label(const std::string line)
{
  for(int i = 1; i < line.size() - 2; ++i)
  {
    if(line[i] == ':' && line[i + 2] == '.')
      return true;
  }
  return false;
}

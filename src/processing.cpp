#include <algorithm>
#include <fstream>

#include "processing.h"

// Read and preprocess source file
// Return a vector containing labels, directives and instructions
std::vector<std::string> read_file(const std::string &file_name)
{
  std::ifstream fs(file_name);

  if (!fs)
    throw std::string("Unable to open source file!");

  // TODO: Add more validations for source file

  std::vector<std::string> lines;
  std::string line;

  while (getline(fs, line))
  {
    // Skip comments and empty lines
    if (is_comment(line) || !line.size())
      continue;

    // Trim left whitespaces
    line.erase(
        line.begin(), 
        std::find_if(
          line.begin(), line.end(), [](int ch){ return !std::isspace(ch); }
        ));

    // Trim right whitespaces
    line.erase(
        std::find_if(
          line.rbegin(), line.rend(), [](int ch){ return !std::isspace(ch); }
        ).base(), line.end());

    if (line.size())
      lines.push_back(line);
  }

  return lines;
}

// TODO: Support multi line initialisation
// Check if given line is a data label
// Format - symbol: .data_type [initialisation_expression]
bool is_data_label(const std::string &line)
{
  auto idx = line.find(':', 0);
  return idx != std::string::npos && idx != line.size() - 1;
}

// Check if given line is a text label
// Format - symbol:
bool is_text_label(const std::string &line)
{
  return line.find(':', 0) == line.size() - 1;
}

// Check if given line is an assembly directive
// Format - .directive [parameters]
bool is_assembly_directive(const std::string &line)
{
  return line.find(':', 0) == std::string::npos
    && line.find('.', 0) != std::string::npos;
}

// Check if given line is an assembly directive
// Format - # Comment text
bool is_comment(const std::string &line){ return line.find('#', 0) != std::string::npos; };

#include <vector>
#include <algorithm>

#include "processing.h"

// Preprocess lines to remove comments and trim whitespace
void preprocess_lines(std::vector<std::string> &lines)
{
  // Remove comments
  std::remove_if(lines.begin(), lines.end(), is_comment);

  // Trim whitespace
  for (auto& line: lines)
  {
    // Left Trim
    line.erase(
        line.begin(), 
        std::find_if(
          line.begin(), line.end(), [](int ch){ return !std::isspace(ch); }
        ));

    // Right trim
    line.erase(
        std::find_if(
          line.rbegin(), line.rend(), [](int ch){ return !std::isspace(ch); }
        ).base(), line.end());
  }

  // Remove empty lines
  auto iter = std::remove_if(lines.begin(), lines.end(),
      [](const std::string &line){ return !line.size(); }
      );

  // Erase lines from the container
  lines.erase(iter, lines.end());
}

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

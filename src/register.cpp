#include "register.h"

size_t register_index(const std::string &label)
{
  return std::find(register_labels.begin(), register_labels.end(), label)
    - register_labels.begin();
}

RegisterTable initialize_registers()
// Initialize all register values to zero
{
  return std::vector<int>(register_labels.size(), 0);
}

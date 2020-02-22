/*
 * Helper module to keep register related information together.
 */

#ifndef REGISTER_H
#define REGISTER_H

#include <algorithm>
#include <vector>

typedef std::vector<int> RegisterTable;

// TODO: Add support for r0, r1, ..., r32 format
const std::vector<std::string> register_labels = {
  "zero",
  "at",
  "v0", "v1",
  "a0", "a1", "a2", "a3",
  "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
  "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
  "t8", "t9",
  "k0", "k1",
  "gp",
  "sp",
  "fp",
  "ra",
  "high", "low"
};

size_t register_index(const std::string &);

const size_t gp = register_index("gp");
const size_t sp = register_index("sp");

RegisterTable initialize_registers();

#endif

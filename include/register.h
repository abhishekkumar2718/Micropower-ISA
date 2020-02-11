#ifndef REGISTER_H
#define REGISTER

// Symbolic names associated with each register number
enum RegisterLabel
{
  zero,
  v0, v1,
  a0, a1, a2, a3,
  t0, t1, t2, t3, t4, t5, t6, t7,
  s0, s1, s2, s3, s4, s5, s6, s7,
  t8, t9,
  gp,
  sp,
  fp,
  ra,
  high, low
};

// 32 GPRs + HI, LO
const size_t register_count = 32 + 2;

std::vector<int> initialize_registers()
{ 
  // Initialize all register values to zero
  return std::vector<int>(register_count, 0);
};

#endif

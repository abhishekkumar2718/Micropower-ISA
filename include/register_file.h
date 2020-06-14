/*
 *
 */

#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include <vector>
#include <string>

#include "memory.h"

class RegisterFile
{
  public:
    // 64 bit GPRs
    std::vector<long long> GPR;

    // 64 bit Link Register
    size_t LR;

    // 64 bit Condition Register
    // Treated internally as 32 bit by using upper half
    long long CR;

    // 64 bit Special Register R0
    long long SRR0;

    // 64 bit Current Instruction Address
    size_t CIA;

    // 64 bit Next Instruction Address
    size_t NIA;

    RegisterFile() :
      GPR(std::vector<long long> (32, 0)),
      CIA(text_segment_base), NIA(text_segment_base + 4) {};

    friend std::ostream& operator<<(std::ostream&, const RegisterFile&);
};

#endif

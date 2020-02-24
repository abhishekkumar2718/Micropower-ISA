/*
 *
 *
 */

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "symbol_table.h"

class Instruction
{
  private:
    std::string mnemonic;

    // TODO: Look into bit packing and optimize memory use
    int rs, rt, rd, sh_amt, imm, address;
  public:
    Instruction(const std::string&, const SymbolTable&);

    // Return 32-bit representation of the instruction
    int encode() const;

    char type() const;

    friend std::ostream& operator<<(std::ostream&, const Instruction&);
};

std::vector<Instruction> translate_instructions(const std::vector<std::string>&,
    const SymbolTable&);

#endif

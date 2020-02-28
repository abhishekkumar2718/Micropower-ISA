/*
 *
 *
 */

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "symbol_table.h"
#include "register_file.h"

class Instruction
{
  private:
    std::string mnemonic;

    // TODO: Look into bit packing and optimize memory use
    long long int RA, RB, RS, RT, SI, BO, BI, BD, AA, LK, RC, OE, LI, BH;
  public:
    Instruction(const std::string&, const SymbolTable&);

    // Return 32-bit representation of the instruction
    int encode() const;

    std::string type() const;

    void execute(RegisterFile&);

    friend std::ostream& operator<<(std::ostream&, const Instruction&);
};

std::vector<Instruction> translate_instructions(const std::vector<std::string>&,
    const SymbolTable&);

#endif

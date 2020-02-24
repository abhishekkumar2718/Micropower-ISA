#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <cstring>

#include "instruction.h"
#include "processing.h"

// Position of starting bits in 32-bit representation
const int opcode_position = 32 - 6;
const int rs_position = opcode_position - 5;
const int rt_position = rs_position - 5;
const int rd_position = rt_position - 5;
const int sh_amt_position = rd_position - 5;
const int funct_position = sh_amt_position - 5;
const int imm_position = 0;
const int address_position = 0;

// Map mnemonics to their opcodes
const std::map<std::string, int> OpcodeMapping = {
  {"addi"   , 0b001000},
  {"beq"    , 0b000100},
  {"bne"    , 0b000101},
  {"bgtz"   , 0b000111},
  {"bltz"   , 0b000001},
  {"j"      , 0b000010},
  {"jal"    , 0b000011},
  {"lb"     , 0b100000},
  {"lw"     , 0b100011},
  {"sb"     , 0b101000},
  {"sw"     , 0b101011}
};

// Map mnemonics to their funct codes
const std::map<std::string, int> FunctMapping = {
  {"add"    , 0b100000},
  {"div"    , 0b011010},
  {"jr"     , 0b001000},
  {"mflo"   , 0b010010},
  {"mult"   , 0b011000},
  {"sub"    , 0b100010},
  {"syscall", 0b001100}
};

void tokenize(std::string, const SymbolTable&, std::string&, std::vector<int>&);

Instruction::Instruction(const std::string &line, const SymbolTable &symbol_table)
  : rs(0), rt(0), rd(0), sh_amt(0), imm(0), address(0)
{
  std::vector<int>tokens;
  tokenize(line, symbol_table, mnemonic, tokens);

  if (mnemonic == "syscall")
    return ;

  if (type() == 'R')
  {
    if (mnemonic == "jr" || mnemonic == "mthi" || mnemonic == "mtlo")
      rs = tokens[0];
    else if (mnemonic == "mflo" || mnemonic == "mfhi")
      rd = tokens[0];
    else if (mnemonic == "mult" || mnemonic == "mutlu" || mnemonic == "div"
        || mnemonic == "divu")
    {
      rs = tokens[0];
      rd = tokens[1];
    }
    else if (mnemonic == "sll" || mnemonic == "srl" || mnemonic == "sra")
    {
      rd = tokens[0];
      rt = tokens[1];
      sh_amt = tokens[2];
    }
    else
    {
      rd = tokens[0];
      rs = tokens[1];
      rt = tokens[2];
    }
  }
  else if (type() == 'I')
  {
    if (mnemonic == "blez" || mnemonic == "bgtz" || mnemonic == "lui")
    {
      rs = tokens[0];
      imm = tokens[1];
    }
    else if (mnemonic == "beq" || mnemonic == "bne")
    {
      rs = tokens[0];
      rt = tokens[1];
      imm = tokens[2];
    }
    else
    {
      rt = tokens[0];
      rs = tokens[1];
      imm = tokens[2];
    }
  }
  else if (type() == 'J')
    address = tokens[0];
  // Translate psuedo instructions
  else if (mnemonic == "li")
  {
    mnemonic = "addi";
    rs = 0;
    rt = tokens[0];
    imm = tokens[1];
  }
  else if (mnemonic == "la")
  {
    mnemonic = "addi";
    rs = 0;
    rt = tokens[0];
    imm = 0;
  }
  else if (mnemonic == "move")
  {
    mnemonic = "add";
    rd = tokens[0];
    rs = tokens[1];
    rt = 0;
  }
}

char Instruction::type() const
{
  if (mnemonic == "j" || mnemonic == "jal")
    return 'J';
  else if (OpcodeMapping.count(mnemonic))
    return 'I';
  else if (FunctMapping.count(mnemonic))
    return 'R';

  return '\0';
}

int Instruction::encode() const
{
  int result = 0;

  // R format instructions have opcode 0
  if (type() != 'R')
    result = OpcodeMapping.at(mnemonic) << opcode_position;

  if (type() == 'J')
    result = result | address << address_position;
  else if (type() == 'I')
  {
    result = result | rs << rs_position | rt << rt_position |
      (imm & (1 << 17) - 1);
  }
  else if (type() == 'R')
  {
    result = result | rs << rs_position | rt << rt_position | 
      sh_amt << sh_amt_position | FunctMapping.at(mnemonic) << funct_position;
  }

  return result;
}

// Break down an instruction line into mnemonic, and integer tokens like rs, rt
void tokenize(std::string line, const SymbolTable &symbol_table,
    std::string &mnemonic, std::vector<int> &tokens)
{
  // Read upto first whitespace character for mnemnonic
  size_t mnemonic_end = 0;
  for (mnemonic_end = 0; mnemonic_end < line.size(); ++mnemonic_end)
  {
    if (std::isspace(line[mnemonic_end]))
        break;
  }

  mnemonic = line.substr(0, mnemonic_end);

  // Return early if line just contains a mnemonic
  if (mnemonic_end == line.size())
    return ;

  // Break down the string into words
  line.append(",");

  for (size_t i = mnemonic_end + 1; i < line.size(); ++i)
  {
    if (std::isspace(line[i]))
        continue;

    // Find index of next comma
    auto comma_idx = line.find(',', i);

    // Slice upto the next comma
    auto word = line.substr(i, comma_idx - i);

    if (word.find('R') != std::string::npos)
      // If it is a register
      tokens.push_back(stoi(word.substr(1)));
    else if (word.find('(') != std::string::npos)
    {
      // If it is an indirect address
      auto left_idx = word.find('(');
      auto right_idx = word.find(')');

      auto base = word.substr(left_idx + 1, right_idx - left_idx - 1);
      auto imm = word.substr(0, left_idx);

      std::cout<<"Base: "<<base<<" Imm: "<<imm<<std::endl;

      tokens.push_back(stoi(base));

      tokens.push_back(stoi(imm));
    }
    else if (isalpha(word[0]))
      // If word is a label
      tokens.push_back(symbol_table.address(word));
    else if (word[0] == '-')
      tokens.push_back(-1 * stoi(word.substr(1)));
    else
      tokens.push_back(stoi(word));

    i = comma_idx;
  }
}

std::vector<Instruction> translate_instructions(
    const std::vector<std::string> &lines,
    const SymbolTable &symbol_table)
{
  std::vector<Instruction> instructions;

  for (const auto &l: lines)
  {
    if (is_data_label(l) || is_text_label(l) || is_assembly_directive(l))
      continue;

    instructions.push_back(Instruction(l, symbol_table));
  }

  return instructions;
}

std::ostream& operator<<(std::ostream &os, const Instruction &i)
{
  os << i.mnemonic << " ";

  if (i.type() == 'J')
    os << "Address: 0x" << std::hex << i.address << std::dec;
  else if (i.type() == 'I')
    os << "RS: " << i.rs << " " << "RT: " << i.rt << " "<< "Immediate: " << i.imm;
  else
  {
    os << "RS: " << i.rs << " " << "RT: " << i.rt << " " << "RD: "<< i.rd
      << " " << "Shift Amount: " << i.sh_amt << " " << "Funct: " << std::hex
      << FunctMapping.at(i.mnemonic) << std::dec;
  }

  return os;
}

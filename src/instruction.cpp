#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <cstring>

#include "instruction.h"
#include "processing.h"

// Position of first bit of each field to calculate left shifts needed.
const int po_position = 32 - 6;
const int rs_position = po_position - 5;
const int ra_position = rs_position - 5;
const int rb_position = ra_position - 5;
const int rc_position = 0;

const int rt_position = po_position - 5;
const int oe_position = rb_position - 1;
const int xo_position = oe_position - 9;

const int si_position = 0;

const int bo_position = po_position - 5;
const int bi_position = bo_position - 5;
const int bd_position = bi_position - 14;
const int aa_position = bd_position - 1;
const int lk_position = 0;

const int li_position = po_position - 24;

const int bh_position = 21;

const std::map<std::string, std::string> Formats = {
  // X Instructions
  {"and"  , "X"},
  {"cmp"  , "X"},
  {"extsw", "X"},
  {"nand" , "X"},
  {"or"   , "X"},
  {"sld"  , "X"},
  {"srd"  , "X"},
  {"srad" , "X"},
  {"xor"  , "X"},

  // XO Instructions
  {"add" , "XO"},
  {"subf", "XO"},
  {"divw", "XO"},

  // D Instructions
  {"addi", "D"},
  {"andi", "D"},
  {"lbz" , "D"},
  {"lwz" , "D"},
  {"ori" , "D"},
  {"stb" , "D"},
  {"stw" , "D"},
  {"xori", "D"},

  // B Instructions
  {"bc" , "B"},
  {"bca", "B"},

  // I Instructions
  {"b", "I"},
  {"ba", "I"},
  {"bl", "I"},

  // SC Instructions
  {"sc", "SC"},

  // XL Instructions
  {"bclr", "XL"}
};

const std::map<std::string, int> PrimaryOpcodes = {
  // X Instructions
  {"and"  , 31},
  {"cmp"  , 31},
  {"extsw", 31},
  {"nand" , 31},
  {"or"   , 31},
  {"sld"  , 31},
  {"srd"  , 31},
  {"xor"  , 31},

  // XO Instructions
  {"add" , 31},
  {"subf", 31},
  {"divw", 31},

  // D Instructions
  {"addi", 14},
  {"andi", 28},
  {"lbz" , 34},
  {"lwz" , 32},
  {"ori" , 24},
  {"stb" , 38},
  {"stw" , 36},
  {"xori", 26},

  // B Instructions
  {"bc" , 19},
  {"bca", 19},

  // I Instructions
  {"b", 18},
  {"ba", 18},
  {"bl", 18},

  // SC Instructions
  {"sc", 17},

  // XL Instructions
  {"bclr", 19}
};

const std::map<std::string, int> ExtendedOpcodes = {
  // X Instructions
  {"and"  , 28},
  {"cmp"  , 0},
  {"extsw", 986},
  {"nand" , 476},
  {"or"   , 444},
  {"sld"  , 27},
  {"srd"  , 539},
  {"xor"  , 316},

  // XO Instructions
  {"add" , 266},
  {"subf", 40},
  {"divw", 491}
};

void tokenize(std::string, const SymbolTable&, std::string&, std::vector<int>&);

Instruction::Instruction(const std::string &line, const SymbolTable &symbol_table)
  : RA(0), RB(0), RS(0), RT(0), SI(0), BO(0), BI(0), AA(0), LK(1), RC(0), OE(0),
    BH(0)
{
  std::vector<int>tokens;
  tokenize(line, symbol_table, mnemonic, tokens);

  // Translate pseudo instructions
  if (mnemonic == "li")
  {
    mnemonic = "addi";
    RT = tokens[0];
    // HACK: Use R1 as $zero
    // Change translation from line by line to whole sections
    RA = 0x00001;
    SI = tokens[1];
  }
  else if (mnemonic == "la")
  {
    mnemonic = "addi";
    RT = tokens[0];
    // HACK: Same as li
    RA = 0x00001;
    SI = tokens[1];
  }
  else if (type() == "X")
  {
    if (mnemonic == "cmp")
    {
      RA = tokens[0];
      RB = tokens[1];
    }
    else if (mnemonic == "extsw")
    {
      RA = tokens[0];
      RS = tokens[1];
    }
    else
    {
      RA = tokens[0];
      RS = tokens[1];
      RB = tokens[2];
    }
  }
  else if (type() == "XO")
  {
    RT = tokens[0];
    RA = tokens[1];
    RB = tokens[2];
  }
  else if (type() == "D")
  {
    if (mnemonic == "stw" || mnemonic == "stb")
    {
      RS = tokens[0];
      RT = tokens[1];
    }
    else
    {
      RT = tokens[0];
      RA = tokens[1];
    }

    SI = tokens[2];
  }
  else if (type() == "B")
  {
    BO = tokens[0];
    BI = tokens[1];
    BD = tokens[2];
    AA = (mnemonic == "bca");
  }
  else if (type() == "I")
  {
    LI = tokens[0];
    AA = (mnemonic == "ba");
    LK = (mnemonic == "bl");
  }
  else if (type() == "XL")
    LK = tokens.size() != 0;
}

int Instruction::encode() const
{
  int result = 0;

  if (type() == "X")
  {
    // Record bit is zero
    result = PrimaryOpcodes.at(mnemonic) << po_position
      | RS << rs_position
      | RA << ra_position
      | RB << rb_position
      | ExtendedOpcodes.at(mnemonic) << xo_position
      | RC << rc_position;
  }
  else if (type() == "XO")
  {
    // Overflow exception and record bits are zero
    result = PrimaryOpcodes.at(mnemonic) << po_position
      | RT << rt_position
      | RA << ra_position
      | RB << rb_position
      | OE << oe_position
      | ExtendedOpcodes.at(mnemonic) << xo_position
      | RC << rc_position;
  }
  else if (type() == "D")
  {
    result = PrimaryOpcodes.at(mnemonic) << po_position
      | RT << rt_position
      | RA << ra_position
      | SI << si_position;
  }
  else if (type() == "B")
  {
    result = PrimaryOpcodes.at(mnemonic) << po_position
      | BO << bo_position
      | BI << bi_position
      | BD << bd_position
      | AA << aa_position
      | LK << lk_position;
  }
  else if (type() == "I")
  {
    result = PrimaryOpcodes.at(mnemonic) << po_position
      | LI << li_position
      | AA << aa_position
      | LK << lk_position;
  }
  else if (type() == "SC")
  {
    // TODO: Add LEV code for system calls
    result = PrimaryOpcodes.at(mnemonic) << po_position;
  }
  else if (type() == "XL")
  {
    result = PrimaryOpcodes.at(mnemonic) << po_position
      | BO << bo_position
      | BI << bi_position
      | BH << bh_position
      | LK << lk_position;
  }

  return result;
}

std::ostream& operator<<(std::ostream &os, const RegisterFile &rf)
{
    os << "GPR:" << std::endl;
    for(int i = 0; i < 32; i++){
        os << "GPR[" << i+1 <<"] = "<<rf.GPR[i]<<std::endl;
    }
    os << "LR = " << rf.LR << std::endl;
    os << "CR = " << rf.CR << std::endl;    
    os << "SRR0 = " << rf.SRR0 << std::endl;    
    os << "CIA = " << rf.CIA << std::endl;
    os << "NIA = " << rf.NIA << std::endl;

    return os;
}

void Instruction::execute(RegisterFile &rf)
{
  rf.NIA = rf.CIA + 4;
  // TODO: Implement X, D, B, L, I Instructions

  // X Instructions
  if (mnemonic == "and")
    rf.GPR[RA] = rf.GPR[RS] & rf.GPR[RB];
  else if (mnemonic == "cmp") {
    if (rf.GPR[RA] < rf.GPR[RB])
       rf.CR = (1LL << 63);
    else if (rf.GPR[RA] > rf.GPR[RB])
        rf.CR = (1LL << 62);
    else 
        rf.CR = (1LL << 61);
  }
  else if (mnemonic == "extsw") {
    long long upper32 = (rf.GPR[RS] >> 32);
    long long RS32 = (upper32 % 2);
    rf.GPR[RA] = (upper32 << 32);
    if(RS32)
        rf.GPR[RA] |= -1;
  }
  else if (mnemonic == "nand")
    rf.GPR[RA] = ~(rf.GPR[RS] & rf.GPR[RB]);
  else if (mnemonic == "or")
    rf.GPR[RA] = rf.GPR[RS] | rf.GPR[RB];
  else if (mnemonic == "sld") { 
    // Yet to be figured out.
  }
  else if (mnemonic == "srd") {
    // Yet to be figured out. 
  }
  else if (mnemonic == "xor") {
      rf.GPR[RA] = rf.GPR[RS] ^ rf.GPR[RB];
  }
  // XO Instructions
  else if (mnemonic == "add")
    rf.GPR[RT] = rf.GPR[RA] + rf.GPR[RB];
  else if (mnemonic == "subf")
    rf.GPR[RT] = rf.GPR[RB] - rf.GPR[RA];
  else if (mnemonic == "divw"){
    rf.GPR[RT] = rf.GPR[RB] / rf.GPR[RA];
  }
  // D Instructions
  else if (mnemonic == "addi")
    rf.GPR[RT] = rf.GPR[RA] + rf.GPR[SI];
  else if (mnemonic == "andi")
    rf.GPR[RT] = rf.GPR[RA] & rf.GPR[SI];
  else if (mnemonic == "lbz") {

  }
  else if (mnemonic == "lwz") {

  }
  else if (mnemonic == "ori")
    rf.GPR[RT] = rf.GPR[RA] | rf.GPR[SI];
  else if (mnemonic == "stb") {

  }
  else if (mnemonic == "stw") {

  } 
  else if (mnemonic == "xori")
    rf.GPR[RT] = rf.GPR[RA] ^ rf.GPR[SI];
  // B Instructions
  else if (mnemonic == "bc")
    rf.NIA = rf.CIA + (BD << 2);
  else if (mnemonic == "bca")
    rf.NIA = BD << 2;
  // I Instructions
  else if (mnemonic == "b") {
    rf.GPR[AA] = 0;
    rf.NIA = (rf.GPR[LI] << 2) +rf.CIA;
    if (rf.GPR[LK]) 
      rf.LR = rf.CIA + 4;
  }
  else if (mnemonic == "ba") {
    rf.GPR[AA] = 1;
    rf.NIA = (rf.GPR[LI] << 2);
    if (rf.GPR[LK])
      rf.LR = rf.CIA + 4;
  }
  else if (mnemonic == "bl") {
      rf.GPR[AA] = 0;
      rf.GPR[LK] = 1;
      rf.NIA = rf.CIA + (rf.GPR[LI]<<2);
  }
  // SC Instructions
  else if (mnemonic == "sc"){
    rf.SRR0 = rf.CIA + 4;
    rf.NIA = 0;
    switch(rf.GPR[0]){
        case 1: std::cout << rf.GPR[3] << std::endl;
        break;
        case 4: // Memory access required for printing the string
        break;
        case 5: std::cin >>  rf.GPR[3];
        break;
        case 8: // Memory access required for reading the string
        break;
        default:
        break;
    }
  }
  // XL Instructions
  else if (mnemonic == "bclr"){
    rf.GPR[BH] = 0;
    BO = 0x10100;
    rf.NIA = rf.LR<<2;
  }
  /* std::cout<<rf; */
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

  if (i.type() == "X")
  {
    os << i.RS <<", "<< i.RA << ", " << i.RB << ", "
      << ExtendedOpcodes.at(i.mnemonic) << ", " << i.RC;
  }
  else if (i.type() == "XO")
  {
    os << i.RT <<", "<< i.RA << ", " << i.RB << ", " << i.OE << ", "
      << ExtendedOpcodes.at(i.mnemonic) << ", " << i.RC;
  }
  else if (i.type() == "D")
    os << i.RT << ", " << i.RA << ", " << i.SI;
  else if (i.type() == "B")
    os << i.BO << ", " << i.BI << ", " << i.BD << ", " << i.AA << ", " << i.LK;
  else if (i.type() == "I")
    os << i.LI << ", " <<i.AA << ", " << i.LK;

  return os;
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

    if (word[0] == 'R')
      // If it is a register
      tokens.push_back(stoi(word.substr(1)));
    else if (word.find('(') != std::string::npos)
    {
      // If it is an indirect address
      auto left_idx = word.find('(');
      auto right_idx = word.find(')');

      auto base = word.substr(left_idx + 1, right_idx - left_idx - 1);
      auto imm = word.substr(0, left_idx);

      tokens.push_back(stoi(base.substr(1)));
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

std::string Instruction::type() const {
  return Formats.at(mnemonic);
};

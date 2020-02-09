#include <ostream>
#include "record.h"

std::ostream& operator<<(std::ostream& os, const Record& record)
{
  os << record.symbol << '\t' << record.offset << '\t';
  if (record.section == Section::Data)
    os << "Data" << '\t' << record.size;
  else if (record.section == Section::Text)
    os << "Text";

  os << std::endl;

  return os;
}

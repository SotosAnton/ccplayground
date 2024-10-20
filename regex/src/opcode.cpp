#include <regex/opcode.hpp>

#define RESET "\033[m"
#define MAGENTA "\033[35m" /* Magenta */

namespace regex {
// print operator for ReTree content
std::ostream &operator<<(std::ostream &out, const OpCode &value) {

  if (value < 0x10000) {
    out << static_cast<char>(value);
  } else {
    out << MAGENTA;
    switch (value) {
    case OpCode::ROOT:
      out << "@";
      break;
    case OpCode::FINAL:
      out << "^";
      break;
    case OpCode::ANY_CHAR:
      out << ".";
      break;
    case OpCode::BRACKET:
      out << "[]";
      break;
    case OpCode::INV_BRACKET:
      out << "[^]";
      break;
    case OpCode::KLEENE_STAR:
      out << "*";
      break;
    case OpCode::AT_START:
      out << "^";
      break;
    case OpCode::AT_END:
      out << "$";
      break;
    case OpCode::RANGE:
      out << "-";
      break;
    case OpCode::SUBEXPRESSION:
      out << "()";
      break;
    }
    out << RESET;
  }
  return out;
}

std::ostream &printContent(std::ostream &output, const uint32_t &value) {
  if (value < 0x10000) {
    output << static_cast<char>(value);
  } else {
    output << static_cast<regex::OpCode>(value);
  }
  return output;
}

} // namespace regex
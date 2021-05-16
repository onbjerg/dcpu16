#include "Assembler.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>

bool token_lexeme_eq(const Token *token, const std::string &rhs) {
  auto lexeme = token->lexeme();
  return std::equal(
      lexeme.begin(), lexeme.end(), rhs.begin(), rhs.end(),
      [](char a, char b) { return std::tolower(a) == std::tolower(b); });
}

Word token_value(const Token *token) {
  // TODO(onbjerg): Strings
  // TODO(onbjerg): Labels
  switch (token->kind()) {
  case Token::Kind::Number: {
    auto lexeme = std::string(token->lexeme());
    if (lexeme.rfind("0x", 0) == 0) {
      return std::stoul(lexeme.substr(2), nullptr, 16);
    } else {
      return std::stoul(lexeme);
    }
  }
  default:
    break;
  }
}

Word token_opcode(const Token *token) {
  if (token_lexeme_eq(token, "set")) {
    return 0x01;
  } else if (token_lexeme_eq(token, "add")) {
    return 0x02;
  } else if (token_lexeme_eq(token, "sub")) {
    return 0x03;
  } else if (token_lexeme_eq(token, "mul")) {
    return 0x04;
  } else if (token_lexeme_eq(token, "mli")) {
    return 0x05;
  } else if (token_lexeme_eq(token, "div")) {
    return 0x06;
  } else if (token_lexeme_eq(token, "dvi")) {
    return 0x07;
  } else if (token_lexeme_eq(token, "mod")) {
    return 0x08;
  } else if (token_lexeme_eq(token, "mdi")) {
    return 0x09;
  } else if (token_lexeme_eq(token, "and")) {
    return 0x0a;
  } else if (token_lexeme_eq(token, "bor")) {
    return 0x0b;
  } else if (token_lexeme_eq(token, "xor")) {
    return 0x0c;
  } else if (token_lexeme_eq(token, "shr")) {
    return 0x0d;
  } else if (token_lexeme_eq(token, "asr")) {
    return 0x0e;
  } else if (token_lexeme_eq(token, "shl")) {
    return 0x0f;
  } else if (token_lexeme_eq(token, "ifb")) {
    return 0x10;
  } else if (token_lexeme_eq(token, "ifc")) {
    return 0x11;
  } else if (token_lexeme_eq(token, "ife")) {
    return 0x12;
  } else if (token_lexeme_eq(token, "ifn")) {
    return 0x13;
  } else if (token_lexeme_eq(token, "ifg")) {
    return 0x14;
  } else if (token_lexeme_eq(token, "ifa")) {
    return 0x15;
  } else if (token_lexeme_eq(token, "ifl")) {
    return 0x16;
  } else if (token_lexeme_eq(token, "ifu")) {
    return 0x17;
  } else if (token_lexeme_eq(token, "adx")) {
    return 0x1a;
  } else if (token_lexeme_eq(token, "sbx")) {
    return 0x1b;
  } else if (token_lexeme_eq(token, "sti")) {
    return 0x1e;
  } else if (token_lexeme_eq(token, "std")) {
    return 0x1f;
  } else if (token_lexeme_eq(token, "jsr")) {
    return 0x01 << 5;
  } else if (token_lexeme_eq(token, "int")) {
    return 0x08 << 5;
  } else if (token_lexeme_eq(token, "iag")) {
    return 0x09 << 5;
  } else if (token_lexeme_eq(token, "ias")) {
    return 0x0a << 5;
  } else if (token_lexeme_eq(token, "rfi")) {
    return 0x0b << 5;
  } else if (token_lexeme_eq(token, "iaq")) {
    return 0x0c << 5;
  } else if (token_lexeme_eq(token, "hwn")) {
    return 0x10 << 5;
  } else if (token_lexeme_eq(token, "hwq")) {
    return 0x11 << 5;
  } else if (token_lexeme_eq(token, "hwi")) {
    return 0x12 << 5;
  }

  throw std::domain_error("Unknown operation");
}

void Assembler::compile() {
  while (token_ != tokens_.end()) {
    switch (peek()->kind()) {
    case Token::Kind::Dot:
      this->directive();
      break;
    case Token::Kind::Colon:
      this->label();
      break;
    case Token::Kind::Identifier:
      this->op();
      break;
    default:
      throw std::domain_error("Unexpected token");
    }
  }
}

void Assembler::directive() {
  take(Token::Kind::Dot);
  auto name = take(Token::Kind::Identifier);

  // Check built-in directives
  if (token_lexeme_eq(name, "org")) {
    auto target = token_value(take(Token::Kind::Number));

    while (offset_ < target) {
      emit(0);
    }
  } else if (token_lexeme_eq(name, "dat")) {
    do {
      auto data = token_value(take(Token::Kind::Number, Token::Kind::String));
      emit(data);

      if (peek()->is_not(Token::Kind::Comma)) {
        break;
      }
      take(Token::Kind::Comma);
    } while (true);
  }

  // TODO(onbjerg): Check macros
  // TODO(onbjerg): Fail
}

void Assembler::label() {
  take(Token::Kind::Colon);
  auto name_token = take(Token::Kind::Identifier);
  auto name = std::string(name_token->lexeme());

  // TODO: Check if it uses a reserved word (e.g. a label cannot be named A)
  if (labels_.find(name) != labels_.end()) {
    // TODO(onbjerg): Improve error message
    throw std::domain_error("Label already exists");
  }

  labels_[name] = offset_;
}

std::tuple<Word, std::optional<Word>> Assembler::operand() {
  // TODO: Simple arithmetic?
  // TODO: Strings
  auto token = take(Token::Kind::Identifier, Token::Kind::LeftSquare,
                    Token::Kind::Number);

  if (token->is(Token::Kind::Identifier)) {
    // Registers
    if (token_lexeme_eq(token, "a")) {
      return {0x00, {}};
    } else if (token_lexeme_eq(token, "b")) {
      return {0x01, {}};
    } else if (token_lexeme_eq(token, "c")) {
      return {0x02, {}};
    } else if (token_lexeme_eq(token, "x")) {
      return {0x03, {}};
    } else if (token_lexeme_eq(token, "y")) {
      return {0x04, {}};
    } else if (token_lexeme_eq(token, "z")) {
      return {0x05, {}};
    } else if (token_lexeme_eq(token, "i")) {
      return {0x06, {}};
    } else if (token_lexeme_eq(token, "j")) {
      return {0x07, {}};
    }

    // Stack
    if (token_lexeme_eq(token, "push") || token_lexeme_eq(token, "pop")) {
      return {0x18, {}};
    } else if (token_lexeme_eq(token, "peek")) {
      return {0x19, {}};
    } else if (token_lexeme_eq(token, "sp")) {
      return {0x1b, {}};
    }

    // Special registers
    if (token_lexeme_eq(token, "pc")) {
      return {0x1c, {}};
    } else if (token_lexeme_eq(token, "ex")) {
      return {0x1d, {}};
    }
  }

  // Literals
  // TODO: Short literals (only for a)
  // TODO: Labels
  if (token->is(Token::Kind::Number)) {
    // Since this requires the DCPU16 to read the next word
    // and we haven't emitted the word in bytecode yet,
    // we will move the cursor back one position
    return {0x1f, {token_value(token)}};
  }

  if (token->is(Token::Kind::LeftSquare)) {
    // TODO: [SP--] and [SP++]
    // TODO: Labels
    // TODO: [register + next word]
    auto target = take(Token::Kind::Identifier, Token::Kind::Number);
    Word value_type = 0;

    if (target->is(Token::Kind::Identifier)) {
      if (token_lexeme_eq(token, "a")) {
        value_type = 0x10;
      } else if (token_lexeme_eq(token, "b")) {
        value_type = 0x11;
      } else if (token_lexeme_eq(token, "c")) {
        value_type = 0x12;
      } else if (token_lexeme_eq(token, "x")) {
        value_type = 0x13;
      } else if (token_lexeme_eq(token, "y")) {
        value_type = 0x14;
      } else if (token_lexeme_eq(token, "z")) {
        value_type = 0x15;
      } else if (token_lexeme_eq(token, "i")) {
        value_type = 0x16;
      } else if (token_lexeme_eq(token, "j")) {
        value_type = 0x17;
      }
    }

    if (target->is(Token::Kind::Number)) {
      take(Token::Kind::RightSquare);
      return {0x1e, {token_value(target)}};
    }

    // If we found the value type
    if (value_type > 0) {
      take(Token::Kind::RightSquare);
      return {value_type, {}};
    }
  }

  // TODO: Improve error message
  throw std::domain_error("Unexpected token");
}

constexpr Word OP_MASK = 0x1f;
void Assembler::op() {
  auto name = take(Token::Kind::Identifier);
  Word op = token_opcode(name);

  auto b = operand();
  if ((op & OP_MASK) == 0) {
    emit((std::get<0>(b) << 10) | op);
  } else {
    take(Token::Kind::Comma);
    auto a = operand();

    emit((std::get<0>(a) << 10) | (std::get<0>(b) << 4) | op);
    if (std::get<1>(a)) {
      emit(*std::get<1>(a));
    }
  }

  if (std::get<1>(b)) {
    emit(*std::get<1>(b));
  }
}

const Token *Assembler::peek() { return &(*token_); }

const Token *Assembler::take(Token::Kind kind) {
  auto token = &(*token_);
  if (token->is_not(kind)) {
    // TODO(onbjerg): Improve error message
    throw std::domain_error("Unexpected token");
  }
  token_++;

  return token;
}

template <typename... Ts>
const Token *Assembler::take(Token::Kind k1, Ts... ks) {
  auto token = &(*token_);
  if (!token->is_one_of(k1, ks...)) {
    // TODO(onbjerg): Improve error message
    throw std::domain_error("Unexpected token");
  }
  token_++;

  return token;
}

void Assembler::emit(Word word) {
  bytecode_.push_back(word);
  offset_++;
}

template <typename... Ts> void Assembler::emit(Word word, Ts... wn) {
  emit(word);
  emit(wn...);
}

#include "Lexer.h"

bool is_space(char c) noexcept {
  switch (c) {
  case ' ':
  case '\t':
  case '\r':
  case '\n':
    return true;
  default:
    return false;
  }
}

bool is_digit(char c) noexcept {
  switch (c) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    return true;
  default:
    return false;
  }
}

bool is_hex(char c) noexcept {
  switch (c) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case 'a':
  case 'A':
  case 'b':
  case 'B':
  case 'c':
  case 'C':
  case 'd':
  case 'D':
  case 'e':
  case 'E':
  case 'f':
  case 'F':
    return true;
  default:
    return false;
  }
}

bool is_identifier_char(char c) noexcept {
  switch (c) {
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'u':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case '_':
    return true;
  default:
    return false;
  }
}

Token Lexer::atom(Token::Kind kind) noexcept { return Token(kind, beg_++, 1); }

Token Lexer::next() noexcept {
  while (is_space(peek()))
    get();

  switch (peek()) {
  case '\0':
    return Token(Token::Kind::End, beg_, 1);
  default:
    return atom(Token::Kind::Unexpected);
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'u':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
    return identifier();
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    return number();
  case '(':
    return atom(Token::Kind::LeftParen);
  case ')':
    return atom(Token::Kind::RightParen);
  case '[':
    return atom(Token::Kind::LeftSquare);
  case ']':
    return atom(Token::Kind::RightSquare);
  case '+':
    return atom(Token::Kind::Plus);
  case '-':
    return atom(Token::Kind::Minus);
  case '.':
    return atom(Token::Kind::Dot);
  case ',':
    return atom(Token::Kind::Comma);
  case ':':
    return atom(Token::Kind::Colon);
  case ';': {
    // Skip until next newline or EOF
    while (peek() != '\n') {
      get();
      if (peek() == '\0') {
        return Token(Token::Kind::End, beg_, 1);
      }
    }
    return next();
  }
  case '"': {
    // Ignore starting "
    get();
    const char *start = beg_;

    // Continue until we hit another "
    while (peek() != '"' && peek() != '\0') {
      get();
    }

    if (peek() == '\0') {
      // The string was unterminated
      return Token(Token::Kind::Unexpected);
    }

    auto token = Token(Token::Kind::String, start, beg_);

    // Ignore ending "
    get();

    return token;
  }
  }
}

Token Lexer::identifier() noexcept {
  const char *start = beg_;
  get();
  while (is_identifier_char(peek()))
    get();
  return Token(Token::Kind::Identifier, start, beg_);
}

Token Lexer::number() noexcept {
  const char *start = beg_;
  get();

  // Support hexadecimals prefix
  if (peek() == 'x') {
    get();
    while (is_hex(peek()))
      get();
  }

  while (is_digit(peek()))
    get();
  return Token(Token::Kind::Number, start, beg_);
}

std::ostream &operator<<(std::ostream &os, const Token::Kind &kind) {
  static const char *const names[]{
      "Number",     "String",      "Identifier", "LeftParen", "RightParen",
      "LeftSquare", "RightSquare", "Plus",       "Minus",     "Dot",
      "Comma",      "Colon",       "End",        "Unexpected"};

  return os << names[static_cast<int>(kind)];
}

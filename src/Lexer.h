#pragma once
#include <iomanip>
#include <string>

class Token {
 public:
  enum class Kind {
    Number,
    Identifier,
    LeftParen,
    RightParen,
    LeftSquare,
    RightSquare,
    Plus,
    Minus,
    Asterisk,
    Dot,
    Comma,
    Colon,
    End,
    Unexpected,
  };

  Token(Kind kind) noexcept : kind_(kind) {}

  Token(Kind kind, const char *beg, std::size_t len) noexcept
      : kind_(kind), lexeme_(beg, len) {}

  Token(Kind kind, const char *beg, const char *end) noexcept
      : kind_(kind), lexeme_(beg, std::distance(beg, end)) {}

  Kind kind() const noexcept { return kind_; }
  void kind(Kind kind) noexcept { kind_ = kind; }

  bool is(Kind kind) const noexcept { return kind_ == kind; }

  bool is_not(Kind kind) const noexcept { return kind_ != kind; }

  bool is_one_of(Kind k1, Kind k2) const noexcept { return is(k1) || is(k2); }

  template <typename... Ts>
  bool is_one_of(Kind k1, Kind k2, Ts... ks) const noexcept {
    return is(k1) || is_one_of(k2, ks...);
  }

  std::string_view lexeme() const noexcept { return lexeme_; }
  void lexeme(std::string_view lexeme) noexcept { lexeme_ = std::move(lexeme); }

 private:
  Kind kind_{};
  std::string_view lexeme_{};
};

std::ostream &operator<<(std::ostream &os, const Token::Kind &kind);

class Lexer {
 public:
  Lexer(std::string str) noexcept : beg_(str.c_str()) {}
  Lexer(const char *beg) noexcept : beg_(beg) {}

  Token next() noexcept;

 private:
  Token identifier() noexcept;
  Token number() noexcept;
  Token atom(Token::Kind) noexcept;

  char peek() const noexcept { return *beg_; }
  char get() noexcept { return *beg_++; }

  const char *beg_ = nullptr;
};
#pragma once
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <optional>

#include "Lexer.h"

typedef std::uint16_t Word;
typedef std::vector<Token>::const_iterator TokenIterator;

class Assembler {
public:
  Assembler(std::vector<Token> tokens) : tokens_(tokens) {
    token_ = tokens_.cbegin();
  }

  //! Compile the provided tokens to bytecode.
  void compile();

  std::vector<Word> &bytecode() { return bytecode_; }

private:
  //! Attempt to parse a directive.
  void directive();
  //! Attempt to parse a label.
  void label();
  //! Attempt to parse an operand.
  std::tuple<Word, std::optional<Word>> operand();
  //! Attempt to parse an operation.
  void op();

  //! Get the token currently under the cursor
  const Token *peek();
  //! Get the next token in the token list and
  //! increase the token pointer offset if
  //! the token's kind matches, otherwise throw.
  const Token *take(Token::Kind kind);
  //! Get the next token in the token list and
  //! increase the token pointer offset if
  //! the token's kind matches any of the provided kinds,
  //! otherwise throw.
  template <typename... Ts> const Token *take(Token::Kind k1, Ts... ks);

  //! Add a word to the bytecode and increase the internal
  //! bytecode offset by 1.
  void emit(Word word);
  //! Add two or more words to the bytecode and increase
  //! the internal bytecode offset by the number of words added.
  template <typename... Ts> void emit(Word word, Ts... wn);

  std::vector<Token> tokens_;
  TokenIterator token_;
  // TODO(onbjerg): Get rid of offset, we can just use bytecode_->size()?
  Word offset_ = 0;
  std::vector<Word> bytecode_{};
  std::unordered_map<std::string, Word> labels_{};
  std::unordered_map<Word, std::string> label_refs_{};
};

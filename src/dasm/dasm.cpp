#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

#include "Lexer.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " FILE" << std::endl;
    return 1;
  }

  // Read file
  std::ifstream f(argv[1]);
  std::stringstream buffer;
  buffer << f.rdbuf();

  // TODO(onbjerg): For now, we just dump the tokens of the lexed
  // file. Later on, we want to dump the assembled bytecode
  // to a file.
  Lexer lex(buffer.str());
  auto token = lex.next();
  for (; !token.is_one_of(Token::Kind::End, Token::Kind::Unexpected);
       token = lex.next()) {
    std::cout << std::setw(12) << token.kind() << " |" << token.lexeme()
              << "|\n";
  }

  if (token.is(Token::Kind::Unexpected)) {
    std::cout << "Unexpected token: " << token.lexeme() << "\n";
  }

  return 0;
}

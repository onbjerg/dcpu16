#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

#include "Assembler.h"
#include "Lexer.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " FILE" << std::endl;
    return 1;
  }

  // Read file
  std::ifstream f(argv[1]);
  std::string contents((std::istreambuf_iterator<char>(f)),
                       std::istreambuf_iterator<char>());
  std::cout << "Compiling " << argv[1] << "..." << std::endl;

  // Lex file
  Lexer lex(contents.c_str());

  std::vector<Token> tokens;
  auto token = lex.next();
  for (; token.is_not(Token::Kind::End); token = lex.next()) {
    tokens.push_back(token);

    if (token.is(Token::Kind::Unexpected)) {
      throw std::domain_error("Unexpected token: " +
                              std::string(token.lexeme()));
    }
  }

  // Assemble
  Assembler assembler(tokens);

  assembler.compile();
  for (int i = 0; i < assembler.bytecode().size(); i++) {
    if (i % 8 == 0) {

      std::cout << std::endl
                << std::hex << std::setfill('0') << std::setw(4) << i << "| ";
    }

    std::cout << std::hex << std::setfill('0') << std::setw(4)
              << assembler.bytecode()[i] << " ";
  }

  std::cout << std::endl
            << std::endl
            << "Done (" << assembler.bytecode().size() * 2 << " bytes)"
            << std::endl;

  return 0;
}

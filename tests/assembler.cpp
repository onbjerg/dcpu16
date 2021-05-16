#include "../src/dasm/Assembler.h"
#include <catch2/catch.hpp>

TEST_CASE("Directives") {
  SECTION(".org") {
    auto assembler = Assembler(std::vector<Token>{
        Token(Token::Kind::Dot), Token(Token::Kind::Identifier, "org", 3),
        Token(Token::Kind::Number, "10", 2)});
    assembler.compile();

    REQUIRE_THAT(assembler.bytecode(),
                 Catch::Equals<Word>({0, 0, 0, 0, 0, 0, 0, 0, 0, 0}));
  }

  SECTION(".dat") {
    auto assembler = Assembler(std::vector<Token>{
        Token(Token::Kind::Dot), Token(Token::Kind::Identifier, "dat", 3),
        Token(Token::Kind::Number, "0xffff", 6), Token(Token::Kind::Comma),
        Token(Token::Kind::Number, "1", 1)});
    assembler.compile();

    REQUIRE_THAT(assembler.bytecode(), Catch::Equals<Word>({0xffff, 1}));
  }
}

#include "../src/dasm/Lexer.h"
#include <catch2/catch.hpp>

TEST_CASE("Numbers") {
  SECTION("Integers") {
    auto i = GENERATE("0", "1", "2", "3", "4", "5", "6", "7", "8", "9");

    auto lexer = Lexer(i);
    REQUIRE(lexer.next().kind() == Token::Kind::Number);
    REQUIRE(lexer.next().kind() == Token::Kind::End);
  }

  SECTION("Hexadecimals") {
    auto i = GENERATE("0x0", "0x1", "0x2", "0x3", "0x4", "0x5", "0x6", "0x7",
                      "0x8", "0x9", "0xa", "0xb", "0xc", "0xd", "0xe", "0xf");

    auto lexer = Lexer(i);
    REQUIRE(lexer.next().kind() == Token::Kind::Number);
    REQUIRE(lexer.next().kind() == Token::Kind::End);
  }
}

TEST_CASE("String literals") {
  SECTION("Valid strings") {
    auto s = GENERATE("\"hello world\"", "\"test123\"");

    auto lexer = Lexer(s);
    REQUIRE(lexer.next().kind() == Token::Kind::String);
    REQUIRE(lexer.next().kind() == Token::Kind::End);
  }

  SECTION("Invalid strings") {
    auto s = GENERATE("\"unterminated");
    auto lexer = Lexer(s);

    REQUIRE(lexer.next().kind() == Token::Kind::Unexpected);
    REQUIRE(lexer.next().kind() == Token::Kind::End);
  }
}

TEST_CASE("Identifiers") {
  SECTION("Valid identifiers") {
    auto identifier =
        GENERATE("foo",     // Just ASCII
                 "foo_bar", // Has an underscore in the middle
                 "foo_",    // Ends with an underscore
                 "x1"       // Has a number
        );

    auto lexer = Lexer(identifier);
    REQUIRE(lexer.next().kind() == Token::Kind::Identifier);
    REQUIRE(lexer.next().kind() == Token::Kind::End);
  }

  SECTION("Invalid identifiers") {
    auto identifier = GENERATE("_foo", // Starts with underscore
                                      "1x",   // Starts with a number
                                      "💩"     // Not ASCII
    );

    auto lexer = Lexer(identifier);
    REQUIRE(lexer.next().kind() == Token::Kind::Unexpected);
    REQUIRE(lexer.next().kind() == Token::Kind::End);
  }
}

Token::Kind parse_atomic(const char *str) {
  auto lexer = Lexer(str);
  return lexer.next().kind();
}

TEST_CASE("Special symbols") {
  REQUIRE(parse_atomic("(") == Token::Kind::LeftParen);
  REQUIRE(parse_atomic(")") == Token::Kind::RightParen);
  REQUIRE(parse_atomic("[") == Token::Kind::LeftSquare);
  REQUIRE(parse_atomic("]") == Token::Kind::RightSquare);
  REQUIRE(parse_atomic("+") == Token::Kind::Plus);
  REQUIRE(parse_atomic("-") == Token::Kind::Minus);
  REQUIRE(parse_atomic(".") == Token::Kind::Dot);
  REQUIRE(parse_atomic(",") == Token::Kind::Comma);
  REQUIRE(parse_atomic(":") == Token::Kind::Colon);
  REQUIRE(parse_atomic("") == Token::Kind::End);
}

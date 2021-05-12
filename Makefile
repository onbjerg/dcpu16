all: dasm

BUILD_DIR := ./build
SRC_DIR := ./src

DASM_SRCS := $(SRC_DIR)/bin/dasm.cpp
DASM_SRCS += $(SRC_DIR)/Lexer.cpp

dasm:
	g++ -Wall --std=c++17 $(DASM_SRCS) -o $(BUILD_DIR)/dasm

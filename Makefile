all: dasm

BUILD_DIR := ./build
SRC_DIR := ./src

DASM_SRCS := $(SRC_DIR)/dasm/dasm.cpp
DASM_SRCS += $(SRC_DIR)/dasm/Lexer.cpp

CPPFLAGS := -Wall --std=c++17

dasm:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DASM_SRCS) -o $(BUILD_DIR)/dasm

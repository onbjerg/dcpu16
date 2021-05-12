cmake_minimum_required(VERSION 3.20)

project(dcpu16)

# Configure compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -Wall -Wextra")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set sources
set(DASM_SRCS src/dasm/Lexer.cpp)

add_executable(dasm src/dasm/dasm.cpp ${DASM_SRCS})

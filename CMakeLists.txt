cmake_minimum_required(VERSION 3.20)

project(dcpu16)

# Configure compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -Wall -Wextra")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configure libraries
list(APPEND CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/lib/Catch2/contrib")
add_subdirectory(lib/Catch2)

# Set sources
set(DASM_SRCS src/dasm/Lexer.cpp src/dasm/Assembler.cpp)
set(TEST_SRCS tests/test.cpp tests/lexer.cpp tests/assembler.cpp)

add_executable(dasm src/dasm/dasm.cpp ${DASM_SRCS})

# Configure tests
add_executable(tests ${TEST_SRCS} ${DASM_SRCS})
target_link_libraries(tests PRIVATE Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(tests)

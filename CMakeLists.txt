cmake_minimum_required(VERSION 2.8)
project(DummyCompiler)

find_package(BISON)
find_package(FLEX)

# Set compilaton flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set the bison and flex targets compile rules
BISON_TARGET(BISON_PARSER src/BisonParser.yy src/flex_bison_output/BisonParser.cpp)
FLEX_TARGET(FLEX_LEXER src/FlexLexer.ll src/flex_bison_output/FlexLexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(FLEX_LEXER BISON_PARSER)

# Main output artefact
add_executable(${CMAKE_PROJECT_NAME}
        ${BISON_BISON_PARSER_OUTPUTS}
        ${FLEX_FLEX_LEXER_OUTPUTS}
        src/main.cpp src/CompilerContext.cpp include/CompilerContext.h include/AST.h include/IVisitor.h include/VisitorPrint.h)

# Bison/flex output headers
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC flex_bison_output/)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC flex_bison_output/)

# spdlog headers
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC external/spdlog/include)

# project local headers
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include/)

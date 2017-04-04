cmake_minimum_required(VERSION 2.8)
project(DummyCompiler)

# Set compilaton flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# FIXME: on llvm-3.6 need to copy /usr/share/llvm-3.6/ to /usr/lib/llvm-3.6/share
# Also for some reason there is dependency with libedit-dev (need to install from repos)

# NOTE: there is a problem with default name of cmake config file in llvm-3.4 build from ubuntu repos.
# You should rename LLVM-Config.cmake to LLVMConfig.cmake in /usr/share/llvm-3.4/cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/llvm-3.5/cmake")

find_package(LLVM)
if( NOT LLVM_FOUND )
    message(FATAL_ERROR "LLVM package can't be found. Set CMAKE_PREFIX_PATH variable to LLVM's installation prefix.")
endif()
add_definitions(${LLVM_DEFINITIONS})

find_package(BISON)
find_package(FLEX)

# Set the bison and flex targets compile rules
BISON_TARGET(BISON_PARSER src/BisonParser.yy ${CMAKE_SOURCE_DIR}/src/flex_bison_output/BisonParser.cpp)
FLEX_TARGET(FLEX_LEXER src/FlexLexer.ll ${CMAKE_SOURCE_DIR}/src/flex_bison_output/FlexLexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(FLEX_LEXER BISON_PARSER)

# Main output artefact
add_executable(${CMAKE_PROJECT_NAME}
        ${BISON_BISON_PARSER_OUTPUTS}
        ${FLEX_FLEX_LEXER_OUTPUTS}
        src/main.cpp src/CompilerContext.cpp include/CompilerContext.h include/AST.h include/IVisitor.h include/VisitorPrint.h)

# Bison/flex output headers
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC flex_bison_output/)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC flex_bison_output/)

# FIXME: for some reason LLVM_INCLUDE_DIRS is not set by default by LLVMConfig.cmake on llvm-3.4
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${LLVM_INCLUDE_DIRS})

# spdlog headers
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC external/spdlog/include)

# project local headers
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include/)

# add llvm dependencies
llvm_map_components_to_libnames(REQ_LLVM_LIBRARIES core native)

link_directories( ${LLVM_LIBRARY_DIRS} )
target_link_libraries(${CMAKE_PROJECT_NAME} ${REQ_LLVM_LIBRARIES})

cmake_minimum_required(VERSION 2.8)
project(DummyCompiler)

# Set compilaton flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Main output artefact
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/LangTokenHolder.cpp src/StreamLexer.cpp)

# spdlog headers
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC external/spdlog/include)

# project local headers
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include/)

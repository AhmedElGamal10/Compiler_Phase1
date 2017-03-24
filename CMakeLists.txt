cmake_minimum_required(VERSION 3.6)
project(cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Graph.cpp Graph.h Analyzer.cpp Analyzer.h FileParser.cpp FileParser.h State.h State.cpp)
add_executable(cpp ${SOURCE_FILES} main.cpp)
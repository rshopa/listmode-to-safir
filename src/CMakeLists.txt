cmake_minimum_required(VERSION 2.6)
project(SAFIR_JPET_TRANSFORM CXX) # using only C++

add_definitions(-std=c++11 -Wall -Wunused-parameter -Wextra)

add_executable(ConvertToMUPET ${CMAKE_CURRENT_SOURCE_DIR}/ConvertToMUPET.cxx)

add_executable(GenerateCrystalMapJPET ${CMAKE_CURRENT_SOURCE_DIR}/GenerateCrystalMapJPET.cxx)
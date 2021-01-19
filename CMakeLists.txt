# cmake_minimum_required(VERSION <specify CMake version here>)
project(renderer)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES *.h *.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
cmake_minimum_required(VERSION 3.17)
project(tree_serialization)

set(CMAKE_CXX_STANDARD 17)

set(SRC main.cpp NodeValue.cpp Node.cpp Tree.cpp)

add_executable(tree_serialization ${SRC})
cmake_minimum_required(VERSION 3.15)
project(tree_serialization)

set(CMAKE_CXX_STANDARD 17)

enable_testing()
find_package(GTest REQUIRED)

include_directories(tree_serialization_lib)

add_subdirectory(tree_serialization_lib)
add_executable(tree_serialization main.cpp)
target_link_libraries(tree_serialization tree_serialization_lib)
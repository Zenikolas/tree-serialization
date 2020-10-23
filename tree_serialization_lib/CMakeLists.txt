set(SRC NodeValue.cpp Node.cpp Tree.cpp)

add_library(tree_serialization_lib ${SRC})
add_subdirectory(tests)
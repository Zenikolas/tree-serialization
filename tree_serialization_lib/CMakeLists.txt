set(SRC NodeValue.cpp Tree.cpp Util.cpp)

add_library(tree_serialization_lib ${SRC})
add_subdirectory(tests)
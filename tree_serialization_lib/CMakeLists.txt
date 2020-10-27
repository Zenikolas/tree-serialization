set(SRC NodeValue.cpp TreeUtil.cpp)

add_library(tree_serialization_lib ${SRC})
add_subdirectory(tests)
set(SRC Node.cpp TreeUtil.cpp)

add_library(tree_serialization_lib ${SRC})
add_subdirectory(tests)
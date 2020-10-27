set(SRC NodeValue.cpp TreeUtil.cpp Util.cpp)

add_library(tree_serialization_lib ${SRC})
add_subdirectory(tests)
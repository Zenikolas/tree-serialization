include_directories(../)

set(SRC NodeValue.t.cpp Tree.t.cpp Util.t.cpp Node.t.cpp)
add_executable(tree_serialization_tests ${SRC})
target_link_libraries(tree_serialization_tests tree_serialization_lib GTest::GTest GTest::Main)

gtest_discover_tests(tree_serialization_tests)
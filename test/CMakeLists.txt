add_executable(example_test example_test.cpp)
target_link_libraries(example_test GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(example_test)

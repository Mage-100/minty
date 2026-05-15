add_executable(example_test example_test.cpp)
target_link_libraries(example_test GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(example_test)

add_executable(font_engine_test font_engine_tests/test.cpp)
target_link_libraries(font_engine_test PRIVATE
    font_engine
    font_engine_internal
    dwrite
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(font_engine_test)

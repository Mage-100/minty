include(GoogleTest)

add_executable(example_test example_test.cpp)
target_link_libraries(example_test GTest::gtest_main)
gtest_discover_tests(example_test)

add_executable(utf8parser_test "utf8parser_test/test.cpp")
target_include_directories(utf8parser_test PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
)
target_sources(utf8parser_test PRIVATE
    "${CMAKE_SOURCE_DIR}/src/utf8parser.cpp"
)

target_link_libraries(utf8parser_test GTest::gtest_main)
gtest_discover_tests(utf8parser_test)

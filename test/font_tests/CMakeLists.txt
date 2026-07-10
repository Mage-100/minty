add_executable(font_test test.cpp)

target_compile_definitions(font_test PRIVATE
    FONT_SOURCE="${CMAKE_SOURCE_DIR}/static_fonts"
)

target_link_libraries(font_test PRIVATE
    Font
    GTest::gtest_main
)

target_compile_definitions(font_test PRIVATE
    FONT_SOURCE="${CMAKE_SOURCE_DIR}/static_fonts"
)

include(GoogleTest)
gtest_discover_tests(font_test)

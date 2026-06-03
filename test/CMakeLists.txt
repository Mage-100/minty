add_executable(example_test example_test.cpp)
target_link_libraries(example_test GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(example_test)

set(COLOR_EMOJI_FONT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/font_engine_tests/NotoColorEmoji.ttf")
get_filename_component(
    COLOR_EMOJI_FONT_PATH_ABS
    "${COLOR_EMOJI_FONT_PATH}"
    ABSOLUTE
)

add_executable(font_engine_test font_engine_tests/test.cpp)
target_compile_definitions(font_engine_test PRIVATE
    COLOR_EMOJI_FONT_PATH="${COLOR_EMOJI_FONT_PATH_ABS}"
)
target_link_libraries(font_engine_test PRIVATE
    font_engine
    font_engine_internal
    dwrite
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(font_engine_test)

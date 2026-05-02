add_executable(minty main.cpp)
target_include_directories(minty PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/dependencies/glfw/include"
    "${CMAKE_SOURCE_DIR}/dependencies/glad/include"
    "${CMAKE_SOURCE_DIR}/dependencies/glm"
    "${HARFBUZZ_DIR}/install_harfbuzz/include"
    "${HARFBUZZ_DIR}/install_harfbuzz/include/freetype2"
)

target_sources(minty PUBLIC
    "font.cpp"
    "mty_app.cpp"
    "mty_renderer.cpp"
    "mty_runtime.cpp"
    "mty_state.cpp"
    "mty_window.cpp"
    "mty_cellgrid.cpp"
    "mty_prompt_renderer.cpp"
    "conpty.cpp"
    "vtparser.cpp"
)

target_precompile_headers(minty PRIVATE "pch.h")

target_link_libraries(
    minty
    glfw glad opengl32
    harfbuzz
    "${HARFBUZZ_DIR}/install_harfbuzz/lib/freetype.lib"
)

add_custom_command(TARGET minty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${HARFBUZZ_DIR}/install_harfbuzz/bin/harfbuzz.dll"
        $<TARGET_FILE_DIR:minty>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${HARFBUZZ_DIR}/install_harfbuzz/bin/freetype-6.dll"
        $<TARGET_FILE_DIR:minty>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${HARFBUZZ_DIR}/install_harfbuzz/bin/png16-16.dll"
        $<TARGET_FILE_DIR:minty>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${HARFBUZZ_DIR}/install_harfbuzz/bin/harfbuzz-raster.dll"
        $<TARGET_FILE_DIR:minty>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${HARFBUZZ_DIR}/install_harfbuzz/bin/harfbuzz-subset.dll"
        $<TARGET_FILE_DIR:minty>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${HARFBUZZ_DIR}/install_harfbuzz/bin/harfbuzz-vector.dll"
        $<TARGET_FILE_DIR:minty>
)

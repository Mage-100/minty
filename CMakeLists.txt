cmake_minimum_required(VERSION 4.1)
project(mint VERSION 1.0)

# set(OpenGL_GL_PREFERENCE GLVND)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("./dependencies/glfw")
add_library(glad STATIC "./dependencies/glad/src/gl.c")
target_include_directories(glad PRIVATE
    "./dependencies/glad/include"
)

# ----------------
# Doing Harfbuzz
set(HARFBUZZ_DIR "${CMAKE_SOURCE_DIR}/dependencies/harfbuzz")
add_library(harfbuzz SHARED IMPORTED)
set_target_properties(harfbuzz PROPERTIES
    IMPORTED_LOCATION "${HARFBUZZ_DIR}/install_harfbuzz/bin/harfbuzz.dll"
    IMPORTED_IMPLIB "${HARFBUZZ_DIR}/install_harfbuzz/lib/harfbuzz.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${HARFBUZZ_DIR}/install_harfbuzz/include"
)
# ----------------

add_executable(minty ./src/main.cpp)
target_include_directories(minty PRIVATE
    "./include"
    "./dependencies/glfw/include"
    "./dependencies/glad/include"
    "./dependencies/glm"
    "${HARFBUZZ_DIR}/install_harfbuzz/include"
    "${HARFBUZZ_DIR}/install_harfbuzz/include/freetype2"
)

target_sources(minty PUBLIC
    "./src/font.cpp"
    "./src/mty_app.cpp"
    "./src/mty_renderer.cpp"
    "./src/mty_runtime.cpp"
    "./src/mty_state.cpp"
    "./src/mty_window.cpp"
    "./src/mty_cellgrid.cpp"
    "./src/mty_prompt_renderer.cpp"
    "./src/conpty.cpp"
    "./src/vtparser.cpp"
)

target_precompile_headers(minty PRIVATE "./src/pch.h")

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

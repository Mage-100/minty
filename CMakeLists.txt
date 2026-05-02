cmake_minimum_required(VERSION 4.1)
project(mint VERSION 1.0)

# set(OpenGL_GL_PREFERENCE GLVND)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# GoogleTest (global dependency)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# add_library(GTest::GTest INTERFACE IMPORTED)
# target_link_libraries(GTest::GTest INTERFACE gtest_main)

enable_testing()

# GLFW (global dependency)
add_subdirectory("./dependencies/glfw")

# glad (global dependency)
add_library(glad STATIC "./dependencies/glad/src/gl.c")
target_include_directories(glad PUBLIC
    "./dependencies/glad/include"
)

# Harfbuzz (global dependency)
set(HARFBUZZ_DIR "${CMAKE_SOURCE_DIR}/dependencies/harfbuzz")
add_library(harfbuzz SHARED IMPORTED)
set_target_properties(harfbuzz PROPERTIES
    IMPORTED_LOCATION "${HARFBUZZ_DIR}/install_harfbuzz/bin/harfbuzz.dll"
    IMPORTED_IMPLIB "${HARFBUZZ_DIR}/install_harfbuzz/lib/harfbuzz.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${HARFBUZZ_DIR}/install_harfbuzz/include"
)
# ----------------

add_subdirectory(src)
add_subdirectory(test)

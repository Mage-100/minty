cmake_minimum_required(VERSION 3.21)
project(mint VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

enable_testing()

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)

add_executable(minty main.cpp)
target_include_directories(minty PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
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

target_link_libraries(minty PRIVATE
    Freetype::Freetype
    glad::glad
    glm::glm
    glfw
    harfbuzz::harfbuzz
)

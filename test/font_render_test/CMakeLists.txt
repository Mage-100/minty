add_subdirectory(core)

add_executable(font_render_test test.cpp)

target_link_libraries(font_render_test PRIVATE 
	core
    Font
    glm::glm
)

target_compile_definitions(font_render_test PRIVATE
    FONT_SOURCE="${CMAKE_SOURCE_DIR}/static_fonts"
)

target_compile_definitions(font_render_test PRIVATE
    SHADER_SOURCE="${CMAKE_CURRENT_SOURCE_DIR}/shaders"
)

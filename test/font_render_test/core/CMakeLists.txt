add_library(core
    "${CMAKE_CURRENT_SOURCE_DIR}/Window.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/VertexBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/VertexArray.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/VertexBufferLayout.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Shader.cpp"
)

target_include_directories(core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(core PUBLIC
    glfw
    glad::glad
    glm::glm
)

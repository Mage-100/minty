add_library(font_engine_internal STATIC
    src/font_engine/internal/font_manager.cpp
    src/font_engine/internal/DirectWriteWrapper.cpp
)

target_include_directories(font_engine_internal PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(font_engine_internal INTERFACE

    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

add_library(font_engine
    src/font_engine.cpp
)

target_link_libraries(font_engine PRIVATE
    font_engine_internal
    Freetype::Freetype
    dwrite
)

target_include_directories(font_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

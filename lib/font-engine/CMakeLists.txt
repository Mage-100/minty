add_library(font_engine
    src/font_engine.cpp
)

target_link_libraries(font_engine PRIVATE
    Freetype::Freetype
)

target_include_directories(font_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(Font
    "Font.cpp"
    "FontAtlas.cpp"
)

target_link_libraries(Font PUBLIC
    Freetype::Freetype
)

target_include_directories(Font PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

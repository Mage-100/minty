#include <ft2build.h>
#include FT_FREETYPE_H

#include "Font.hpp"

const std::string FontError(const std::string& msg) {
    return "[FontError]: " + msg;
}

inline static void ft_check(FT_Error error, const std::string& msg) {
    if (error) {
        throw FontError(msg);
    }
}

Font::Font() {
    ft_check(
        FT_Init_FreeType(&_library),
        "Failed to initialize Freetype library"
    );
}

Font::~Font() {
    if (!cache.empty()) {
        for(const auto& [key, value] : cache) {
            FT_Done_Face(value.face);
        }
    }

    FT_Done_FreeType(_library);
}

int Font::addFontFromPath(const std::string& path, int font_size) {
    FT_Face face = nullptr;
    ft_check(
        FT_New_Face(_library, path.c_str(), 0, &face),
        "Failed to create Freetype Face: " + path
    );

    FT_Set_Pixel_Sizes(face, 0, font_size);

    _FontObject obj = {
        .face = face,
        .font_size = font_size
    };

    cache.insert({cache_counter, obj});
    return cache_counter++;
}

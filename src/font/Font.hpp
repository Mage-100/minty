#pragma once
#include <string>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H

class Font {
    FT_Library _library;

    struct _FontObject {
        FT_Face face;
        int font_size;
    };

    int cache_counter = 0;
    std::unordered_map<int, _FontObject> cache;
public:
    Font();
    ~Font();

    int addFontFromPath(const std::string& path, int font_size);
};

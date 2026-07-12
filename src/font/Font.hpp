#pragma once
#include <string>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H

class Font {
public:
    struct GlyphSize {
        signed long int width;
        signed long int height;
    };

    struct GlyphBearing {
        signed long int x;
        signed long int y;
    };

    struct GlyphBitmap {
        unsigned int width;
        unsigned int height;
        int pitch;
    };
private:
    FT_Library _library;

    struct GlyphObject {
        GlyphSize size;
        signed long int advanceX;
        GlyphBearing bearing;
        GlyphBitmap bitmap;
        unsigned char* bitmapBuffer;
    };

    struct _FontObject {
        FT_Face face;
        int font_size;
        std::unordered_map<unsigned int, GlyphObject> glyph_cache;
    };

    int cache_counter = 0;
    std::unordered_map<int, _FontObject> cache;
public:
    Font();
    ~Font();

    int addFontFromPath(const std::string& path, int font_size);

    unsigned char* getGlyphBuffer(int fontID, unsigned int codepoint);

    GlyphSize getGlyphSize(int fontID, unsigned int codepoint);
    GlyphBearing getGlyphBearing(int fontID, unsigned int codepoint);
    GlyphBitmap getGlyphBitmap(int fontID, unsigned int codepoint);

    signed long int getGlyphAdvanceX(int fontID, unsigned int codepoint);
private:
    std::unordered_map<int, _FontObject>::iterator checkFontID(int fontID);
    GlyphObject& loadGlyph(int fontID, unsigned int codepoint);
};

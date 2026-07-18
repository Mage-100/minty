#include <iostream>
#include <string>

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

unsigned char* Font::getGlyphBuffer(int fontID, unsigned int codepoint) {
    auto& glyphObj = loadGlyph(fontID, codepoint);
    return glyphObj.bitmapBuffer;
}

Font::GlyphSize Font::getGlyphSize(int fontID, unsigned int codepoint) {
    auto& glyphObj = loadGlyph(fontID, codepoint);
    return glyphObj.size;
}

Font::GlyphBearing Font::getGlyphBearing(int fontID, unsigned int codepoint) {
    auto& glyphObj = loadGlyph(fontID, codepoint);
    return glyphObj.bearing;
}

Font::GlyphBitmap Font::getGlyphBitmap(int fontID, unsigned int codepoint) {
    auto& glyphObj = loadGlyph(fontID, codepoint);
    return glyphObj.bitmap;
}

FT_Short Font::getAscender(int fontID) {
    auto fontObjIt = checkFontID(fontID);
    auto& fontObj = fontObjIt->second;
    return fontObj.face->ascender;
}

FT_Short Font::getDescender(int fontID) {
    auto fontObjIt = checkFontID(fontID);
    auto& fontObj = fontObjIt->second;
    return fontObj.face->descender;
}

FT_UShort Font::getUnitsPerEm(int fontID) {
    auto fontObjIt = checkFontID(fontID);
    auto& fontObj = fontObjIt->second;
    return fontObj.face->units_per_EM;
}

FT_Pos Font::getMaxAdvanceX(int fontID) {
    auto fontObjIt = checkFontID(fontID);
    auto& fontObj = fontObjIt->second;
    return fontObj.face->size->metrics.max_advance;
}

signed long int Font::getGlyphAdvanceX(int fontID, unsigned int codepoint) {
    auto& glyphObj = loadGlyph(fontID, codepoint);
    return glyphObj.advanceX;
}

std::unordered_map<int, Font::_FontObject>::iterator Font::checkFontID(int fontID) {
    auto it = cache.find(fontID);

    if (it == cache.end())
        throw "Font not created: " + std::to_string(fontID);

    return it;
}

Font::GlyphObject& Font::loadGlyph(int fontID, unsigned int codepoint) {
    auto fontIter = checkFontID(fontID);
    auto& fontObj = fontIter->second;

    auto it = fontObj.glyph_cache.find(codepoint);

    if (it != fontObj.glyph_cache.end()) {
        return it->second;
    } else {
        auto& face = fontObj.face;

        FT_UInt glyphIndex = FT_Get_Char_Index(fontObj.face, codepoint);
        FT_Int32 loadFlags = FT_LOAD_RENDER;

        if (FT_Load_Glyph(fontObj.face, glyphIndex, loadFlags) != 0)
            std::cerr << "Failed to find glyph: Codepoint: " + std::to_string(codepoint);

        GlyphObject glyphObj = {
            .size = {
                .width = fixedToPixels(face->glyph->metrics.width),
                .height = fixedToPixels(face->glyph->metrics.height)
            },
            .advanceX = fixedToPixels(face->glyph->metrics.horiAdvance),
            .bearing = {
                .x = fixedToPixels(face->glyph->metrics.horiBearingX),
                .y = fixedToPixels(face->glyph->metrics.horiBearingY)
            },
            .bitmap = {
                .width = face->glyph->bitmap.width,
                .height = face->glyph->bitmap.rows,
                .pitch = face->glyph->bitmap.pitch
            },
            .bitmapBuffer = face->glyph->bitmap.buffer
        };

        auto inserted = fontObj.glyph_cache.insert({codepoint, glyphObj});

        if (inserted.second == true) return inserted.first->second;
        else throw "Failed to load glyph: Codepoint: " + std::to_string(codepoint);
    }
}

template<typename T>
T Font::fixedToPixels(T x) {
    return x >> 6;
}
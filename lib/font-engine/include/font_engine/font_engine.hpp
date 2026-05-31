#pragma once
#include "font_engine/FontID.hpp"
#include <string>
#include <memory>
#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <font_engine/FaceID.hpp>

struct GlyphMetadata {
    int glyph_width;
    int glyph_height;
    int bearingX;
    int bearingY;
    int advanceX;
    std::vector<std::uint32_t> pixelBuffer;
    int pixelBufferStride;
};

struct FontFace {
    FT_Face face;
    bool isEmoji = false;
    std::unordered_map<std::uint32_t, GlyphMetadata> glyphMetadataCache; // stores glyphmetadata per character
    int ascender;
    int descender;
};

class FontManager;

class FontEngine {
public:
    FontEngine();
    ~FontEngine();

    // Only to load fonts - not colored fonts (emojis)
    // uses font_manager
    FontID loadFontByName(const std::string&, int);

    FaceID loadFaceFromPath(const std::string&, int);
    FaceID loadEmojiFontFromPath(const std::string&);
    bool isFaceMonospaced(FaceID);
    bool rasterize(FaceID, std::uint32_t);
    GlyphMetadata* getGlyph(FaceID, std::uint32_t);
private:
    FT_Library ft_library;
    std::unique_ptr<FontManager> font_manager;

    std::unordered_map<FaceID, FontFace> font_face_cache;
    std::unordered_map<FontID, std::vector<FaceID>> font_cache;
    bool isFaceInCache(FaceID);
};

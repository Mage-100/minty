#pragma once
#include <string>
#include <memory>
#include <unordered_map>

typedef int FaceID;

#include <ft2build.h>
#include FT_FREETYPE_H

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

    FaceID loadFaceFromPath(const std::string&, int);
    FaceID loadEmojiFontFromPath(const std::string&);
    bool isFaceMonospaced(FaceID);
    bool rasterize(FaceID, std::uint32_t);
    GlyphMetadata* getGlyph(FaceID, std::uint32_t);
private:
    FT_Library ft_library;
    std::unique_ptr<FontManager> fontmanager;

    std::unordered_map<FaceID, FontFace> font_face_cache;
    bool isFaceInCache(FaceID);
};

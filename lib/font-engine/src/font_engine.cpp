#include <cstdint>
#include <iostream>
#include <memory>
#include <cassert>

#include <font_engine/font_engine.hpp>
#include <font_engine/font_utils.hpp>
#include <font_engine/font_container.hpp>
#include "font_engine/internal/font_manager.hpp"
#include <font_engine/TypedID.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

static void ftCheck(FT_Error error, const char* msg) {
    if (error)
        throw FontEngineError(msg);
}

// FreeType metrics for scalable faces are in 26.6 fixed-point; shift to pixels.
static int fixedToPixels(FT_Pos v) { return static_cast<int>(v >> 6); }

FontEngine::FontEngine()
    : font_manager(std::make_unique<FontManager>()) {
    ftCheck(FT_Init_FreeType(&ft_library),
                "Failed to initialize FreeType library");
}

FontEngine::FaceEntry& FontEngine::requireFace(FaceID id) {
    auto it = faceCache.find(id);
    if (it == faceCache.end())
        throw FontEngineError("Invalid FaceID");
    return it->second;
}

const FontEngine::FaceEntry& FontEngine::requireFace(FaceID id) const {
    auto it = faceCache.find(id);
    if (it == faceCache.end())
        throw FontEngineError("Invalid FaceID");
    return it->second;
}

FaceID FontEngine::loadFaceFromPath(const std::string& path, int pixelSize) {
    assert(pixelSize > 0);

    FT_Face face = nullptr;
    FT_Error err = FT_New_Face(ft_library, path.c_str(), 0, &face);
    if (err == FT_Err_Unknown_File_Format)
        throw FontEngineError("Unknown font file format: " + path);
    if (err)
        throw FontEngineError("Failed to open font file: " + path);

    ftCheck(FT_Set_Pixel_Sizes(face, 0, pixelSize),
            "Failed to set pixel size");

    FaceEntry entry;
    entry.face      = face;
    entry.isEmoji   = false;
    entry.ascender  = fixedToPixels(face->ascender);
    entry.descender = fixedToPixels(face->descender);

    auto id = FaceID::make();
    faceCache.emplace(id, std::move(entry));
    return id;
}

FaceID FontEngine::loadEmojiFaceFromPath(const std::string& path) {
    FT_Face face;
    int error = FT_New_Face(ft_library, path.c_str(), 0, &face);
    if (error == FT_Err_Unknown_File_Format) {
        std::cerr << "Error: Unknown Font File Format" << std::endl;
        exit(EXIT_FAILURE);
    } else if (error) {
        std::cerr << "Error: Failed to open Font File: " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    FT_Select_Size(face, 0);

    FaceEntry entry;
    entry.face = face;
    entry.isEmoji = true;
    entry.ascender = face->ascender >> 6;
    entry.descender = face->descender >> 6;

    auto id = FaceID::make();
    faceCache.emplace(id, std::move(entry));
    return id;
}

bool FontEngine::isFaceMonospaced(FaceID id) const {
    const FaceEntry& entry = requireFace(id);
    return FT_IS_FIXED_WIDTH(entry.face) != 0;
}

bool FontEngine::rasterizeIntoCache(FaceID id, uint32_t codepoint) {
    FaceEntry& faceEntry = requireFace(id);

    FT_UInt glyphIndex = FT_Get_Char_Index(faceEntry.face, codepoint);
    FT_Int32 loadFlags = faceEntry.isEmoji ? (FT_LOAD_RENDER | FT_LOAD_COLOR) : FT_LOAD_RENDER;

    if (FT_Load_Glyph(faceEntry.face, glyphIndex, loadFlags) != 0)
        return false;

    FT_Bitmap& bmp = faceEntry.face->glyph->bitmap;
    int w = static_cast<int>(bmp.width);
    int h = static_cast<int>(bmp.rows);

    GlyphMetadata meta;
    meta.width    = w;
    meta.height   = h;
    meta.bearingX = faceEntry.face->glyph->bitmap_left;
    meta.bearingY = faceEntry.face->glyph->bitmap_top;
    meta.advanceX = fixedToPixels(faceEntry.face->glyph->advance.x);
    meta.stride   = bmp.pitch;
    meta.pixels.resize(static_cast<size_t>(w * h), 0);

    if (faceEntry.isEmoji) {
        for (int i = 0; i < w * h; i++) {
            const uint8_t* src = bmp.buffer + (i * 4);
            meta.pixels[i] = (static_cast<uint32_t>(src[3]) << 24) |
                             (static_cast<uint32_t>(src[2]) << 16) |
                             (static_cast<uint32_t>(src[1]) << 8)  |
                              static_cast<uint32_t>(src[0]);
        }
    } else {
        for (int i = 0; i < w * h; i++)
            meta.pixels[i] = static_cast<uint32_t>(bmp.buffer[i]) << 24;
    }

    faceEntry.glyphCache.emplace(codepoint, std::move(meta));
    return true;
}

const GlyphMetadata* FontEngine::getGlyph(FaceID id, uint32_t codepoint) {
    FaceEntry& entry = requireFace(id);

    auto glyphIt = entry.glyphCache.find(codepoint);
    if (glyphIt == entry.glyphCache.end()) {
        rasterizeIntoCache(id, codepoint);
        glyphIt = entry.glyphCache.find(codepoint);
    }

    return &glyphIt->second;
}

FontContainer* FontEngine::createFontContainer() {
    auto c = std::make_unique<FontContainer>(ft_library, font_manager.get());
    auto* ptr = c.get();
    m_containers.push_back(std::move(c));
    return ptr;
}

FontEngine::~FontEngine() {
    m_containers.clear();
    for (auto& [id, entry] : faceCache)
        FT_Done_Face(entry.face);
    FT_Done_FreeType(ft_library);
}

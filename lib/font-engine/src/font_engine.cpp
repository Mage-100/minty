#include <cstdint>
#include <iostream>
#include <memory>
#include <cassert>

#include <font_engine/font_engine.hpp>
#include <font_engine/font_utils.hpp>
#include "font_engine/internal/font_manager.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <harfbuzz/hb-ft.h>

FontEngine::FontEngine()
    : fontmanager(std::make_unique<FontManager>()) {
    int error;
    error = FT_Init_FreeType(&ft_library);
    if (error) {
        std::cerr << __FILE__ << ":" << __LINE__ << std::endl;
        std::cerr << "Error: Failed to initialize Freetype Library" << std::endl;
        exit(EXIT_FAILURE);
    }
}

FaceID FontEngine::loadFaceFromPath(const std::string& path, int font_size) {
    assert(font_size > 0);

    FaceID id = path.length() + font_size;

    FT_Face face;
    int error = FT_New_Face(ft_library, path.c_str(), 0, &face);
    if (error == FT_Err_Unknown_File_Format) {
        std::cerr << "Error: Unknown Font File Format" << std::endl;
        exit(EXIT_FAILURE);
    } else if (error) {
        std::cerr << "Error: Failed to open Font File: " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    error = FT_Set_Pixel_Sizes(face, font_size, 0);
    if (error) {
        std::cerr << "Error: Failed to set pixel size" << std::endl;
        exit(EXIT_FAILURE);
    }

    FontFace f_face;
    f_face.face = face;
    f_face.ascender = face->ascender >> 6;
    f_face.descender = face->descender >> 6;

    font_face_cache.insert({id, f_face});
    return id;
}

FaceID FontEngine::loadEmojiFontFromPath(const std::string& path) {
    FaceID id = path.length() + 100;

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

    FontFace f_face;
    f_face.face = face;
    f_face.isEmoji = true;
    f_face.ascender = face->ascender >> 6;
    f_face.descender = face->descender >> 6;

    font_face_cache.insert({id, f_face});
    return id;
}

bool FontEngine::isFaceInCache(FaceID id) {
    auto it = font_face_cache.find(id);
    if (it != font_face_cache.end()) {
        return true;
    }
    return false;
}

bool FontEngine::isFaceMonospaced(FaceID id) {
    if (!isFaceInCache(id)) {
        std::cerr << "Not Valid FaceID" << std::endl;
        return false;
    }

    auto it = font_face_cache.find(id);

    if (FT_IS_FIXED_WIDTH(it->second.face) == 1) return true;
    return false;
}

bool FontEngine::rasterize(FaceID id, std::uint32_t ch) {
    if (!isFaceInCache(id)) {
        std::cerr << "Not Valid FaceID" << std::endl;
        return false;
    }

    auto it = font_face_cache.find(id);

    GlyphMetadata metadata;

    int glyph_index = FT_Get_Char_Index(it->second.face, ch);
    int error;
    if (it->second.isEmoji) {
        error = FT_Load_Glyph(it->second.face, glyph_index, FT_LOAD_RENDER | FT_LOAD_COLOR);
    } else {
        error = FT_Load_Glyph(it->second.face, glyph_index, FT_LOAD_RENDER);
    }
    if(error) {
        std::cerr << "Failed to load char: " << ch << std::endl;
        return false;
    }

    metadata.glyph_width           = it->second.face->glyph->bitmap.width;
    metadata.glyph_height          = it->second.face->glyph->bitmap.rows;
    metadata.bearingX              = it->second.face->glyph->bitmap_left;
    metadata.bearingY              = it->second.face->glyph->bitmap_top;
    metadata.advanceX              = it->second.face->glyph->advance.x >> 6;
    metadata.pixelBufferStride     = it->second.face->glyph->bitmap.pitch;


    int w = it->second.face->glyph->bitmap.width;
    int h = it->second.face->glyph->bitmap.rows;
    metadata.pixelBuffer.resize(w * h);

    if (it->second.isEmoji) {
        for (int i = 0; i < w * h; i++) {
            std::uint32_t b = it->second.face->glyph->bitmap.buffer[i*4+0];
            std::uint32_t g = it->second.face->glyph->bitmap.buffer[i*4+1];
            std::uint32_t r = it->second.face->glyph->bitmap.buffer[i*4+2];
            std::uint32_t a = it->second.face->glyph->bitmap.buffer[i*4+3];

            std::uint32_t pixel = a << 8*3 | (b << 8*2) | (g << 8*1) | r;
            metadata.pixelBuffer[i] = pixel;
        }
    } else {
        for (int i = 0; i < w * h; i++) {
            std::uint8_t pixel = it->second.face->glyph->bitmap.buffer[i];
            if (pixel > 0) {
                metadata.pixelBuffer[i] = static_cast<std::uint32_t>(pixel) << 8*3;
            } else {
                metadata.pixelBuffer[i] = 0;
            }
        }
    }

    it->second.glyphMetadataCache.insert({ch, metadata});
    return true;
}

GlyphMetadata* FontEngine::getGlyph(FaceID id, std::uint32_t ch) {
    if (!isFaceInCache(id)) {
        std::cerr << "Not Valid FaceID" << std::endl;
        return nullptr;
    }

    auto it = font_face_cache.find(id);
    auto glyphIt = it->second.glyphMetadataCache.find(ch);

    if (glyphIt == it->second.glyphMetadataCache.end()) {
        this->rasterize(id,ch);
        glyphIt = it->second.glyphMetadataCache.find(ch);
    }


    return &glyphIt->second;
}

FontEngine::~FontEngine() {
    auto it = font_face_cache.begin();

    while (it!=font_face_cache.end()) {
        FT_Done_Face(it->second.face);
        it++;
    }

    FT_Done_FreeType(ft_library);
}

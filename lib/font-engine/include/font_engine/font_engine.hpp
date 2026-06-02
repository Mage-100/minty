#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <stdexcept>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <font_engine/TypedID.hpp>
#include <font_engine/font_utils.hpp>


struct GlyphMetadata {
    int width   = 0;
    int height  = 0;
    int bearingX = 0;
    int bearingY = 0;
    int advanceX = 0;
    int stride   = 0;
    std::vector<std::uint32_t> pixels;
};


struct FontEngineError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

class FontManager;

class FontEngine {
public:
    FontEngine();
    ~FontEngine();

    FontEngine(const FontEngine&)            = delete;
    FontEngine& operator=(const FontEngine&) = delete;

    // Load all faces that make up a named font family (regular, bold, italic…).
    // Throws FontEngineError on failure.
    FontID loadFontByName(const std::string& name, int pixelSize);

    // Load a single face directly from a file path.
    // Throws FontEngineError on failure.
    FaceID loadFaceFromPath(const std::string& path, int pixelSize);

    // Load a color (emoji) face. Bitmap strike index 0 is selected automatically.
    // Throws FontEngineError on failure.
    FaceID loadEmojiFaceFromPath(const std::string& path);

    bool isFaceMonospaced(FaceID id) const;

    // Rasterizes into the cache. Returns pointer on success, nullptr on failure.
    bool rasterizeIntoCache(FaceID id, std::uint32_t codepoint);

    // Returns a stable pointer valid for the lifetime of this FontEngine,
    // or nullptr if the codepoint could not be rasterized.
    const GlyphMetadata* getGlyph(FaceID id, std::uint32_t codepoint);

private:
    struct FaceEntry {
        FT_Face face        = nullptr;
        bool    isEmoji     = false;
        int     ascender    = 0;
        int     descender   = 0;
        std::unordered_map<std::uint32_t, GlyphMetadata> glyphCache;
    };

    struct FontEntry {
        FaceID   faceID;
        FontObj  fontObj;
    };

    // Looks up an entry; throws FontEngineError if not found.
    FaceEntry& requireFace(FaceID id);
    const FaceEntry& requireFace(FaceID id) const;


    FT_Library ft_library = nullptr;
    std::unique_ptr<FontManager> font_manager;

    std::unordered_map<FaceID, FaceEntry>              faceCache;
    std::unordered_map<FontID, std::vector<FontEntry>> fontCache;
};

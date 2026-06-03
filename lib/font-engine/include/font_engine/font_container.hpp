#pragma once
#include <font_engine/FontFace.hpp>
#include <font_engine/font_utils.hpp>
#include <font_engine/TypedID.hpp>
#include <font_engine/font_engine.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

class FontManager;

class FontContainer {
public:
    FontContainer(FT_Library ft, FontManager* fm);
    ~FontContainer();

    FontContainer(const FontContainer&) = delete;
    FontContainer& operator=(const FontContainer&) = delete;
    FontContainer(FontContainer&&) = delete;
    FontContainer& operator=(FontContainer&&) = delete;

    void bindFont(const std::string& fontName);
    bool hasFace(FontWeight weight, FontStyle style) const;
    FontFace acquireFace(FontWeight weight, FontStyle style);
    const GlyphMetadata* getGlyph(const FontFace& face, int fontSize, uint32_t codepoint);
    void unbind();

private:
    struct WeightStylePair {
        FontWeight weight;
        FontStyle style;
        bool operator==(const WeightStylePair& o) const {
            return weight == o.weight && style == o.style;
        }
    };

    struct WeightStyleHash {
        size_t operator()(const WeightStylePair& p) const {
            return std::hash<int>{}(static_cast<int>(p.weight)) ^
                   (std::hash<int>{}(static_cast<int>(p.style)) << 1);
        }
    };

    struct SizeEntry {
        std::unordered_map<uint32_t, GlyphMetadata> glyphCache;
    };

    struct FaceSlot {
        FontObj fontObj;
        FaceID id;
        FT_Face face = nullptr;
        int ascender   = 0;
        int descender  = 0;
        int unitsPerEm = 0;
        int currentSize = 0;
        std::unordered_map<int, SizeEntry> sizeCache;
    };

    FaceSlot* getFaceSlot(const FontFace& face);
    void ensureFaceHasSize(FaceSlot& slot, int pixelSize);

    FT_Library m_ft_library;
    FontManager* m_font_manager;

    std::string m_boundFontName;
    std::vector<FontObj> m_fontFaces;
    std::unordered_map<FaceID, FaceSlot> m_slots;
    std::unordered_map<WeightStylePair, FontFace, WeightStyleHash> m_acquired;
    bool m_bound = false;
};

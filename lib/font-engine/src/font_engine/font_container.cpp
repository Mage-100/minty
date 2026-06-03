#include <font_engine/font_container.hpp>
#include <font_engine/FontFace.hpp>
#include <font_engine/font_utils.hpp>
#include "font_engine/internal/font_manager.hpp"

static int fixedToPixels(FT_Pos v) {
    return static_cast<int>(v >> 6);
}

FontContainer::FontContainer(FT_Library ft, FontManager* fm)
    : m_ft_library(ft), m_font_manager(fm) {}

FontContainer::~FontContainer() {
    for (auto& [id, slot] : m_slots) {
        if (slot.face)
            FT_Done_Face(slot.face);
    }
}

void FontContainer::bindFont(const std::string& fontName) {
    m_fontFaces = m_font_manager->get_font_by_name(fontName);
    m_boundFontName = fontName;
    m_bound = true;
}

bool FontContainer::hasFace(FontWeight weight, FontStyle style) const {
    if (!m_bound) return false;
    for (const auto& obj : m_fontFaces) {
        if (obj.weight == weight && obj.style == style)
            return true;
    }
    return false;
}

FontFace FontContainer::acquireFace(FontWeight weight, FontStyle style) {
    WeightStylePair key{weight, style};
    {
        auto existing = m_acquired.find(key);
        if (existing != m_acquired.end())
            return existing->second;
    }

    const FontObj* match = nullptr;
    for (const auto& obj : m_fontFaces) {
        if (obj.weight == weight && obj.style == style) {
            match = &obj;
            break;
        }
    }

    FT_Face ftFace = nullptr;
    FT_New_Face(m_ft_library, match->fontPath.c_str(), 0, &ftFace);

    FaceSlot slot;
    slot.fontObj    = *match;
    slot.id         = FaceID::make();
    slot.face       = ftFace;
    slot.ascender   = ftFace->ascender;
    slot.descender  = ftFace->descender;
    slot.unitsPerEm = ftFace->units_per_EM;

    FontFace result;
    result.id         = slot.id;
    result.source     = *match;
    result.ascender   = ftFace->ascender;
    result.descender  = ftFace->descender;
    result.unitsPerEm = ftFace->units_per_EM;
    result.valid      = true;

    m_slots.emplace(slot.id, std::move(slot));
    m_acquired.emplace(key, result);
    return result;
}

FontContainer::FaceSlot* FontContainer::getFaceSlot(const FontFace& face) {
    auto it = m_slots.find(face.id);
    return it != m_slots.end() ? &it->second : nullptr;
}

void FontContainer::ensureFaceHasSize(FaceSlot& slot, int pixelSize) {
    if (slot.currentSize != pixelSize) {
        FT_Set_Pixel_Sizes(slot.face, 0, pixelSize);
        slot.currentSize = pixelSize;
    }
}

const GlyphMetadata* FontContainer::getGlyph(const FontFace& face, int fontSize, uint32_t codepoint) {
    FaceSlot* slot = getFaceSlot(face);
    ensureFaceHasSize(*slot, fontSize);

    auto& sizeEntry = slot->sizeCache[fontSize];
    auto glyphIt = sizeEntry.glyphCache.find(codepoint);
    if (glyphIt != sizeEntry.glyphCache.end())
        return &glyphIt->second;

    FT_UInt glyphIndex = FT_Get_Char_Index(slot->face, codepoint);
    FT_Load_Glyph(slot->face, glyphIndex, FT_LOAD_RENDER);

    FT_Bitmap& bmp = slot->face->glyph->bitmap;
    int w = static_cast<int>(bmp.width);
    int h = static_cast<int>(bmp.rows);

    GlyphMetadata meta;
    meta.width    = w;
    meta.height   = h;
    meta.bearingX = slot->face->glyph->bitmap_left;
    meta.bearingY = slot->face->glyph->bitmap_top;
    meta.advanceX = fixedToPixels(slot->face->glyph->advance.x);
    meta.stride   = bmp.pitch;
    meta.pixels.resize(static_cast<size_t>(w * h), 0);

    for (int i = 0; i < w * h; i++)
        meta.pixels[i] = static_cast<uint32_t>(bmp.buffer[i]) << 24;

    auto [it, inserted] = sizeEntry.glyphCache.emplace(codepoint, std::move(meta));
    return &it->second;
}

void FontContainer::unbind() {
    for (auto& [id, slot] : m_slots) {
        if (slot.face)
            FT_Done_Face(slot.face);
    }
    m_slots.clear();
    m_fontFaces.clear();
    m_acquired.clear();
    m_boundFontName.clear();
    m_bound = false;
}

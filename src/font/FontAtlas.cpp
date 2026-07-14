#include <cstdint>
#include <string>
#include "FontAtlas.hpp"

FontAtlas::FontAtlas() {}
FontAtlas::~FontAtlas() {}

int FontAtlas::generate(int w, int h) {

    AtlasObject obj = {
        .width = w,
        .height = h,
        .stride = w,
        .maxGlyphHeight = 0,
        .atlas = std::vector<std::uint32_t>(w * h, 0xFF003300)
    };

    cache.insert({cache_counter, obj});

    return cache_counter++;
}

void FontAtlas::addGlyph(int atlasID, GlyphInfo info) {
    auto it = cache.find(atlasID);

    if (it == cache.end())
        throw "Atlas not created yet: " + std::to_string(atlasID);

    it->second.glyphs.push_back(info);
    m_addGlyphToAtlas(it->second);
}

void FontAtlas::m_addGlyphToAtlas(AtlasObject& obj) {
    GlyphInfo& glyphInfo = obj.glyphs.back();
    auto& bitmapBuffer = glyphInfo.bitmapBuffer;

    auto bitmapWidth = glyphInfo.bitmapWidth;
    auto bitmapHeight = glyphInfo.bitmapHeight;
    auto bitmapPitch = glyphInfo.bitmapPitch;

    auto& atlasBuffer = obj.atlas;

    auto& maxGlyphHeight = obj.maxGlyphHeight;
	if (bitmapHeight > maxGlyphHeight)
		maxGlyphHeight = bitmapHeight;

    auto& penX = obj.penx;
    auto& penY = obj.peny;

	if (penX + glyphInfo.advanceX > obj.width) {
		penX = 0;
		penY += maxGlyphHeight;
	}

    for (int y = 0; y < bitmapHeight; y++) {
        for (int x = 0; x < bitmapWidth; x++) {
            std::uint32_t grayLevelPixel = bitmapBuffer[x + y * bitmapPitch];
            std::uint32_t red   = grayLevelPixel << (8*0);
            std::uint32_t green = grayLevelPixel << (8*1);
            std::uint32_t blue  = grayLevelPixel << (8*2);
            std::uint32_t alpha =           0xFF << (8*3);

            std::uint32_t atlasPixel = alpha | blue | green | red;

			int index = (penX + x) + (penY + y) * obj.stride;
            atlasBuffer[index] = atlasPixel;
        }
    }

    penX += glyphInfo.advanceX;
}

const std::vector<std::uint32_t>& FontAtlas::getAtlas(int atlasID) {
    auto it = cache.find(atlasID);

    if (it == cache.end())
        throw "Atlas not created yet: " + std::to_string(atlasID);

    if (it->second.glyphs.size() == 0)
        throw "Glyphs not added yet: " + std::to_string(atlasID);

    return it->second.atlas;
}

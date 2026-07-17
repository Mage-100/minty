#include <ranges>
#include <stdexcept>
#include <array>
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
        .atlas_buffer = std::vector<std::uint32_t>(w * h, 0xFF003300)
    };

    cache.insert({cache_counter, obj});

    return cache_counter++;
}

std::unordered_map<int, FontAtlas::AtlasObject>::iterator FontAtlas::checkAtlas(int atlasID) {
    auto it = cache.find(atlasID);

    if (it == cache.end())
        throw std::runtime_error("Atlas not created yet: " + std::to_string(atlasID));

    return it;
}

void FontAtlas::addGlyph(int atlasID, GlyphInfo glyphInfo) {
    auto& atlasObj = checkAtlas(atlasID)->second;


    auto& bitmapBuffer = glyphInfo.bitmapBuffer;

    auto bitmapWidth = glyphInfo.bitmapWidth;
    auto bitmapHeight = glyphInfo.bitmapHeight;
    auto bitmapPitch = glyphInfo.bitmapPitch;

    auto& atlasBuffer = atlasObj.atlas_buffer;

    auto& maxGlyphHeight = atlasObj.maxGlyphHeight;
	if (bitmapHeight > maxGlyphHeight)
		maxGlyphHeight = bitmapHeight;

    auto& penX = atlasObj.penx;
    auto& penY = atlasObj.peny;

	if (penX + glyphInfo.advanceX > atlasObj.width) {
		penX = 0;
		penY += maxGlyphHeight;
	}

    FontAtlas::GlyphObject glyph_obj = {
        .posX = penX,
        .posY = penY,
        .codepoint = glyphInfo.codepoint
    };
    atlasObj.glyph_store.insert({ glyphInfo.codepoint, glyph_obj});

    for (int y = 0; y < bitmapHeight; y++) {
        for (int x = 0; x < bitmapWidth; x++) {
            std::uint32_t grayLevelPixel = bitmapBuffer[x + y * bitmapPitch];
            std::uint32_t red   = grayLevelPixel << (8*0);
            std::uint32_t green = grayLevelPixel << (8*1);
            std::uint32_t blue  = grayLevelPixel << (8*2);
            std::uint32_t alpha =           0xFF << (8*3);

            std::uint32_t atlasPixel = alpha | blue | green | red;

			int index = (penX + x) + (penY + y) * atlasObj.stride;
            atlasBuffer[index] = atlasPixel;
        }
    }

    penX += glyphInfo.advanceX;
}

const std::vector<std::uint32_t>& FontAtlas::getAtlas(int atlasID) {
    auto it = cache.find(atlasID);

    if (it == cache.end())
        throw "Atlas not created yet: " + std::to_string(atlasID);

    if (it->second.glyph_store.size() == 0)
        throw "Glyphs not added yet: " + std::to_string(atlasID);

    return it->second.atlas_buffer;
}

std::array<int, 2> FontAtlas::getGlyphPos(int atlasID, unsigned int codepoint) {
    auto it = checkAtlas(atlasID);

	auto& atlasObj = it->second;

	if (atlasObj.glyph_store.empty())
		throw "Glyphs not added yet: " + std::to_string(atlasID);

    auto glyphIt = atlasObj.glyph_store.find(codepoint);

	if (glyphIt == atlasObj.glyph_store.end()) {
		throw std::runtime_error("Glyph not found: Codepoint: " + std::to_string(codepoint));
	}

    auto& glyph = glyphIt->second;

    return { glyph.posX, glyph.posY };

}

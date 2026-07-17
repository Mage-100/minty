#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>
#include <stdexcept>
#include <array>

#include "Font.hpp"
#include "FontAtlas.hpp"
#include "FontLibrary.hpp"

FontLibrary::FontLibrary() : font(Font()), atlas(FontAtlas()) {}
FontLibrary::~FontLibrary() {};

std::size_t FontLibrary::addFontFromPath(const std::string& path, std::size_t font_size) {
	int f = font.addFontFromPath(path, font_size);

	int atlasW = font_size * 26;
	int atlasH = atlasW * 30;
	int a = atlas.generate(atlasW, atlasH);

    FontObject obj;
    obj.ID = std::make_pair(f, a);

	cache.insert({ _id, obj});

	return _id++;
}

void FontLibrary::addGlyph(std::size_t id, unsigned int codepoint) {
	auto fontObjIt = checkID(id);
	auto fontID = extractFontID(fontObjIt);
	auto atlasID = extractAtlasID(fontObjIt);
    
    auto& fontObj = fontObjIt->second;

    auto size = font.getGlyphSize(fontID, codepoint);
    auto bearing = font.getGlyphBearing(fontID, codepoint);
    auto bitmap = font.getGlyphBitmap(fontID, codepoint);
    auto bitmapBuffer = font.getGlyphBuffer(fontID, codepoint);

    FontAtlas::GlyphInfo info = {
		.codepoint    = codepoint,
        .glyphWidth   = size.width,
        .glyphHeight  = size.height,
        .advanceX     = font.getGlyphAdvanceX(fontID, codepoint),
        .bearingX     = bearing.x,
        .bearingY     = bearing.y,
        .ascender     = 0,
        .descender    = 0,
        .bitmapHeight = bitmap.height,
        .bitmapWidth  = bitmap.width,
        .bitmapPitch  = bitmap.pitch,
        .bitmapBuffer = bitmapBuffer
    };

    atlas.addGlyph(atlasID, info);
    fontObj.glyph_info.insert({ codepoint, info });
}

bool FontLibrary::hasGlyph(std::size_t id, unsigned int codepoint) {
    try {
        checkGlyph(id, codepoint);
        return true;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

	return false;
}

std::array<int, 2> FontLibrary::atlas_getGlyphPos(std::size_t id, unsigned int codepoint) {
    auto fontObjIt = checkID(id);
    auto f = extractFontID(fontObjIt);
    auto a = extractAtlasID(fontObjIt);

    checkGlyph(id, codepoint);
    return atlas.getGlyphPos(a, codepoint);

}

const std::unordered_map<std::size_t, FontLibrary::FontObject>::iterator 
    FontLibrary::checkID(std::size_t id) {

	auto it = cache.find(id);

	if (it == cache.end())
		throw std::runtime_error("Passed ID doesn't exist." + std::to_string(id));

	return it;
}

const FontAtlas::GlyphInfo& FontLibrary::checkGlyph(std::size_t id, unsigned int codepoint) {
    auto fontObjIt = checkID(id);
    auto& fontObj  = fontObjIt->second;

    auto glyphInfoIt = fontObj.glyph_info.find(codepoint);

    if (glyphInfoIt != fontObj.glyph_info.end()) {
        return glyphInfoIt->second;
    }
    else {
        throw std::runtime_error("Glyph not found: Codepoint: " + std::to_string(codepoint));
    }
}

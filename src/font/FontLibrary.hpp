#pragma once

#include <array>
#include <utility>
#include <unordered_map>

#include "Font.hpp"
#include "FontAtlas.hpp"

class FontLibrary {
	Font font;
	FontAtlas atlas;

	struct FontObject {
		int atlas_width = 0, atlas_height = 0, font_size;
		signed short int ascender = 0, descender = 0;
		unsigned short int units_per_em = 0;
		signed long int max_advance_x = 0;
		std::unordered_map<unsigned int, FontAtlas::GlyphInfo> glyph_info;
		std::pair<int, int> ID;

		// REMARK
		// Only works cause the underlying FontAtlas ensures the
		// the position of the glyph doesn't change within the atlas. 
		// Future api changes may break this method
		std::unordered_map<unsigned int, std::array<int, 2>> glyph_pos_cache;
	};

	std::size_t _id = 0;
	// std::pair<FontID, AtlasID>
	std::unordered_map<std::size_t, FontObject> cache;
	
public:
	FontLibrary();
	~FontLibrary();

	std::size_t addFontFromPath(const std::string& path, std::size_t font_size);

	signed short int getAscender(std::size_t id);
	float getAscenderPX(std::size_t id);

	signed short int getDescender(std::size_t id);
	float getDescenderPX(std::size_t id);

	unsigned short int getUnitsPerEm(std::size_t id);

	signed long int getMaxAdvanceX(std::size_t id);

	void addGlyph(std::size_t id, unsigned int codepoint);
	bool hasGlyph(std::size_t id, unsigned int codepoint);

	std::array<signed long int, 2> glyph_getDimension(std::size_t id, unsigned int codepoint);
	std::array<unsigned int, 2> glyph_getBitmapDimension(std::size_t id, unsigned int codepoint);
	std::array<signed long int, 2> glyph_getBearing(std::size_t id, unsigned int codepoint);

	std::array<int, 2> atlas_getGlyphPos(std::size_t id, unsigned int codepoint);
	std::array<int, 2> atlas_getDimension(std::size_t id);

	const std::vector<std::uint32_t>& atlas_getBuffer(std::size_t id);

private:
	const std::unordered_map<std::size_t, FontObject>::iterator checkID(std::size_t id);
	const FontAtlas::GlyphInfo& checkGlyph(std::size_t id, unsigned int codepoint);
	int extractFontID(const std::unordered_map<std::size_t, FontObject>::iterator it) const { return it->second.ID.first; }
	int extractAtlasID(const std::unordered_map<std::size_t, FontObject>::iterator it) const { return it->second.ID.second; }
};
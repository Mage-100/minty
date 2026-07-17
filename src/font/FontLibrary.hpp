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
		int atlas_width = 0, atlas_height = 0;
		std::unordered_map<unsigned int, FontAtlas::GlyphInfo> glyph_info;
		std::pair<int, int> ID;
	};

	std::size_t _id = 0;
	// std::pair<FontID, AtlasID>
	std::unordered_map<std::size_t, FontObject> cache;
public:
	FontLibrary();
	~FontLibrary();

	std::size_t addFontFromPath(const std::string& path, std::size_t font_size);

	void addGlyph(std::size_t id, unsigned int codepoint);
	bool hasGlyph(std::size_t id, unsigned int codepoint);

	std::array<signed long int, 2> glyph_getDimension(std::size_t id, unsigned int codepoint);

	std::array<int, 2> atlas_getGlyphPos(std::size_t id, unsigned int codepoint);
	std::array<int, 2> atlas_getDimension(std::size_t id);

	const std::vector<std::uint32_t>& atlas_getBuffer(std::size_t id);

private:
	const std::unordered_map<std::size_t, FontObject>::iterator checkID(std::size_t id);
	const FontAtlas::GlyphInfo& checkGlyph(std::size_t id, unsigned int codepoint);
	int extractFontID(const std::unordered_map<std::size_t, FontObject>::iterator it) const { return it->second.ID.first; }
	int extractAtlasID(const std::unordered_map<std::size_t, FontObject>::iterator it) const { return it->second.ID.second; }
};
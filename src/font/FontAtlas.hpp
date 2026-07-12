#pragma once
#include <vector>
#include <unordered_map>

class FontAtlas {
public:
    struct AddGlyphInfo {
        unsigned int codepoint;

        signed long int glyphWidth;
        signed long int glyphHeight;

        signed long int advanceX;

        signed long int bearingX;
        signed long int bearingY;

        signed short int ascender;
        signed short int descender;

        unsigned int bitmapHeight;
        unsigned int bitmapWidth;
        int bitmapPitch;

        unsigned char* bitmapBuffer;
    };
private:
    struct AtlasObject {
        int width;
        int height;
        int stride;
        int usedWidth = 0;
        int usedHeight = 0;
        int maxGlyphHeight;
        std::vector<AddGlyphInfo> glyphs = {};
        std::vector<std::uint32_t> atlas;
    };

    int cache_counter = 0;
    std::unordered_map<int, AtlasObject> cache;

    void m_addGlyphToAtlas(AtlasObject& obj);
public:
    FontAtlas();
    ~FontAtlas();

    int generate(int w, int h);
    void addGlyph(int atlasID, AddGlyphInfo info);

    const std::vector<std::uint32_t>& getAtlas(int atlasID);
};

#pragma once
#include <vector>
#include <unordered_map>

class FontAtlas {
public:
    struct GlyphInfo {
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

        int atlasPosX;
        int atlasPosY;

        unsigned char* bitmapBuffer;
    };
private:
    struct GlyphObject {
        int posX;
        int posY;
        unsigned int codepoint;
    };

    struct AtlasObject {
        int width;
        int height;
        int stride;
        int maxGlyphHeight = 0;
        int penx = 0;
        int peny = 0;
        std::unordered_map<unsigned int, GlyphObject> glyph_store = {};
        std::vector<std::uint32_t> atlas_buffer;
    };

    int cache_counter = 0;
    std::unordered_map<int, AtlasObject> cache;

    std::unordered_map<int, AtlasObject>::iterator checkAtlas(int atlasID);
public:
    FontAtlas();
    ~FontAtlas();

    int generate(int w, int h);
    void addGlyph(int atlasID, GlyphInfo info);

    const std::vector<std::uint32_t>& getAtlas(int atlasID);

    std::array<int, 2> getGlyphPos(int atlasID, unsigned int codepoint);
};

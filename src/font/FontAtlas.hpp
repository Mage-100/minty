#pragma once
#include <vector>
#include <unordered_map>

class FontAtlas {
    struct AtlasObject {
        int width;
        int height;
        int maxGlyphHeight;
        std::vector<unsigned char> buffer;
    };

    int cache_counter = 0;
    std::unordered_map<int, AtlasObject> cache;
public:
    FontAtlas();
    ~FontAtlas();

    int generate(int w, int h);
    void addGlyph(int atlasID, unsigned int codepoint, unsigned char* buffer);
};

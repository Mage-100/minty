#include <algorithm>
#include <array>
#include <vector>
#include <font_engine/font_engine.hpp>
#include <font_engine/font_utils.hpp>
#include <font_engine/internal/font_manager.hpp>
#include <gtest/gtest.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using font_list = std::vector<font_obj_t>;

TEST(FONT_MANAGER, GetFontFaceCount) {
    FontManager fontmanager;
    font_list list;
#ifdef _WIN32
    list = fontmanager.get_font_by_name("Consolas");
    EXPECT_TRUE(list.size() == 4) << "Font list for the font Consolas is empty";
#endif
}

TEST(FONT_MANAGER, GetFontByName) {
    FontManager fontmanager;
    font_list list;
#ifdef _WIN32
    list = fontmanager.get_font_by_name("Consolas");
    std::array<std::string, 4> expected = {
        "C:\\WINDOWS\\FONTS\\CONSOLA.TTF",
        "C:\\WINDOWS\\FONTS\\CONSOLAI.TTF",
        "C:\\WINDOWS\\FONTS\\CONSOLAB.TTF",
        "C:\\WINDOWS\\FONTS\\CONSOLAZ.TTF",
    };

    for (const std::string& path : expected) {
        bool found = std::any_of(list.begin(), list.end(), [&](const font_obj_t& obj) {
            return obj.fontPath == path;
        });
        EXPECT_TRUE(found);
    }
#endif
}

TEST(FONT_ENGINE, TestMonospaceFontFace) {
    FontEngine engine;
#ifdef _WIN32
    FaceID id1 = engine.loadFaceFromPath("C:\\WINDOWS\\FONTS\\CONSOLA.TTF", 20);
    ASSERT_TRUE(engine.isFaceMonospaced(id1));
    FaceID id2 = engine.loadFaceFromPath("C:\\WINDOWS\\FONTS\\ARIAL.TTF", 20);
    ASSERT_FALSE(engine.isFaceMonospaced(id2));
#endif
}

TEST(FONT_ENGINE, TestGlyphPixelBuffer) {
    FontEngine engine;
#ifdef _WIN32
    FaceID id = engine.loadFaceFromPath("C:\\WINDOWS\\FONTS\\CONSOLA.TTF", 100);
#endif
    ASSERT_TRUE(engine.isFaceMonospaced(id));
    engine.rasterize(id, 'A');

    GlyphMetadata* data1 = engine.getGlyph(id, 'A');
    stbi_write_png("test1.png", data1->glyph_width, data1->glyph_height, 4, data1->pixelBuffer.data(), data1->glyph_width * 4);

    GlyphMetadata* data2 = engine.getGlyph(id, 'Z');
    stbi_write_png("test2.png", data2->glyph_width, data2->glyph_height, 4, data2->pixelBuffer.data(), data2->glyph_width * 4);
}

TEST(FONT_ENGINE, TestEmoji) {
    FontEngine engine;
    FaceID id = engine.loadEmojiFontFromPath(COLOR_EMOJI_FONT_PATH);
    int result1 = engine.rasterize(id, 0x1F601);
    GlyphMetadata* data = engine.getGlyph(id, 0x1F601);
    int result2 = stbi_write_png("test3.png", data->glyph_width, data->glyph_height, 4, data->pixelBuffer.data(), data->glyph_width * 4);

    ASSERT_TRUE(result1 & result2);
}

TEST(FONT_ENGINE, CreateEmojiAtlas) {
    FontEngine engine;
    FaceID id = engine.loadEmojiFontFromPath(COLOR_EMOJI_FONT_PATH);
    int atlas_width = 4096;
    int atlas_height = 4096;

    std::vector<std::uint32_t> atlas;
    atlas.resize(atlas_width * atlas_height);

    int cursorX = 0;
    int cursorY = 0;
    int currRowHeight = 0;

    const std::vector<std::pair<int,int>> emojiRanges = {
        {0x1F600, 0x1F64F},
        {0x1F300, 0x1F5FF},
        {0x1F680, 0x1F6FF},
        {0x1F900, 0x1F9FF},
        {0x1FA00, 0x1FAFF},
        {0x2600,  0x26FF },
        {0x2700,  0x27BF },
        {0x1F1E0, 0x1F1FF},
        {0x1F200, 0x1F2FF},
        {0x1F000, 0x1F02F},
        {0x1F0A0, 0x1F0FF},
    };

    for (auto& [start, end] : emojiRanges) {
        for (int ch = start; ch <= end; ch++) {
            GlyphMetadata* data = engine.getGlyph(id, ch);
            if (data->glyph_width > atlas_width) continue;

            if (cursorX + data->glyph_width > atlas_width) {
                cursorX = 0;
                cursorY += currRowHeight;
                currRowHeight = 0;
            }

            if (data->glyph_height > currRowHeight) {
                currRowHeight = data->glyph_height;
            }

            if (cursorY + data->glyph_height > atlas_height) {
                atlas_height *= 2;
                atlas.resize(atlas_width * atlas_height, 0);
            }

            for (int y = 0; y < data->glyph_height; y++) {
                for (int x = 0; x < data->glyph_width; x++) {
                    int col = cursorX + x;
                    int row = cursorY + y;

                    atlas[col + row * atlas_width] = data->pixelBuffer[x + y * data->glyph_width];
                }
            }

            cursorX += data->glyph_width;
        }
    }


    stbi_write_png("emojiglyph.png", atlas_width, atlas_height, 4, atlas.data(), atlas_width * 4);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

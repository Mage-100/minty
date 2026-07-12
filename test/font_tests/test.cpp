#include <exception>
#include <filesystem>
#include <gtest/gtest.h>
#include <Font.hpp>
#include <FontAtlas.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using Path = std::filesystem::path;

static void addGlyphHelper(Font& font, int fontID, FontAtlas& atlas, int atlasID, unsigned int codepoint) {
    auto size = font.getGlyphSize(fontID, codepoint);
    auto bearing = font.getGlyphBearing(fontID, codepoint);
    auto bitmap = font.getGlyphBitmap(fontID, codepoint);
    auto bitmapBuffer = font.getGlyphBuffer(fontID, codepoint);

    FontAtlas::AddGlyphInfo info = {
        .glyphWidth = size.width,
        .glyphHeight = size.height,
        .advanceX = font.getGlyphAdvanceX(fontID, codepoint),
        .bearingX = bearing.x,
        .bearingY = bearing.y,
        .ascender = 0,
        .descender = 0,
        .bitmapHeight = bitmap.height,
        .bitmapWidth = bitmap.width,
        .bitmapPitch = bitmap.pitch,
        .bitmapBuffer = bitmapBuffer
    };

    atlas.addGlyph(atlasID, info);
}

TEST(FONT_TESTS, AddFontFromPath) {
    Path font_source = FONT_SOURCE;

    try {
        Font font;
        Path robotoFontRegular = font_source / "RobotoMono/RobotoMono-Regular.ttf";
        int f1 = font.addFontFromPath(robotoFontRegular.string(), 16);
        ASSERT_GE(f1, 0);
    } catch(const std::exception& e) {
        FAIL() << "std::exception: " << e.what();
    } catch(...) {
        FAIL() << "Unknown Exception";
    }

}

TEST(FONT_TESTS, GeneralTest) {
    Path font_source = FONT_SOURCE;

    try {
        Font font;
        Path robotoFontRegular = font_source / "RobotoMono/RobotoMono-Regular.ttf";
        int f1 = font.addFontFromPath(robotoFontRegular.string(), 200);

        FontAtlas atlas;
        int a1 = atlas.generate(500, 500);

        addGlyphHelper(font, f1, atlas, a1, 'A');

        auto& atlasBuffer = atlas.getAtlas(a1);
        int a = stbi_write_png("atlas_test.png", 500, 500, 4, atlasBuffer.data(), 500*4);
    } catch(const std::exception& e) {
        FAIL() << "std::exception: " << e.what();
    } catch(...) {
        FAIL() << "Unknown Exception";
    }

    ASSERT_TRUE(true);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

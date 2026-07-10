#include <exception>
#include <filesystem>
#include <gtest/gtest.h>
#include <Font.hpp>

using Path = std::filesystem::path;

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

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include <algorithm>
#include <array>
#include <vector>
#include <font_engine/font_engine.hpp>
#include <font_engine/font_utils.hpp>
#include <font_engine/internal/font_manager.hpp>
#include <gtest/gtest.h>

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


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

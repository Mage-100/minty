#include <font_engine/font_engine.hpp>
#include <gtest/gtest.h>

TEST(FONT_ENGINE_TEST, FONT_ENGINE) {
    ASSERT_EQ(2,3);
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

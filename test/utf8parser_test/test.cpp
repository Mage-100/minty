#include <utf8parser.hpp>
#include <gtest/gtest.h>

TEST(UTF8ParserTests, UTF8_1Byte_Test) {
    uint32_t test_bit;

    // 1-byte ASCII
    test_bit = UTF8Parser::encode(0x0041);
    EXPECT_EQ(test_bit, 0x41);       // 'A'
    test_bit = UTF8Parser::encode(0x007F);
    EXPECT_EQ(test_bit, 0x7F);       // DEL (boundary)
    test_bit = UTF8Parser::encode(0x0000);
    EXPECT_EQ(test_bit, 0x00);       // NULL
}

TEST(UTF8ParserTests, UTF8_2Byte_Test) {
    uint32_t test_bit;

    // 2-byte boundary cases
    test_bit = UTF8Parser::encode(0x0080);
    EXPECT_EQ(test_bit, 0xC280);     // first 2-byte codepoint
    test_bit = UTF8Parser::encode(0x07FF);
    EXPECT_EQ(test_bit, 0xDFBF);     // last 2-byte codepoint
    test_bit = UTF8Parser::encode(0x0115);
    EXPECT_EQ(test_bit, 0xC495);     // ĕ
    test_bit = UTF8Parser::encode(0x029A);
    EXPECT_EQ(test_bit, 0xCA9A);     // ʚ
    test_bit = UTF8Parser::encode(0x00A2);
    EXPECT_EQ(test_bit, 0xC2A2);     // ¢
    test_bit = UTF8Parser::encode(0x00E9);
    EXPECT_EQ(test_bit, 0xC3A9);     // é
}

TEST(UTF8ParserTests, UTF8_3Byte_Test) {
    uint32_t test_bit;

    // 3-byte boundary cases
    test_bit = UTF8Parser::encode(0x0800);
    EXPECT_EQ(test_bit, 0xE0A080);   // first 3-byte codepoint
    test_bit = UTF8Parser::encode(0xFFFF);
    EXPECT_EQ(test_bit, 0xEFBFBF);   // last 3-byte codepoint
    test_bit = UTF8Parser::encode(0x6841);
    EXPECT_EQ(test_bit, 0xE6A181);   // 栁
    test_bit = UTF8Parser::encode(0x20AC);
    EXPECT_EQ(test_bit, 0xE282AC);   // € (euro sign)
    test_bit = UTF8Parser::encode(0x4E2D);
    EXPECT_EQ(test_bit, 0xE4B8AD);   // 中
    test_bit = UTF8Parser::encode(0x3042);
    EXPECT_EQ(test_bit, 0xE38182);   // あ
    test_bit = UTF8Parser::encode(0xD7FF);
    EXPECT_EQ(test_bit, 0xED9FBF);   // just before surrogate range
}

TEST(UTF8ParserTests, UTF8_4Byte_Test) {
    uint32_t test_bit;

    // 4-byte boundary cases
    test_bit = UTF8Parser::encode(0x10000);
    EXPECT_EQ(test_bit, 0xF0908080); // first 4-byte codepoint
    test_bit = UTF8Parser::encode(0x10FFFF);
    EXPECT_EQ(test_bit, 0xF48FBFBF); // last valid unicode codepoint
    test_bit = UTF8Parser::encode(0x1F600);
    EXPECT_EQ(test_bit, 0xF09F9880); // 😀
    test_bit = UTF8Parser::encode(0x1F916);
    EXPECT_EQ(test_bit, 0xF09FA496); // 🤖
    test_bit = UTF8Parser::encode(0x1D11E);
    EXPECT_EQ(test_bit, 0xF09D849E); // 𝄞 (musical symbol)
    test_bit = UTF8Parser::encode(0x10348);
    EXPECT_EQ(test_bit, 0xF0908D88); // 𐍈 (gothic letter)
}

TEST(UTF8ParserTests, UTF8Decode_1Byte_Test) {
    uint32_t test_bit;
    test_bit = UTF8Parser::decode(0x41);
    EXPECT_EQ(test_bit, 0x0041);     // 'A'
    test_bit = UTF8Parser::decode(0x7F);
    EXPECT_EQ(test_bit, 0x007F);     // DEL (boundary)
    test_bit = UTF8Parser::decode(0x00);
    EXPECT_EQ(test_bit, 0x0000);     // NULL
}

TEST(UTF8ParserTests, UTF8Decode_2Byte_Test) {
    uint32_t test_bit;
    test_bit = UTF8Parser::decode(0xC280);
    EXPECT_EQ(test_bit, 0x0080);     // first 2-byte codepoint
    test_bit = UTF8Parser::decode(0xDFBF);
    EXPECT_EQ(test_bit, 0x07FF);     // last 2-byte codepoint
    test_bit = UTF8Parser::decode(0xC495);
    EXPECT_EQ(test_bit, 0x0115);     // ĕ
    test_bit = UTF8Parser::decode(0xCA9A);
    EXPECT_EQ(test_bit, 0x029A);     // ʚ
    test_bit = UTF8Parser::decode(0xC2A2);
    EXPECT_EQ(test_bit, 0x00A2);     // ¢
    test_bit = UTF8Parser::decode(0xC3A9);
    EXPECT_EQ(test_bit, 0x00E9);     // é
}

TEST(UTF8ParserTests, UTF8Decode_3Byte_Test) {
    uint32_t test_bit;
    test_bit = UTF8Parser::decode(0xE0A080);
    EXPECT_EQ(test_bit, 0x0800);     // first 3-byte codepoint
    test_bit = UTF8Parser::decode(0xEFBFBF);
    EXPECT_EQ(test_bit, 0xFFFF);     // last 3-byte codepoint
    test_bit = UTF8Parser::decode(0xE6A181);
    EXPECT_EQ(test_bit, 0x6841);     // 栁
    test_bit = UTF8Parser::decode(0xE282AC);
    EXPECT_EQ(test_bit, 0x20AC);     // € (euro sign)
    test_bit = UTF8Parser::decode(0xE4B8AD);
    EXPECT_EQ(test_bit, 0x4E2D);     // 中
    test_bit = UTF8Parser::decode(0xE38182);
    EXPECT_EQ(test_bit, 0x3042);     // あ
    test_bit = UTF8Parser::decode(0xED9FBF);
    EXPECT_EQ(test_bit, 0xD7FF);     // just before surrogate range
}

TEST(UTF8ParserTests, UTF8Decode_4Byte_Test) {
    uint32_t test_bit;
    test_bit = UTF8Parser::decode(0xF0908080);
    EXPECT_EQ(test_bit, 0x10000);    // first 4-byte codepoint
    test_bit = UTF8Parser::decode(0xF48FBFBF);
    EXPECT_EQ(test_bit, 0x10FFFF);   // last valid unicode codepoint
    test_bit = UTF8Parser::decode(0xF09F9880);
    EXPECT_EQ(test_bit, 0x1F600);    // 😀
    test_bit = UTF8Parser::decode(0xF09FA496);
    EXPECT_EQ(test_bit, 0x1F916);    // 🤖
    test_bit = UTF8Parser::decode(0xF09D849E);
    EXPECT_EQ(test_bit, 0x1D11E);    // 𝄞 (musical symbol)
    test_bit = UTF8Parser::decode(0xF0908D88);
    EXPECT_EQ(test_bit, 0x10348);    // 𐍈 (gothic letter)
}

TEST(UTF8ParserTests, UTF8_RoundTrip_Test) {
    uint32_t codepoints[] = {
        0x0041, 0x007F,           // 1-byte
        0x0080, 0x07FF, 0x00E9,   // 2-byte
        0x0800, 0xFFFF, 0x20AC,   // 3-byte
        0x10000, 0x1F600, 0x10FFFF // 4-byte
    };
    for (uint32_t cp : codepoints) {
        EXPECT_EQ(UTF8Parser::decode(UTF8Parser::encode(cp)), cp);
    }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include "utf8parser.hpp"
#include <cstdint>

uint32_t UTF8Parser::encode(unsigned int codepoint) {
    uint32_t result = 0;

    if (codepoint <= 0x007F) { // 1 Byte
        return codepoint;
    } else if (codepoint <= 0x07FF) { // 2 Byte
        result |= (0x3 << (1 * 8 + 6));
        result |= (0x2 << (0 * 8 + 6));
        result |= (codepoint & 0x3F) | ((codepoint & 0x7C0) << 2);
    } else if (codepoint <= 0xFFFF) { // 3 Byte
        result |= (0x7 << (2 * 8 + 5));
        result |= (0x2 << (1 * 8 + 6));
        result |= (0x2 << (0 * 8 + 6));
        result |= (codepoint & 0x3F) | ((codepoint & 0xFC0) << 2) | ((codepoint & 0xF000) << 4);
    } else if (codepoint <= 0x10FFFF) { // 4 Byte
        result |= (0xF << (3 * 8 + 4));
        result |= (0x2 << (2 * 8 + 6));
        result |= (0x2 << (1 * 8 + 6));
        result |= (0x2 << (0 * 8 + 6));
        result |= (codepoint & 0x3F) | ((codepoint & 0xFC0) << 2) | ((codepoint & 0x3F000) << 4) | ((codepoint & 0x1C0000) << 6);
    }

    return result;
}

uint32_t UTF8Parser::decode(uint32_t bytes) {
    uint32_t result = 0;

    if ((bytes & 0x80) == 0) { // 1 Byte
        return bytes;
    } else if ((bytes & 0xE000) == 0xC000) { // 2 Byte
        result |= (bytes & 0x3F);
        result |= ((bytes & 0x1F00) >> 2);
    } else if ((bytes & 0xF00000) == 0xE00000) { // 3 Byte
        result |= (bytes & 0x3F);
        result |= ((bytes & 0x3F00) >> 2);
        result |= ((bytes & 0xF0000) >> 4);
    } else if ((bytes & 0xF8000000) == 0xF0000000) { // 4 Byte
        result |= (bytes & 0x3F);
        result |= ((bytes & 0x3F00) >> 2);
        result |= ((bytes & 0x3F0000) >> 4);
        result |= ((bytes & 0x7000000) >> 6);
    }

    return result;
}

#pragma once
#include <cstdint>

class UTF8Parser {
public:
    uint32_t encode(unsigned int);
    uint32_t decode(uint32_t);
    void decode();
};

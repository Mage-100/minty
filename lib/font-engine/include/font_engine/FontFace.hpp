#pragma once
#include <font_engine/TypedID.hpp>
#include <font_engine/font_utils.hpp>
#include <string>

struct FontFace {
    FaceID    id;
    FontObj   source;

    int ascender   = 0;
    int descender  = 0;
    int unitsPerEm = 0;
    bool valid     = false;
};

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

class FontEngine {
private:
    FT_Library library;
public:
    FontEngine();
    ~FontEngine();

    void loadTTF();
};

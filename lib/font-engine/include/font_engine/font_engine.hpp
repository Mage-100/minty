#pragma once
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

class FontManager;

class FontEngine {
private:
    FT_Library library;
    std::unique_ptr<FontManager> fontmanager;
public:
    FontEngine();
    ~FontEngine();

    void loadTTF();
};

#include <iostream>
#include <font_engine/font_engine.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <harfbuzz/hb-ft.h>

FontEngine::FontEngine() {
    int error;
    error = FT_Init_FreeType(&library);
    if (error) {
        std::cerr << __FILE__ << ":" << __LINE__ << std::endl;
        std::cerr << "Error: Failed to initialize Freetype Library" << std::endl;
        exit(EXIT_FAILURE);
    }
}



FontEngine::~FontEngine() {

}

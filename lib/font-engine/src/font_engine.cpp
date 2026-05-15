#include <iostream>
#include <memory>

#include <font_engine/font_engine.hpp>
#include <font_engine/font_utils.hpp>
#include "font_engine/internal/font_manager.hpp"

#include <ft2build.h>
#include <winerror.h>
#include FT_FREETYPE_H

#include <harfbuzz/hb-ft.h>

#include <dwrite.h>

FontEngine::FontEngine()
    : fontmanager(std::make_unique<FontManager>()) {
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

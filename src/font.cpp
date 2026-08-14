#include <unordered_map>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

// #include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

#include "font.hpp"

Font::Font(const char* font_path, unsigned int size) : font_size(size), texture_buffer(std::vector<unsigned char>(texture_width * texture_height)) {
    error = FT_Init_FreeType(&library);
    if (error) {
        std::cerr << "Error: Failed to initialize Freetype Library" << std::endl;
        exit(EXIT_FAILURE);
    }

    error = FT_New_Face(library, font_path, 0, &face);
    if (error == FT_Err_Unknown_File_Format) {
        std::cerr << "Error: Unknown Font File Format" << std::endl;
        exit(EXIT_FAILURE);
    } else if (error) {
        std::cerr << "Error: Failed to open Font File" << std::endl;
        exit(EXIT_FAILURE);
    }

    error = FT_Set_Pixel_Sizes(face, font_size, 0);
    if (error) {
        std::cerr << "Error: Failed to set pixel size" << std::endl;
        exit(EXIT_FAILURE);
    }

    hb_font_t *hb_font = hb_ft_font_create_referenced(face);
    hb_ft_font_set_funcs(hb_font);

    ascender = face->size->metrics.ascender >> 6;
}

void Font::init_fonts() {
    int cursorX                     = padding;
    int cursorY                     = padding;
    const int row_height            = face->size->metrics.height >> 6;
    int glyph_metadata_index = 0;

    for (int c = 32; c <= 127; c++) {
        error = FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT);
        if(error) {
            std::cerr << "Failed to load char: " << c << std::endl;
            continue;
        }

        const int glyph_width  = face->glyph->bitmap.width;
        const int glyph_height = face->glyph->bitmap.rows;
        const int bearingX              = face->glyph->bitmap_left;
        const int bearingY              = face->glyph->bitmap_top;
        const int advanceX              = face->glyph->advance.x >> 6;
        const int baselineOffset        = ascender - bearingY;

        int above = face->glyph->bitmap_top;
        int below = (int)face->glyph->bitmap.rows - above;

        if (above > max_above_baseline) max_above_baseline = above;
        if (below > max_below_baseline) max_below_baseline = below;

        if (cursorX + padding + glyph_width > texture_width) {
            cursorX = padding;
            cursorY += row_height + padding;
        }

        for (int row = 0; row < face->glyph->bitmap.rows; row++) {
            for (int col = 0; col < face->glyph->bitmap.width; col++) {
                int x = cursorX + col;
                int y = cursorY + row;
                unsigned char pixel = face->glyph->bitmap.buffer[col + row * face->glyph->bitmap.pitch];
                if (pixel > 0) {
                    texture_buffer[x + y * texture_width] = pixel;
                } else {
                    texture_buffer[x + y * texture_width] = 0;
                }
            }
        }

        GlyphInfo glyph_info = {
            glyph_width,
            glyph_height,
            bearingX,
            bearingY,
            advanceX,
            glm::vec4(0, 0, 0, 0),
            cursorX,
            cursorY,
            padding
        };

        glyph_cache.insert({c, glyph_info});


        GlyphMetadata md;
        md.atlas_u   = (float)cursorX / texture_width;
        md.atlas_v   = (float)cursorY / texture_height;
        md.atlas_w   = (float)glyph_width  / texture_width;
        md.atlas_h   = (float)glyph_height / texture_height;
        md.bearing_x = (float)face->glyph->bitmap_left;
        md.bearing_y = (float)face->glyph->bitmap_top;   // positive = above baseline
        md.bitmap_w  = (float)glyph_width;
        md.bitmap_h  = (float)glyph_height;

        int glyph_index = glyph_metadata.size();
        glyph_metadata.push_back(md);
        glyph_to_index[c] = glyph_index;

        cursorX += glyph_width + padding;
    }
}

const std::vector<GlyphMetadata>& Font::getGlyphMetadata() {
    return glyph_metadata;
}

int Font::getGlyphIndex(char c) const {
    auto it = glyph_to_index.find(c);
    if (it != glyph_to_index.end()) {
        std::cerr << "Failed to fetch glyph index for: " << c << std::endl;
        return it->second;
    }
    return glyph_to_index.at('?');
}

float Font::get_advanceX() {
    return glyph_cache.at('M').advanceX;
}

float Font::get_ascender() {
    return face->size->metrics.ascender >> 6;
}

float Font::get_descender() {
    return face->size->metrics.descender >> 6;
}

float Font::get_line_height() {
    return face->size->metrics.height >> 6;
}

int Font::get_max_above_baseline() {
    return max_above_baseline;
}

int Font::get_max_below_baseline() {
    return max_below_baseline;
}

int Font::get_cell_width() {
    return (int)get_advanceX();
}

int Font::get_cell_height() {
    int asc = face->size->metrics.ascender >> 6;
    int desc = -(face->size->metrics.descender >> 6);
    return asc + desc;
}

void Font::generate_texture() {
    glad_glGenTextures(1, &textureID);
    glad_glBindTexture(GL_TEXTURE_2D, textureID);
    glad_glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        texture_width,
        texture_height,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        texture_buffer.data()
    );

    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}



unsigned int Font::get_textureID() {
    return textureID;
}

std::array<int, 2> Font::get_textureDimensions() {
    return {texture_width, texture_height};
}

GlyphInfo Font::get_glyph_info(char c) {
return glyph_cache.find(c)->second;
}

void Font::debug_metrics() {
    // These are the ONLY values we trust for layout
    int face_ascender  =  face->size->metrics.ascender  >> 6;
    int face_descender = -(face->size->metrics.descender >> 6); // positive
    int face_height    =  face->size->metrics.height    >> 6;   // includes leading

    printf("=== Font Metrics ===\n");
    printf("face ascender  : %d px\n", face_ascender);
    printf("face descender : %d px\n", face_descender);
    printf("face height    : %d px (has leading = %d px)\n",
            face_height, face_height - face_ascender - face_descender);
    printf("max_above_baseline (ink): %d px\n", max_above_baseline);
    printf("max_below_baseline (ink): %d px\n", max_below_baseline);
    printf("advance_x ('M')         : %d px\n", (int)get_advanceX());
    printf("cell_width  = advance_x = %d px\n", (int)get_advanceX());
    printf("cell_height = ascender + descender = %d px\n",
            face_ascender + face_descender);
    printf("====================\n");
}

void Font::destroy() {
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

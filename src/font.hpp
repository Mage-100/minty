#ifndef FONT_H
#define FONT_H

#include <unordered_map>
#include <array>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <shader.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

// #include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

typedef struct GlyphInfo {
    int width;
    int height;
    int bearingX;
    int bearingY;
    int advanceX;
    glm::vec4 uv;
    int cursorX;
    int cursorY;
    int padding;
} GlyphInfo;

struct GlyphMetadata {
    // Slot 0 — UV in atlas (normalized 0..1)
    float atlas_u;       // left edge of glyph in atlas
    float atlas_v;       // top edge of glyph in atlas
    float atlas_w;       // width of glyph region in atlas
    float atlas_h;       // height of glyph region in atlas

    // Slot 1 — Glyph metrics in PIXELS
    float bearing_x;     // pixels right from cell left edge to ink left edge
    float bearing_y;     // pixels up from baseline to ink top edge (positive = above)
    float bitmap_w;      // ink width in pixels
    float bitmap_h;      // ink height in pixels
};


class Font {
private:
    float ascender;
    int max_above_baseline = 0;
    int max_below_baseline = 0;
    int font_size;
    int error;

    FT_Library library;
    FT_Face face;

    int texture_width  = 1024;
    int texture_height = 1024;

    const int padding  = 2;

    unsigned int textureID;
    std::unordered_map<unsigned int, GlyphInfo> glyph_cache;
    std::unordered_map<int, int> glyph_to_index;
    std::vector<GlyphMetadata> glyph_metadata;
    std::vector<unsigned char> texture_buffer;
public:
    Font(const char *, unsigned int);
    void init_fonts();

    const std::vector<GlyphMetadata>& getGlyphMetadata();

    int getGlyphIndex(char c) const;

    float get_advanceX();

    float get_ascender();
    float get_descender();
    float get_line_height();
    int get_max_above_baseline();
    int get_max_below_baseline();
    int get_cell_width();
    int get_cell_height();
    void generate_texture();
    unsigned int get_textureID();
    std::array<int, 2> get_textureDimensions();
    GlyphInfo get_glyph_info(char c);
    void debug_metrics();
    void destroy();
};

#endif // FONT_H

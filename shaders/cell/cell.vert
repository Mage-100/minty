#version 330 core
layout(location = 0) in vec2 aPos; // unit quad: 0..1 in x and y

out vec2 v_uv;
out vec2 v_local;   // position within glyph bitmap (0..1), for masking
out vec4 v_fg;
out vec4 v_bg;

uniform vec2  screen_size;   // framebuffer in pixels
uniform vec2  grid_size;     // cols, rows
uniform vec2  cell_size_px;  // cell width, height in pixels
uniform float cell_ascender; // pixels from cell top to baseline
uniform usampler2D cell_data;
uniform sampler1D  metadata_tex;

uniform int pass;

void main() {
    int col = gl_InstanceID % int(grid_size.x);
    int row = gl_InstanceID / int(grid_size.x);

    uvec4 cell = texelFetch(cell_data, ivec2(col, row), 0);
    uint glyph_id = cell.r;

    vec4 uv_slot   = texelFetch(metadata_tex, int(glyph_id) * 2,     0);
    vec4 px_slot   = texelFetch(metadata_tex, int(glyph_id) * 2 + 1, 0);
    // uv_slot  = (atlas_u, atlas_v, atlas_w, atlas_h)
    // px_slot  = (bearing_x, bearing_y, bitmap_w, bitmap_h)

    float bearing_x = px_slot.x;
    float bearing_y = px_slot.y;  // positive = above baseline
    float bitmap_w  = px_slot.z;
    float bitmap_h  = px_slot.w;

    // Center the whole grid on screen
    float grid_w = grid_size.x * cell_size_px.x;
    float grid_h = grid_size.y * cell_size_px.y;
    vec2 grid_origin = (screen_size - vec2(grid_w, grid_h)) * 0.5;

    vec2 cell_origin_px = grid_origin + vec2(col, row) * cell_size_px;

    // --- 5. Glyph ink top-left in pixels ---
    // baseline is at cell_origin_px.y + cell_ascender
    // ink top  is at baseline - bearing_y
    float baseline_y   = cell_origin_px.y + cell_ascender;
    float ink_top_px   = baseline_y - bearing_y;
    float ink_left_px  = cell_origin_px.x + bearing_x;

    // --- 6. This vertex's pixel position (quad over the ink region only) ---
    vec2 ink_origin_px = vec2(ink_left_px, ink_top_px);
    vec2 pos_px = ink_origin_px + aPos * vec2(bitmap_w, bitmap_h);

    if (pass == 0) {
        pos_px = cell_origin_px + aPos * cell_size_px;
    }
    // --- 7. Pixel → NDC ---
    vec2 ndc = (pos_px / screen_size) * 2.0 - 1.0;
    gl_Position = vec4(ndc.x, -ndc.y, 0.0, 1.0);

    // --- 8. UV (aPos maps directly to atlas region) ---
    v_uv = uv_slot.xy + aPos * uv_slot.zw;
    v_fg = unpackUnorm4x8(cell.g);
    v_bg = unpackUnorm4x8(cell.b);
    // v_local for debug border
    v_local = aPos;
}

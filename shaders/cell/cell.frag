#version 330 core
in vec2 v_uv;
in vec2 v_local;
in vec4 v_fg;
in vec4 v_bg;

out vec4 FragColor;

uniform sampler2D atlas_tex;
uniform int pass;

void main() {
    // Debug: red border around each glyph quad
    // if (v_local.x < 0.02 || v_local.y < 0.02 ||
    //     v_local.x > 0.98 || v_local.y > 0.98) {
    //     FragColor = vec4(1, 0, 0, 1);
    //     return;
    // }

    if (pass == 0) {
        if (v_bg.a < 0.01) discard;
        FragColor = v_bg;
    } else {
        float alpha = texture(atlas_tex, v_uv).r;
        FragColor = vec4(v_fg.rgb,  alpha); // white glyph for now
    }
}

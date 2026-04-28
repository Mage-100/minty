#version 330 core
in  vec2 v_uv;
out vec4 FragColor;

uniform float u_time;
uniform int   u_num_cells;

void main() {
    vec3 colorA = vec3(0.635, 0.894, 0.721);
    // vec3 colorB = vec3(0.118, 0.565, 1.000);
    vec3 colorB = vec3(0.694, 0.737, 0.333);
    // vec3 colorC = vec3(0.0, 0.298, 0.329);
    vec3 colorC = vec3(0.118, 0.565, 1.000);

    float cell_uv     = 1.0 / float(u_num_cells);
    float angle_start = 1.0 - cell_uv;
    float cutoff      = angle_start + v_uv.y * cell_uv;

    if (v_uv.x > cutoff) discard;

    float edge_alpha = 1.0 - smoothstep(cutoff - 0.008, cutoff, v_uv.x);

    float t0 = fract(v_uv.x + u_time * 0.4);
    float t1 = fract(v_uv.x + u_time * 0.4 + 0.333);
    float t2 = fract(v_uv.x + u_time * 0.4 + 0.666);

    float w0 = exp(-pow(t0 - 0.5, 2.0) / 0.04);
    float w1 = exp(-pow(t1 - 0.5, 2.0) / 0.04);
    float w2 = exp(-pow(t2 - 0.5, 2.0) / 0.04);

    float total = w0 + w1 + w2 + 0.001;
    vec3 color  = (colorA * w0 + colorB * w1 + colorC * w2) / total;
    color = mix(vec3(0.04, 0.01, 0.05), color, 0.85);

    FragColor = vec4(color, edge_alpha);
}

#shader(vertex)
#version 330 core
layout (location = 0) in vec3 offset;

vec3 vertex[6] = vec3[6](
	vec3( 0.0,  0.0, 0.0), // Top-Left
	vec3( 1.0,  0.0, 0.0), // Top-Right
	vec3( 1.0,  1.0, 0.0), // Bottom-Right

	vec3( 1.0,  1.0, 0.0), // Bottom-Right
	vec3( 0.0,  1.0, 0.0), // Bottom-Left
	vec3( 0.0,  0.0, 0.0)  // Top-Left
);

vec2 texCoords[6] = vec2[6](
	vec2(0.0, 1.0),	 // Top-Left
	vec2(1.0, 1.0),	 // Top-Right
	vec2(1.0, 0.0),	 // Bottom-Right

	vec2(1.0, 0.0),	 // Bottom-Right
	vec2(0.0, 0.0),	 // Bottom-Left
	vec2(0.0, 1.0)   // Top-Left
);

out vec2 TexCoord;
out vec2 uv;
flat out int test;

uniform float padding_x;
uniform float padding_y;
uniform vec2 cell_size;
uniform mat4 projection;

void main() {
	vec3 quad = vertex[gl_VertexID] * vec3(cell_size, 1.0);
	quad += offset;
	quad += vec3(padding_x, padding_y, 0.0);

	gl_Position = projection * vec4(quad, 1.0);
	TexCoord = texCoords[gl_VertexID];
	uv = vertex[gl_VertexID].xy;

	if (gl_InstanceID == 10) test = 1;
	else test = 0;
}

#shader(fragment)
#version 330 core

out vec4 FragColor;
in vec2 TexCoord;
in vec2 uv;
flat in int test;

uniform sampler2D texture;
uniform vec2 glyph_size;
uniform float pen_x;
uniform float pen_y;
uniform vec2 cell_size;
uniform vec2 bearing;
uniform float ascender;

void main() {
	float top_offset = (ascender - bearing.y) / float(cell_size.y);
	float bottom_offset = top_offset + (glyph_size.y / cell_size.y);

	float left_offset = float(bearing.x) / cell_size.x;
	float right_offset = left_offset + float(glyph_size.x / cell_size.x);
	
	vec2 pen = vec2(pen_x, pen_y);

	if (test == 1) {
		bool condition_left_right = uv.x > left_offset && uv.x < right_offset;
		bool condition_top_bottom = uv.y > top_offset && uv.y < bottom_offset;
		if (condition_left_right && condition_top_bottom) {
			vec2 local = (uv - vec2(left_offset, top_offset)) * cell_size;
			vec4 texel = texelFetch(texture, ivec2(pen) + ivec2(local), 0);
			
			if (texel.xyz == vec3(0.0)) {
				texel = vec4(vec3(0.2), 1.0);
			}

			FragColor = texel;
		} else {
			// FragColor = vec4(1.0, 0.0, 0.0, 1.0);
			FragColor = vec4(vec3(0.2), 1.0);
		}
	} else {
		FragColor = vec4(vec3(0.2), 1.0);
	}

}
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
uniform vec2 cellDimension;
uniform mat4 projection;

void main() {
	vec3 quad = vertex[gl_VertexID] * vec3(cellDimension, 1.0);
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
uniform vec2 atlas_size;
uniform vec2 glyph_size;

void main() {
	atlas_size;
	
	if (test == 1) {
		// FragColor = vec4(vec3(1.0), 1.0);
		FragColor = texelFetch(texture, ivec2(uv * glyph_size), 0);
	} else {
		FragColor = vec4(vec3(0.2), 1.0);
	}

}
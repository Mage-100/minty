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
}

#shader(fragment)
#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

void main() {
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>


#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glad/glad.h>
#include <Window.hpp>
#include <VertexBuffer.hpp>
#include <VertexArray.hpp>
#include <VertexBufferLayout.hpp>
#include <Shader.hpp>
#include <Texture2D.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <Font.hpp>
#include <FontAtlas.hpp>
#include <FontLibrary.hpp>

using Path = std::filesystem::path;

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;


struct Pos {
	float x;
	float y;
	float z;
};

struct Pen {
	float x;
	float y;
};

struct Size {
	float w;
	float h;
};

struct Bearing {
	float x;
	float y;
};

struct Vertex {
	Pos pos;
	Pen pen;
	Size size;
	Bearing bearing;
};

int main() {
	Path fontSource = std::filesystem::path(FONT_SOURCE);
	Path fontPath = fontSource / "RobotoMono" / "RobotoMono-Regular.ttf";

	float font_size = 60;
	FontLibrary font_lib;
	auto id = font_lib.addFontFromPath(fontPath.string(), font_size); 
	font_lib.addGlyph(id, 'A');
	font_lib.addGlyph(id, 'B');
	font_lib.addGlyph(id, 'C');
	font_lib.addGlyph(id, 'D');
	font_lib.addGlyph(id, 'E');
	font_lib.addGlyph(id, 'i');
	font_lib.addGlyph(id, 'g');
	font_lib.addGlyph(id, 'q');

	unsigned int codepoint = 0x03B2;
	std::cout << "Codepoint: '" << static_cast<char>(codepoint) << "'" << std::endl;
	font_lib.addGlyph(id, codepoint);
	
	auto ascender_px = font_lib.getAscenderPX(id);
	auto descender_px = font_lib.getDescenderPX(id);
	std::cout << "Ascender(px): " << ascender_px << std::endl;
	std::cout << "Descender(px): " << descender_px << std::endl;

	auto max_advance_x = font_lib.getMaxAdvanceX(id);
	std::cout << "Max Advance X: " << max_advance_x << std::endl;

	int pen_x = -1, pen_y = -1;
	if (font_lib.hasGlyph(id, codepoint)) {
		auto [x, y] = font_lib.atlas_getGlyphPos(id, codepoint);
		std::cout << "X: " << x << " Y: " << y << std::endl;
		pen_x = x;
		pen_y = y;
	}
	auto [glyphW, glyphH] = font_lib.glyph_getDimension(id, codepoint);
	auto [bitmapW, bitmapH] = font_lib.glyph_getBitmapDimension(id, codepoint);
	auto [bearingX, bearingY] = font_lib.glyph_getBearing(id, codepoint);

	std::cout << "Glyph Width: " << glyphW << std::endl;
	std::cout << "Glyph Height: " << glyphH << std::endl;
	std::cout << "Bitmap Width: " << bitmapW << std::endl;
	std::cout << "Bitmap Height: " << bitmapH << std::endl;
	std::cout << "Bearing X: " << bearingX << std::endl;
	std::cout << "Bearing Y: " << bearingY << std::endl;
	std::cout << "Top Offset: " << ascender_px - bearingY << std::endl;

	float cell_width = max_advance_x;
	float cell_height = ascender_px - descender_px;

	std::cout << "Cell Width: " << cell_width << std::endl;
	std::cout << "Cell Height: " << cell_height << std::endl;

	float cols = 20;
	float padding_x = 5;
	float gap_x = 2;
	float WINDOW_WIDTH = (cell_width * cols) + (2*padding_x) + (gap_x * (cols-1));

	float rows = 8;
	float padding_y = 5;
	float gap_y = 2;
	float WINDOW_HEIGHT = (cell_height * rows) + (2 * padding_y) + (gap_y * (rows - 1)); 

	int fb_width = 0;
	int fb_height = 0;

	Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Font Render Test");

	window.onFramebufferSizeCallback([&fb_width, &fb_height](int w, int h) {
		glad_glViewport(0.0f, 0.0f, w, h);

		if (w == 0 || h == 0) return;
		fb_width = w;
		fb_height = h;
	});

	window.getFramebufferSize(&fb_width, &fb_height);

	std::filesystem::path shaderSource = SHADER_SOURCE;
	auto shaderPath = shaderSource / "shader.glsl";

	Shader s(shaderPath.string());


	std::vector<Vertex> vertices;

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			unsigned int code = 'q';
			auto [gW, gH] = font_lib.glyph_getDimension(id, code);
			auto [pW, pH] = font_lib.atlas_getGlyphPos(id, code);
			auto [bX, bY] = font_lib.glyph_getBearing(id, code);

			Vertex v;
			Pos p = {
				static_cast<float>(x * (cell_width + gap_x)),
				static_cast<float>(y * (cell_height + gap_y)),
				0.0f
			};

			Pen pen = {
				(float)pW, (float)pH
			};

			Size size{
				(float)gW, (float)gH
			};

			Bearing bearing{
				(float)bX, (float)bY
			};

			v.pos = p;
			v.pen = pen;
			v.size = size;
			v.bearing = bearing;

			vertices.push_back(v);
		}
	}

	VertexBuffer vb(vertices.data(), vertices.size() * sizeof(Vertex));
	
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(2);
	layout.Push<float>(2);

	VertexArray va;
	va.AddBuffer(vb, layout);

	glad_glVertexAttribDivisor(0, 1);
	glad_glVertexAttribDivisor(1, 1);
	glad_glVertexAttribDivisor(2, 1);
	glad_glVertexAttribDivisor(3, 1);

	auto [atlasW, atlasH] = font_lib.atlas_getDimension(id);
	const void* atlasBuffer = font_lib.atlas_getBuffer(id).data();
	int a = stbi_write_png("font_render_test.png", atlasW, atlasH, 4, atlasBuffer, atlasW * 4);
	Texture2D glyph_atlas(atlasW, atlasH, const_cast<void*>(atlasBuffer));

	float top_offset = (ascender_px - bearingY) / (float)(cell_height);
	float left_offset = (float)bearingX / cell_width;

	float bottom_offset = top_offset + (glyphH / (float)cell_height);
	float right_offset = left_offset + (glyphW / (float)cell_width);

	std::cout << "Top Offset: " << top_offset << std::endl;
	std::cout << "Left Offset: " << left_offset << std::endl;
	std::cout << "Bottom Offset: " << bottom_offset << std::endl;
	std::cout << "Right Offset: " << right_offset << std::endl;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!window.shouldWindowClose()) {
		glfwWaitEvents();
		glad_glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glad_glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 projection = glm::ortho(
			0.0f, static_cast<float>(fb_width),
			static_cast<float>(fb_height), 0.0f,
			-1.0f, 1.0f
		);

		va.Bind();
		s.use();
		s.setFloat("padding_x", padding_x);
		s.setFloat("padding_y", padding_y);
		s.setMat4("projection", projection);
		s.setVec2("cell_size", glm::vec2(cell_width, cell_height));
		s.setFloat("ascender", ascender_px);
		glad_glDrawArraysInstanced(GL_TRIANGLES, 0, 6, cols * rows);

		window.flush();
	}

	return 0;
}
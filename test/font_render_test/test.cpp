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

#include <Font.hpp>
#include <FontAtlas.hpp>

using Path = std::filesystem::path;

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;


struct Pos {
	float x;
	float y;
	float z;
};

std::array<Pos, 6> CreateQuad(float x, float y, float w, float h) {
	return { {
		    x,     y, 0.0f,
		x + w,     y, 0.0f,
		x + w, y + h, 0.0f,

		x + w, y + h, 0.0f,
		    x, y + h, 0.0f,
		    x,     y, 0.0f,
	} };
}

static void addGlyphHelper(Font& font, int fontID, FontAtlas& atlas, int atlasID, unsigned int codepoint) {
    auto size = font.getGlyphSize(fontID, codepoint);
    auto bearing = font.getGlyphBearing(fontID, codepoint);
    auto bitmap = font.getGlyphBitmap(fontID, codepoint);
    auto bitmapBuffer = font.getGlyphBuffer(fontID, codepoint);

    FontAtlas::GlyphInfo info = {
        .glyphWidth = size.width,
        .glyphHeight = size.height,
        .advanceX = font.getGlyphAdvanceX(fontID, codepoint),
        .bearingX = bearing.x,
        .bearingY = bearing.y,
        .ascender = 0,
        .descender = 0,
        .bitmapHeight = bitmap.height,
        .bitmapWidth = bitmap.width,
        .bitmapPitch = bitmap.pitch,
        .bitmapBuffer = bitmapBuffer
    };

    atlas.addGlyph(atlasID, info);
}

int main() {
	Path fontSource = std::filesystem::path(FONT_SOURCE);
	Path fontPath = fontSource / "RobotoMono" / "RobotoMono-Regular.ttf";

	Font font;
	int font_size = 16;
	int f1 = font.addFontFromPath(fontPath.string(), font_size);

	FontAtlas atlas;
	int atlasW = font_size * 26;
	int atlasH = atlasW;
	int a1 = atlas.generate(atlasW, atlasH);


	int cols = 50;
	int padding_x = 5;
	int gap_x = 2;
	int WINDOW_WIDTH = (font_size * cols) + (2*padding_x) + (gap_x * (cols-1));

	int row_height = font_size * 1.5;
	int rows = 25;
	int padding_y = 5;
	int gap_y = 2;
	int WINDOW_HEIGHT = (row_height * rows) + (2 * padding_y) + (gap_y * (rows - 1)); 
	
	int fb_width = 0;
	int fb_height = 0;

	int cell_width = font_size;
	int cell_height = row_height;

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


	std::vector<Pos> vertices;

	//auto q1 = CreateQuad(2, 2, 100, 100);

	//vertices.insert(vertices.end(), q1.begin(), q1.end());

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			Pos p = {
				static_cast<float>(x * (cell_width + gap_x)),
				static_cast<float>(y * (cell_height + gap_y)),
				0.0f
			};

			vertices.push_back(p);
		}
	}

	VertexBuffer vb(vertices.data(), vertices.size() * sizeof(Pos));
	
	VertexBufferLayout layout;
	layout.Push<float>(3);

	VertexArray va;
	va.AddBuffer(vb, layout);

	glad_glVertexAttribDivisor(0, 1);


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
		s.setVec2("cellDimension", glm::vec2(cell_width, cell_height));
		s.setMat4("projection", projection);
		//glad_glDrawArrays(GL_TRIANGLES, 0, 6);
		glad_glDrawArraysInstanced(GL_TRIANGLES, 0, 6, cols * rows);

		window.flush();
	}

	return 0;
}
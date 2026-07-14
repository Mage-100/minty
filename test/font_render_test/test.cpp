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

int main() {
	int font_size = 16;

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
	vertices.reserve(6 * rows * cols);

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			auto q = CreateQuad(
				padding_x + (x * (gap_x + font_size)),
				padding_y + (y * (gap_y + row_height)),
				font_size, row_height);
			
			vertices.insert(vertices.end(), q.begin(), q.end());
		}
	}

	VertexBuffer vb(vertices.data(), vertices.size() * sizeof(Pos));
	
	VertexBufferLayout layout;
	layout.Push<float>(3);

	VertexArray va;
	va.AddBuffer(vb, layout);
	

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
		s.setMat4("projection", projection);
		glad_glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));

		window.flush();
	}

	return 0;
}
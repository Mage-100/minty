#include <filesystem>
#include <glad/glad.h>
#include <Window.hpp>
#include <VertexBuffer.hpp>
#include <VertexArray.hpp>
#include <VertexBufferLayout.hpp>
#include <Shader.hpp>

int main() {
	Window window("Font Render Test");

	std::filesystem::path shaderSource = SHADER_SOURCE;
	auto shaderPath = shaderSource / "shader.glsl";

	Shader s(shaderPath.string());

	float vertices[] = {
		-0.5f, 0.5f, 0.0f, // Top-Left
		0.5f, 0.5f, 0.0f, // Top-Right
		0.5f, -0.5f, 0.0f, // Bottom-Right

		0.5f, -0.5f, 0.0f, // Bottom-Right
		-0.5f, -0.5f, 0.0f, // Bottom-Left
		-0.5f, 0.5f, 0.0f, // Top-Left
	};

	VertexBuffer vb(vertices, sizeof(vertices));
	
	VertexBufferLayout layout;
	layout.Push<float>(3);

	VertexArray va;
	va.AddBuffer(vb, layout);
	

	while (!window.shouldWindowClose()) {
		glad_glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glad_glClear(GL_COLOR_BUFFER_BIT);

		va.Bind();
		s.use();
		glad_glDrawArrays(GL_TRIANGLES, 0, 6);

		window.flush();
	}

	return 0;
}
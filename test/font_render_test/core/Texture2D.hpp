#pragma once
#include <glad/glad.h>

class Texture2D {
	unsigned int _id;
	int width = 0;
	int height = 0;
public:
	Texture2D() = delete;

	Texture2D(int width, int height, void* data);
	~Texture2D();

	void bind();
	void active(GLenum Texture);
};
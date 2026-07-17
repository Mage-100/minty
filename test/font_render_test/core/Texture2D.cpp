#include <stdexcept>

#include <glad/glad.h>

#include "Texture2D.hpp"

Texture2D::Texture2D(int w, int h, void* data) : width(w), height(h) {
	if (w <= 0 || h <= 0) {
		throw std::runtime_error("Width or height must be greater than 0");
	}
	
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

Texture2D::~Texture2D() {
	glDeleteTextures(1, &_id);
}

void Texture2D::bind() {
	glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture2D::active(GLenum texture) {
	glActiveTexture(texture);
}

#include "VertexBuffer.hpp"
#include <glad/glad.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    glad_glGenBuffers(1, &m_id);
    glad_glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glad_glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    glad_glDeleteBuffers(1, &m_id);
}

void VertexBuffer::Bind() const {
    glad_glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::Unbind() {
    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
}

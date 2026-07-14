#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray {
private:
    unsigned int m_id;
public:
    VertexArray();
    ~VertexArray();

    void Bind();
    void Unbind();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};

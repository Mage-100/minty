#include "VertexBufferLayout.hpp"
#include "glad/glad.h"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

VertexArray::VertexArray() {
    glad_glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray() {
    glad_glDeleteVertexArrays(1, &m_id);
}

void VertexArray::Bind() {
    glad_glBindVertexArray(m_id);
}

void VertexArray::Unbind() {
    glad_glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {

    Bind();
    vb.Bind();

    const auto& elements = layout.GetElements();
    unsigned int offset = 0;

    for (int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];

        glad_glEnableVertexAttribArray(i);
        glad_glVertexAttribPointer(
            i,
            element.count,
            element.type,
            element.normalized,
            layout.GetStride(),
            (void*)offset
        );

        offset += element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
    }

}

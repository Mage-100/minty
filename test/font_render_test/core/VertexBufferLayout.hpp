#pragma once
#include <vector>
#include <glad/glad.h>
#include <cassert>

struct VertexBufferLayoutElement {
    unsigned int count;
    unsigned int type;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type) {
        switch(type) {
            case GL_FLOAT:         return 4;
            case GL_UNSIGNED_INT:  return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        assert(false);
        return 0;
    }
};

class VertexBufferLayout {
private:
    unsigned int index;
    unsigned int size;

    std::vector<VertexBufferLayoutElement> m_Elements;
    unsigned int m_Stride;
public:

    VertexBufferLayout(): m_Stride(0){};
    ~VertexBufferLayout() = default;

    template<typename T>
    void Push(unsigned int count) {
        static_assert(false);
    }

    template<>
    void Push<float>(unsigned int count) {
        m_Elements.push_back({count, GL_FLOAT, GL_FALSE});
        m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_FLOAT);
    }

    inline const std::vector<VertexBufferLayoutElement> GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const {return m_Stride;}
};

#include "pch.h"

#include <shader.hpp>
#include "mty_renderer.hpp"
#include "mty_state.hpp"

using namespace MintyRenderer;

void OpenGLCellRenderer::generateGridTexture() {
    if (gridTexID == 0) {
        glad_glGenTextures(1, &gridTexID);
    }

    glad_glBindTexture(GL_TEXTURE_2D, gridTexID);
    glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32UI, cols, rows, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, nullptr);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void OpenGLCellRenderer::updateGridTexture() {
    glad_glBindTexture(GL_TEXTURE_2D, gridTexID);
    // glad_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, cols, rows, GL_RGBA_INTEGER, GL_UNSIGNED_INT, state->getCellBuffer().data());
    glad_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, cols, rows, GL_RGBA_INTEGER, GL_UNSIGNED_INT, state->getCellGrid()->data());
}

OpenGLCellRenderer::OpenGLCellRenderer(MintyState* s, std::unique_ptr<Font> &f)
    : state(s), font(f), cols(s->getCols()), rows(s->getRows()) {

    cellProgram = std::make_unique<Shader>("D:/Azwad/home/programming/minty/shaders/cell/cell.vert", "D:/Azwad/home/programming/minty/shaders/cell/cell.frag");

    float quad_vertices[] = {
        // x,    y
        0.0f, 0.0f, // Bottom-left
        1.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, // Top-right

        0.0f, 0.0f, // Bottom-left
        1.0f, 1.0f, // Top-right
        0.0f, 1.0f  // Top-left

    };

    // VBO and VAO for a single cell
    glad_glGenVertexArrays(1, &quadVAO);
    glad_glGenBuffers(1, &quadVBO);
    glad_glBindVertexArray(quadVAO);

    glad_glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    glad_glEnableVertexAttribArray(0);
    glad_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
    glad_glBindVertexArray(0);

    // Generating the grid as a texture for the GPU
    generateGridTexture();
    updateGridTexture();
    state->setCellBufferClean();

    // Generating the glyph metadata as a texture for the GPU
    glad_glGenTextures(1, &glyphMetadataTexID);
    glad_glBindTexture(GL_TEXTURE_1D, glyphMetadataTexID);

    glad_glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, font->getGlyphMetadata().size() * 2, 0, GL_RGBA, GL_FLOAT, font->getGlyphMetadata().data());
    glad_glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    framebuffer_w = state->getFramebufferWidth();
    framebuffer_h = state->getFramebufferHeight();
}

void OpenGLCellRenderer::render() {

    cellProgram->use();
    cellProgram->setVec2("screen_size", glm::vec2(framebuffer_w, framebuffer_h));
    cellProgram->setVec2("grid_size", cols, rows);
    cellProgram->setVec2("cell_size_px", font->get_cell_width(), font->get_cell_height());
    cellProgram->setFloat("cell_ascender", font->get_ascender());

    glad_glActiveTexture(GL_TEXTURE0);
    glad_glBindTexture(GL_TEXTURE_2D, font->get_textureID());
    cellProgram->setInt("atlas_tex", 0);

    glad_glActiveTexture(GL_TEXTURE1);
    glad_glBindTexture(GL_TEXTURE_2D, gridTexID);
    cellProgram->setInt("cell_data", 1);

    glad_glActiveTexture(GL_TEXTURE2);
    glad_glBindTexture(GL_TEXTURE_1D, glyphMetadataTexID);
    cellProgram->setInt("metadata_tex", 2);

    glad_glBindVertexArray(quadVAO);

    cellProgram->setInt("pass", 0);
    glad_glDrawArraysInstanced(GL_TRIANGLES, 0, 6, cols * rows);

    glad_glEnable(GL_BLEND);
    glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cellProgram->setInt("pass", 1);
    glad_glDrawArraysInstanced(GL_TRIANGLES, 0, 6, cols * rows);
    glad_glDisable(GL_BLEND);

    glad_glBindVertexArray(0);

}

void OpenGLCellRenderer::update() {
    if (true == state->getCellBufferStatus()) {
        cols = state->getCols();
        rows = state->getRows();
        framebuffer_w = state->getFramebufferWidth();
        framebuffer_h = state->getFramebufferHeight();
        generateGridTexture();
        updateGridTexture();
        state->setCellBufferClean();
    }
}

OpenGLCellRenderer::~OpenGLCellRenderer() {

}

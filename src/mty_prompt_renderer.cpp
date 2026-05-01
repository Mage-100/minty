
#include "mty_state.hpp"
#include "shader.hpp"
#include <memory>
#include "mty_prompt_renderer.hpp"

PromptRenderer::PromptRenderer(MintyState* s) : state(s) {
    cell_width = state->getCellWidth();
    cell_height = state->getCellHeight();

    shader = std::make_unique<Shader>("D:/Azwad/home/programming/minty/shaders/prompt/prompt.vert", "D:/Azwad/home/programming/minty/shaders/prompt/prompt.frag");
    float quad_vertices[] = {
        // x,    y
        0.0f, 0.0f, // Top-left
        1.0f, 0.0f, // Top-right
        1.0f, 1.0f, // bottom-right

        0.0f, 0.0f, // Top-left
        1.0f, 1.0f, // bottom-right
        0.0f, 1.0f  // bottom-left

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

    last_time = glfwGetTime();
}

void PromptRenderer::calculateCellGridSize() {
    cell_grid_width = state->getCols() * cell_width;
    cell_grid_height = state->getRows() * cell_height;
    cell_grid_top_x = (fb_width - cell_grid_width) / 2.0;
    cell_grid_top_y = (fb_height - cell_grid_height) / 2.0;
}

void PromptRenderer::setFramebufferSize(int w, int h) {
    fb_width = w;
    fb_height = h;
    calculateCellGridSize();
}

void PromptRenderer::resize(int w, int h) {
    fb_width = w;
    fb_height = h;
    calculateCellGridSize();
}

void PromptRenderer::setPromptStart(int col, int row) {
    prompt_start_col = col;
    prompt_start_row = row;
    has_prompt = (prompt_end_col >= 0);
}

void PromptRenderer::setPromptEnd(int col, int row) {
    prompt_end_col = col;
    prompt_end_row = row;
    has_prompt = (prompt_start_col >= 0);
}

void PromptRenderer::render() {

    if (!has_prompt || fb_width == 0) return;

       double now = glfwGetTime();
       elapsed += static_cast<float>(now - last_time);
       last_time = now;

       // Convert cell coords to pixel coords
       // (assuming single-row prompt for now — easy to extend to multi-row)
       float grid_origin_x = (fb_width  - static_cast<float>(state->getCols() * cell_width))  / 2.0f;
       float grid_origin_y = (fb_height - static_cast<float>(state->getRows() * cell_height)) / 2.0f;

       float x      = grid_origin_x + static_cast<float>(prompt_start_col * cell_width);
       float y      = grid_origin_y + static_cast<float>(prompt_start_row * cell_height);
       float width  = static_cast<float>((prompt_end_col - prompt_start_col) * cell_width);
       float height = static_cast<float>(cell_height);

       shader->use();
       shader->setVec2("u_pos",    glm::vec2(x, y));
       shader->setVec2("u_size",   glm::vec2(width, height));
       shader->setVec2("u_screen", glm::vec2(fb_width, fb_height));
       shader->setFloat("u_time",  elapsed);

       // Teal-to-purple gradient — tweak to taste
       shader->setVec4("u_color_left",  glm::vec4(0.85f, 0.078f, 0.0039f, 1.0f));
       shader->setVec4("u_color_right", glm::vec4(0.046f, 0.218f, 0.390f, 1.0f));

       int num_cells = prompt_end_col - prompt_start_col;
       shader->setInt("u_num_cells", num_cells);

       glad_glEnable(GL_BLEND);
       glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       glad_glBindVertexArray(quadVAO);
       glad_glDrawArrays(GL_TRIANGLES, 0, 6);
       glad_glBindVertexArray(0);
       glad_glDisable(GL_BLEND);
}

PromptRenderer::~PromptRenderer() {

}

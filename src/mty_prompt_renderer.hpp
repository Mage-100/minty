#pragma once
#include "mty_state.hpp"
#include "shader.hpp"

class PromptRenderer {
private:
    MintyState* state;

    unsigned int quadVAO, quadVBO;
    std::unique_ptr<Shader> shader;

    int fb_width, fb_height;
    int cell_width, cell_height;
    float cell_grid_width, cell_grid_height;
    float cell_grid_top_x, cell_grid_top_y;

    int prompt_start_col = -1, prompt_start_row = -1;
    int prompt_end_col   = -1, prompt_end_row   = -1;

    float elapsed = 0.0f;
    double last_time = 0.0;

    bool has_prompt = false;

    void calculateCellGridSize();
public:
    PromptRenderer(MintyState*);
    ~PromptRenderer();

    void setFramebufferSize(int, int);
    void resize(int, int);

    void setPromptStart(int, int);
    void setPromptEnd(int, int);

    void render();
};

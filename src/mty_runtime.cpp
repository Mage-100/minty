#include "pch.h"

#include "font.hpp"
#include "mty_state.hpp"
#include "mty_window.hpp"
#include "mty_runtime.hpp"
#include "mty_renderer.hpp"
#include <chrono>
#include <iostream>
#include <thread>

MintyRuntime::MintyRuntime(MintyState* s) : state(s) {
    font = std::make_unique<Font>("C:\\windows\\FONTS\\Consola.ttf", 30);
    font->init_fonts();
    state->setFont(font.get());
    window = std::make_unique<OpenGLWindow>(state);
    font->generate_texture();

    int fb_w, fb_h;
    window->getFramebufferSize(fb_w, fb_h);
    state->setFramebufferSize(fb_w, fb_h);
    state->initCellBuffer();

    window->setFramebufferCallback([this](int w, int h) {
        state->setFramebufferSize(w, h);
        std::cout << "cols: " << state->getCols() << ", rows: " << state->getRows() << std::endl;
        state->resizeCellBuffer();
    });

    cellRenderer =
        std::make_unique<MintyRenderer::OpenGLCellRenderer>(state, font);
}

void MintyRuntime::start() {
    while(window->shouldRun()) {
        glad_glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        cellRenderer->update();
        cellRenderer->render();
        window->pollEvents();
        window->swapBuffers();


    }
}

MintyRuntime::~MintyRuntime() {

}

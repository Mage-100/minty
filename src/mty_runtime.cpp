#include "mty_prompt_renderer.hpp"
#include "pch.h"
#include "GLFW/glfw3.h"

#include "font.hpp"
#include "mty_state.hpp"
#include "mty_window.hpp"
#include "mty_runtime.hpp"
#include "mty_renderer.hpp"
#include "threadsafequeue.hpp"
#include "vtcallbacks.hpp"
#include "vtparser.hpp"
#include <iostream>
#include <memory>

MintyRuntime::MintyRuntime(ThreadSafeQueue<std::vector<char>>* q, MintyState* s, VTParser* v) : queue(q), state(s), vtparser(v) {
    font = std::make_unique<Font>("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 25);
    font->init_fonts();
    state->setFont(font.get());
    window = std::make_unique<OpenGLWindow>(state);
    promptRenderer = std::make_unique<PromptRenderer>(state);
    font->generate_texture();

    window->setFramebufferCallback([this](int w, int h) {
        state->setFramebufferSize(w, h);
        std::cout << "cols: " << state->getCols() << ", rows: " << state->getRows() << std::endl;
        state->resizeCellBuffer();
        if (ptyResizeCallback) {
            ptyResizeCallback(state->getCols(), state->getRows());
        }
        auto cell_grid = state->getCellGrid();
        cell_grid->resize(state->getCols(), state->getRows());
        vtparser->resize(state->getCols(), state->getRows());
        promptRenderer->resize(w, h);
    });

    window->setKeyInputCallback([this](int key, int scancode, int action, int mods) {
        parseKeyWrite(key, scancode, action, mods);
        if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
            std::cout << "UP" << std::endl;
            auto cell_grid = state->getCellGrid();
            cell_grid->scroll_up(1);
            state->setCellBufferDirty();
        }
    });

    window->setTextInputCallback([this](unsigned int codepoint) {
        parseTextWrite(codepoint);
    });

}

void MintyRuntime::parseKeyWrite(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_ENTER) {
            if (ptyWriteCallback) {
                ptyWriteCallback("\r");
            }
        }

        if (key == GLFW_KEY_BACKSPACE) {
            if (ptyWriteCallback) {
                ptyWriteCallback("\x7f");
                glfwPostEmptyEvent();
            }
        }
    }
}

void MintyRuntime::parseTextWrite(unsigned int codepoint) {
    std::string s(1, static_cast<char>(codepoint));
    ptyWriteCallback(s);
}

void MintyRuntime::hookParserCallbacks() {
    VTCallbacks cb;

    cb.onPrintChar = [this](int col, int row, unsigned int ch) {
        auto cell_grid = state->getCellGrid();
        cell_grid->printChar(col, row, ch);
        state->setCellBufferDirty();
    };

    cb.onScrollUp = [this](int lines) {
        auto cell_grid = state->getCellGrid();
        cell_grid->scroll_up(lines);
        state->setCellBufferDirty();
    };

    cb.onEraseEntireScreen = [this](void) {
        auto cell_grid = state->getCellGrid();
        cell_grid->eraseEntireScreen();
        state->setCellBufferDirty();
    };

    cb.onEraseLine = [this](int col, int row, int param) {
        auto cell_grid = state->getCellGrid();
        cell_grid->eraseLine(col, row, param);
        state->setCellBufferDirty();
    };

    cb.onPromptStart = [this](int col, int row) {
        std::cout << "Start" << std::endl;
        promptRenderer->setPromptStart(col, row);
    };

    cb.onPromptEnd = [this](int col, int row) {
        std::cout << "End" << std::endl;
        promptRenderer->setPromptEnd(col, row);
    };

    vtparser->setCallbacks(std::move(cb));
}

void MintyRuntime::initInitials() {
    int fb_w, fb_h;
    window->getFramebufferSize(fb_w, fb_h);
    state->setFramebufferSize(fb_w, fb_h);
    if (ptyResizeCallback) {
        ptyResizeCallback(state->getCols(), state->getRows());
    }
    state->initCellBuffer();
    state->initCellGrid();
    vtparser->resize(state->getCols(), state->getRows());
    promptRenderer->setFramebufferSize(fb_w, fb_h);

    cellRenderer =
        std::make_unique<MintyRenderer::OpenGLCellRenderer>(state, font);
    hookParserCallbacks();
}

void MintyRuntime::start() {
    initInitials();
    while(window->shouldRun()) {
        glad_glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        std::vector<char> buf;
        while (queue->try_pop(buf)) {
            vtparser->feed(buf);
        }

        cellRenderer->update();
        promptRenderer->render();
        cellRenderer->render();
        window->pollEvents();
        window->swapBuffers();
    }
}

void MintyRuntime::setPTYWriteCallback(std::function<void(const std::string&)>&& cb) {
    ptyWriteCallback = cb;
}

void MintyRuntime::setPTYResizeCallback(std::function<void(int, int)>&& cb) {
    ptyResizeCallback = cb;
}

MintyRuntime::~MintyRuntime() {

}

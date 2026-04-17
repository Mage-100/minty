#pragma once
#include <memory>
#include "font.hpp"
#include "mty_renderer.hpp"
#include "mty_window.hpp"

class MintyState;

class MintyRuntime {
private:
    MintyState* state;
    std::unique_ptr<IWindow> window;
    std::unique_ptr<Font> font;
    std::unique_ptr<MintyRenderer::ICellRenderer> cellRenderer;
public:
    MintyRuntime(MintyState*);
    ~MintyRuntime();

    void start();
};

#pragma once
#include <functional>
#include <memory>
#include "font.hpp"
#include "mty_prompt_renderer.hpp"
#include "mty_renderer.hpp"
#include "mty_window.hpp"
#include "threadsafequeue.hpp"
#include "vtparser.hpp"

class MintyState;

class MintyRuntime {
private:
    MintyState* state;
    VTParser* vtparser;
    ThreadSafeQueue<std::vector<char>>* queue;
    std::unique_ptr<IWindow> window;
    std::unique_ptr<Font> font;
    std::unique_ptr<MintyRenderer::ICellRenderer> cellRenderer;
    std::unique_ptr<PromptRenderer> promptRenderer;

    void initInitials();

    void parseKeyWrite(int, int, int, int);
    void parseTextWrite(unsigned int);
    std::function<void(const std::string&)> ptyWriteCallback;
    std::function<void(int, int)> ptyResizeCallback;
public:
    MintyRuntime(ThreadSafeQueue<std::vector<char>>*, MintyState*, VTParser*);
    ~MintyRuntime();

    void start();
    void hookParserCallbacks();
    void setPTYWriteCallback(std::function<void(const std::string&)>&&);
    void setPTYResizeCallback(std::function<void(int, int)>&&);
};

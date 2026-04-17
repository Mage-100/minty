#pragma once
#include "mty_state.hpp"
#include <GLFW/glfw3.h>
#include <functional>
#include <memory>

class IWindow {
public:
    virtual ~IWindow() = default;
    virtual void pollEvents() = 0;
    virtual void swapBuffers() = 0;
    virtual bool shouldRun() = 0;

    virtual void setFramebufferCallback(std::function<void(int, int)> cb) = 0;
    virtual void getFramebufferSize(int &, int &) = 0;
};

class OpenGLWindow: public IWindow {
private:
    int window_width, window_height;
    GLFWwindow *window;

    MintyState* state;

    std::function<void(int, int)> framebuffer_cb;
public:
    OpenGLWindow(MintyState*);
    ~OpenGLWindow();

    bool shouldRun() override;
    void pollEvents() override;
    void swapBuffers() override;
    void setFramebufferCallback(std::function<void (int, int)> cb) override;
    void getFramebufferSize(int &, int &) override;
};

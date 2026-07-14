#pragma once
#include <functional>
#include <GLFW/glfw3.h>

class Window {
    GLFWwindow *window;

    int width = 800;
    int height = 600;

public:
    Window(const char* windowName);
    Window(int w, int h, const char* windowName);

    ~Window();

    GLFWwindow* getGLFWhandle() { return window; };
    void getFramebufferSize(int* w, int* h);

    bool shouldWindowClose();
    void flush();

    void onFramebufferSizeCallback(std::function<void(int width, int height)> cb);
    void onCursorPosCallback(std::function<void(double xpos, double ypos)> cb);

private:
    std::function<void(int w, int h)> _framebufferSizeCallback;
    std::function<void(double xpos, double ypos)> _cursorPosCallback;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
};

#include <iostream>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"

Window::Window(const char* windowName) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, windowName, NULL, NULL);

    if (window == NULL) {
        std::cerr << "Error: Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error: Failed to initialize OpenGL Context" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, this->framebufferSizeCallback);
    glfwSetCursorPosCallback(window, this->cursorPosCallback);
}

Window::Window(int w, int h, const char* windowName) : width(w), height(h) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, windowName, NULL, NULL);

    if (window == NULL) {
        std::cerr << "Error: Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error: Failed to initialize OpenGL Context" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, this->framebufferSizeCallback);
    glfwSetCursorPosCallback(window, this->cursorPosCallback);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::getFramebufferSize(int* w, int* h) {
    glfwGetFramebufferSize(window, w, h);
}

bool Window::shouldWindowClose() {
    return glfwWindowShouldClose(window);
}

void Window::flush() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::onFramebufferSizeCallback(std::function<void(int width, int height)> cb) {
    _framebufferSizeCallback = cb;
}

void Window::onCursorPosCallback(std::function<void(double xpos, double ypos)> cb) {
    _cursorPosCallback = cb;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
     auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));

     if (self && self->_framebufferSizeCallback) {
         self->_framebufferSizeCallback(width, height);
     }
}

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
     auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));

     if (self && self->_cursorPosCallback) {
         self->_cursorPosCallback(xpos, ypos);
     }
}

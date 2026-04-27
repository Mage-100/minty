#include "pch.h"

#include <functional>
#include <iostream>
#include "mty_state.hpp"
#include "mty_window.hpp"

OpenGLWindow::OpenGLWindow(MintyState* s) : state(s) {
    window_width = state->getWindowWidth();
    window_height = state->getWindowHeight();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(window_width, window_height, "Minty", NULL, NULL);

    if (this->window == NULL) {
        std::cerr << "Error: Failed to create GLFW Window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* w, int width, int height) {
        auto self = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(w));
        self->state->setFramebufferSize(width, height);
        glViewport(0, 0, width, height);
        if (self->framebuffer_cb) {
            self->framebuffer_cb(width, height);
        }
    });

    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        auto self = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(w));
        if (self->keyinput_cb) {
            self->keyinput_cb(key, scancode, action, mods);
        }
    });

    glfwSetCharCallback(window, [](GLFWwindow* w, unsigned int codepoint) {
        auto self = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(w));
        if (self->textinput_cb) {
            self->textinput_cb(codepoint);
        }
    });

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        std::cerr << "Error: Failed to initialize OpenGL Context" << std::endl;
        glfwTerminate();
    }
}

void OpenGLWindow::pollEvents() {
    glfwPollEvents();
}

void OpenGLWindow::swapBuffers() {
    glfwSwapBuffers(window);
}

bool OpenGLWindow::shouldRun() {
    return !glfwWindowShouldClose(window);
}

void OpenGLWindow::setFramebufferCallback(std::function<void(int, int)> cb) {
    framebuffer_cb = cb;
}

void OpenGLWindow::setKeyInputCallback(std::function<void(int, int, int, int)> cb) {
    keyinput_cb = cb;
}

void OpenGLWindow::setTextInputCallback(std::function<void(unsigned int)> cb) {
    textinput_cb = cb;
}

void OpenGLWindow::getFramebufferSize(int &w, int &h) {
    glfwGetFramebufferSize(window, &w, &h);
}

OpenGLWindow::~OpenGLWindow() {

}

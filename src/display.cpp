#include "display.h"

#include <cstdio>

Display& Display::get()
{
    static Display d;
    return d;
}

Display::Display()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(m_width, m_height, "Trayracer", NULL, NULL);

    if (!m_window) {
        printf("Failed to create window\n");
    }

    glfwMakeContextCurrent(m_window);

    glfwSwapInterval(0);

    glViewport(0, 0, m_width, m_height);
}

void Display::close()
{
    glfwDestroyWindow(m_window);
}

void Display::swapBuffers()
{
    glfwSwapBuffers(m_window);
}

bool Display::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}

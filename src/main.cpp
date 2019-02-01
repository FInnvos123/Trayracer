#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstdio>
#include <exception>

#include "display.h"
#include "game.h"

void error_callback(int err, const char* desc)
{
    fprintf(stderr, "Error: %s}n", desc);
}

void key_callback(GLFWwindow* window, int key, int scode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void init()
{
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(error_callback);

    Display& display = Display::get();

    glfwSetKeyCallback(display.getWindow(), key_callback);

    glewExperimental = GL_TRUE;
    glewInit();
}

int main()
{
    try {
        init();

        Game::get().runGameLoop();

        Display::get().close();

        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
    catch (std::exception& e) {
        printf("%s\n", e.what());
        exit(EXIT_FAILURE);
    }
}

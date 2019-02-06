#include "game.h"

#include <glm/glm.hpp>
#include <cstdio>

Game& Game::get()
{
    static Game g;
    return g;
}

Game::Game()
    : m_texture(Display::get().getWidth(), Display::get().getHeight()),
      m_raytracer(Display::get().getWidth(), Display::get().getHeight())
{}

void Game::runGameLoop()
{
    Display& display = Display::get();

    float prevTime = glfwGetTime();
    float deltaTime = 0;

    float timer = prevTime;

    int frames, updates;

    glEnable(GL_TEXTURE_2D);

    while (!display.shouldClose()) {
        float now = glfwGetTime();
        deltaTime += (now - prevTime) / m_upsLimit;
        prevTime = now;

        while (deltaTime >= 1.0) {
            update();
            updates++;
            deltaTime--;
        }

        render(display);
        frames++;

        if (glfwGetTime() - timer > 1.0) {
            timer++;
            printf("FPS: %d, UPS: %d\n", frames, updates);
            frames = updates = 0;
        }
    }
}

void Game::input()
{
    GLFWwindow* window = Display::get().getWindow();
    m_camera.input(window);
}

void Game::update()
{
    input();

    m_camera.update();

    glfwPollEvents();
}

void Game::render(Display& display)
{
    m_texture.bindImageTexture();

    m_raytracer.dispatch(m_camera);

    m_texture.unbindImageTexture();

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glClear(GL_COLOR_BUFFER_BIT);

    m_texture.bindTexture();
    m_quad.bind();
    m_quadshader.bind();
    glDrawElements(GL_TRIANGLES, m_quad.getIndicesCount(), GL_UNSIGNED_INT, nullptr);
    m_quadshader.unbind();
    m_quad.unbind();
    m_texture.unbindTexture();

    display.swapBuffers();
}

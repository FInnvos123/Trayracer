#include "game.h"

#include "primitives.h"

#include <glm/glm.hpp>
#include <cstdio>

Game& Game::get()
{
    static Game g;
    return g;
}

Game::Game()
    : m_texture(Display::get().getWidth(), Display::get().getHeight()),
      m_raytracer(Display::get().getWidth(), Display::get().getHeight()),
      m_camera(glm::vec3(0, 4, 5))
{}

void Game::runGameLoop()
{
    Display& display = Display::get();

    float prevTime = glfwGetTime();
    float deltaTime = 0; float timer = prevTime;

    int frames = 0, updates = 0;

    glEnable(GL_TEXTURE_2D);

    loadScene();

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

void Game::loadScene()
{
    m_scene.addSphere(new Sphere(glm::vec3(0, 3, 0), 1),
                      new Material(glm::vec4(0.03f, 0.58f, 0.04f, 1),
                                   glm::vec4(glm::vec3(0.9f), 1), 0.4f));

    m_scene.addSphere(new Sphere(glm::vec3(-3, 2, -1), 1),
                      new Material(glm::vec4(0.53f, 0.03f, 0.04f, 1),
                                   glm::vec4(glm::vec3(0.9f), 1), 0.74f));


    m_scene.addSphere(new Sphere(glm::vec3(0, 6, 0), 1),
                      new Material(glm::vec4(0.44f, 0.47f, 0.45f, 1),
                                   glm::vec4(0.84f, 0.87f, 0.85f, 1), 0.67f, 0.83f));

    std::vector<GLfloat> verts = {-5, 0, -5, 5, 0, -5, -5, 10, -5,
                                  5, 0, -5, 5, 10, -5, -5, 10, -5,
                                  -5, 10, 5, 5, 10, 5, -5, 10, -5,
                                  5, 10, 5, 5, 10, -5, -5, 10, -5};
    std::vector<GLfloat> norms = {0, 0, 1, 0, 0, 1,
                                  0, -1, 0, 0, -1, 0};

    m_scene.addPolygon(verts, norms,
                       new Material(glm::vec4(1),
                                    glm::vec4(1)));

    verts = {-5, 0, 5, -5, 0, -5, -5, 10, 5,
             -5, 0, -5, -5, 10, -5, -5, 10, 5};
    norms = {1, 0, 0, 1, 0, 0};

    m_scene.addPolygon(verts, norms,
                       new Material(glm::vec4(0.84f, 0.21f, 0.18f, 1),
                                    glm::vec4(0.85f, 0.74f, 0.73f, 1)));

    m_scene.addPrimitive(PrimitiveType::pointlight,
                         new Pointlight(glm::vec3(-1, 4, 4),
                                        glm::vec4(4, 4, 4, 1)));

    verts = {5, 0, 5, 5, 0, -5, 5, 10, 5,
             5, 0, -5, 5, 10, -5, 5, 10, 5};
    norms = {-1, 0, 0, -1, 0, 0};

    m_scene.addPolygon(verts, norms,
                       new Material(glm::vec4(0.18f, 0.84f, 0.21f, 1),
                                    glm::vec4(0.73f, 0.74f, 0.85f, 1)));

    verts = {-5, 0, 5, 5, 0, 5, -5, 0, -5,
             5, 0, 5, 5, 0, -5, -5, 0, -5};
    norms = {0, 1, 0, 0, 1, 0};

    m_scene.addPolygon(verts, norms,
                       new Material(glm::vec4(0.68f, 0.63f, 0.64f, 1),
                                    glm::vec4(0.86f, 0.84f, 0.87f, 1), 0.6f, 0.6f));

    m_raytracer.loadScene(m_scene);
}

#ifndef GAME_H
#define GAME_H

#include "singleton.h"
#include "display.h"
#include "texture.h"
#include "quad.h"
#include "quadshader.h"
#include "raytracer.h"
#include "camera.h"

class Game : public Singleton
{
    public:
        static Game& get();

        void runGameLoop();

    private:
        Game();

        void update();

        void input();

        void render(Display& display);

        const float m_upsLimit = 1.0 / 60.0;

        Texture m_texture;
        Quad m_quad;
        Quadshader m_quadshader;
        Raytracer m_raytracer;
        Camera m_camera;
};

#endif /* ifndef GAME_H */

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class Raytracer
{
    public:
        Raytracer(int width, int height);
        ~Raytracer();

        void dispatch(glm::mat4& viewMatrix);

    private:
        void getUniformLocations();

        GLuint m_id;

        GLuint m_viewMatrixLocation = 0;
        GLuint m_screenWidthLocation = 0;
        GLuint m_screenHeightLocation = 0;

        GLuint m_workGroupSizeX = 0;
        GLuint m_workGroupSizeY = 0;

        int m_width;
        int m_height;
};

#endif /* ifndef RAYTRACER_H */

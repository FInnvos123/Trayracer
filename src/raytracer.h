#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "camera.h"

class Raytracer
{
    public:
        Raytracer(int width, int height);
        ~Raytracer();

        void dispatch(Camera& cam);

    private:
        void getUniformLocations();

        void loadVec3(GLuint loc, const glm::vec3& vec);

        GLuint m_id;

        GLuint m_camPosLocation = 0;
        GLuint m_p0Location = 0;
        GLuint m_p0p1Location = 0;
        GLuint m_p0p2Location = 0;
        GLuint m_screenWidthLocation = 0;
        GLuint m_screenHeightLocation = 0;

        GLuint m_workGroupSizeX = 0;
        GLuint m_workGroupSizeY = 0;

        int m_width;
        int m_height;
};

#endif /* ifndef RAYTRACER_H */

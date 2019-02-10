#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "camera.h"
#include "scene.h"

class Raytracer
{
    public:
        Raytracer(int width, int height);
        ~Raytracer();

        void dispatch(Camera& cam);

        void loadScene(Scene& scene);

    private:
        void getUniformLocations();

        void* mapBuffer(GLuint bufID, GLuint layout,
                       size_t bufSize, size_t elemSize);

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

        GLuint m_buffers[(int)PrimitiveType::count];
};

#endif /* ifndef RAYTRACER_H */

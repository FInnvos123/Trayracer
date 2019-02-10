#ifndef CAMERA_H
#define CAMERA_H

#include "display.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <math.h>

class Camera
{
    public:
        Camera(glm::vec3 pos);

        void update();

        void input(GLFWwindow* window);

        glm::vec3& getPosition() { return m_position; }
        glm::vec3& getp0() { return m_p0; }
        glm::vec3& getp0p1() { return m_p0p1; }
        glm::vec3& getp0p2() { return m_p0p2; }

    private:
        void mouseLook(GLFWwindow* window);

        const float M_TWOPI = M_PI * 2;

        const float m_movementSpeed = 0.1f;
        const float m_mouseSensitivity = 0.01f;
        const float m_vertBounds = 2.0f;

        double m_mouseCenterX = 0.0;
        double m_mouseCenterY = 0.0;

        glm::vec3 m_position;
        glm::vec3 m_rotation;

        glm::vec3 m_p0;
        glm::vec3 m_p0p1;
        glm::vec3 m_p0p2;

        glm::vec3 m_forward;
        glm::vec3 m_right;
};

#endif /* ifndef CAMERA_H */

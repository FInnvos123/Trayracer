#ifndef CAMERA_H
#define CAMERA_H

#include "display.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <math.h>

class Camera
{
    public:
        Camera();

        void update();

        void input(GLFWwindow* window);

        glm::mat4& getViewMatrix() { return m_viewMatrix; }

    private:
        void mouseLook(GLFWwindow* window);

        glm::mat4 createViewMatrix();

        const float M_TWOPI = M_PI * 2;

        const float m_movementSpeed = 0.1f;
        const float m_mouseSensitivity = 0.01f;
        const float m_vertBounds = 1.5f;

        double m_mouseCenterX = 0.0;
        double m_mouseCenterY = 0.0;

        glm::mat4 m_viewMatrix;

        glm::vec3 position;
        glm::vec3 rotation;

        glm::vec3 m_forward;
        glm::vec3 m_right;
};

#endif /* ifndef CAMERA_H */

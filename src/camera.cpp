#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
    Display& display = Display::get();

    m_mouseCenterX = display.getWidth() / 2;
    m_mouseCenterY = display.getHeight() / 2;

    glfwSetCursorPos(display.getWindow(), m_mouseCenterX, m_mouseCenterY);
}

void Camera::update()
{
    m_viewMatrix = createViewMatrix();
}

void Camera::input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += m_forward * m_movementSpeed;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= m_forward * m_movementSpeed;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= m_right * m_movementSpeed;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += m_right * m_movementSpeed;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        position += glm::vec3(0, 1, 0) * m_movementSpeed;
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        position -= glm::vec3(0, 1, 0) * m_movementSpeed;

    mouseLook(window);
}

void Camera::mouseLook(GLFWwindow* window)
{
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    double dx = mouseX - m_mouseCenterX;
    double dy = mouseY - m_mouseCenterY;

    rotation.x += dy * m_mouseSensitivity;
    rotation.y -= dx * m_mouseSensitivity;

    if (rotation.y > M_TWOPI)
        rotation.y = 0;
    else if (rotation.y < 0)
        rotation.y = M_TWOPI;

    if (rotation.x > m_vertBounds)
        rotation.x = m_vertBounds;
    else if (rotation.x < -m_vertBounds)
        rotation.x = -m_vertBounds;

    glfwSetCursorPos(window, m_mouseCenterX, m_mouseCenterY);
}

glm::mat4 Camera::createViewMatrix()
{
    m_forward = -glm::normalize(glm::vec3(glm::sin(rotation.y),
                                          rotation.x,
                                          glm::cos(rotation.y)));

    m_right = glm::normalize(glm::cross(m_forward, glm::vec3(0, 1, 0)));

    glm::vec3 up = glm::normalize(glm::cross(m_right, m_forward));

    return glm::lookAt(position, position + m_forward, up);
}

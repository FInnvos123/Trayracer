#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos) : m_position(pos)
{
    Display& display = Display::get();

    m_mouseCenterX = display.getWidth() / 2;
    m_mouseCenterY = display.getHeight() / 2;

    glfwSetCursorPos(display.getWindow(), m_mouseCenterX, m_mouseCenterY);
}

void Camera::update()
{
    m_forward = -glm::normalize(glm::vec3(glm::sin(m_rotation.y),
                                          m_rotation.x,
                                          glm::cos(m_rotation.y)));

    m_right = glm::normalize(glm::cross(m_forward, glm::vec3(0, 1, 0)));

    glm::vec3 up = glm::normalize(glm::cross(m_right, m_forward));

    glm::vec3 center = m_position + m_forward;
    m_p0 = center - up - m_right;
    glm::vec3 p1 = center - up + m_right;
    glm::vec3 p2 = center + up - m_right;

    m_p0p1 = p1 - m_p0;
    m_p0p2 = p2 - m_p0;
}

void Camera::input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_position += m_forward * m_movementSpeed;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_position -= m_forward * m_movementSpeed;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_position -= m_right * m_movementSpeed;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_position += m_right * m_movementSpeed;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_position += glm::vec3(0, 1, 0) * m_movementSpeed;
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        m_position -= glm::vec3(0, 1, 0) * m_movementSpeed;

    mouseLook(window);
}

void Camera::mouseLook(GLFWwindow* window)
{
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    double dx = mouseX - m_mouseCenterX;
    double dy = mouseY - m_mouseCenterY;

    m_rotation.x += dy * m_mouseSensitivity;
    m_rotation.y -= dx * m_mouseSensitivity;

    if (m_rotation.y > M_TWOPI)
        m_rotation.y = 0;
    else if (m_rotation.y < 0)
        m_rotation.y = M_TWOPI;

    if (m_rotation.x > m_vertBounds)
        m_rotation.x = m_vertBounds;
    else if (m_rotation.x < -m_vertBounds)
        m_rotation.x = -m_vertBounds;

    glfwSetCursorPos(window, m_mouseCenterX, m_mouseCenterY);
}

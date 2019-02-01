#include "raytracer.h"

#include <glm/gtc/type_ptr.hpp>

#include "shaderloading.h"

Raytracer::Raytracer(int width, int height)
    : m_id(createComputeProgram("raytracer.comp"))
{
    glUseProgram(m_id);

    GLint workGroupSize[3];

    glGetProgramiv(m_id, GL_COMPUTE_WORK_GROUP_SIZE, workGroupSize);
    m_workGroupSizeX = workGroupSize[0];
    m_workGroupSizeY = workGroupSize[1];

    glUniform1i(m_screenWidthLocation, width);
    glUniform1i(m_screenHeightLocation, height);

    m_width = width;
    m_height = height;

    glUseProgram(0);
}

Raytracer::~Raytracer()
{
    glDeleteProgram(m_id);
}

void Raytracer::dispatch(glm::mat4& viewMatrix)
{
    glUseProgram(m_id);
    glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glDispatchCompute(m_width / m_workGroupSizeX, m_height / m_workGroupSizeY, 1);
    glUseProgram(0);
}

void Raytracer::getUniformLocations()
{
    m_viewMatrixLocation = glGetUniformLocation(m_id, "view_matrix");
    m_screenWidthLocation = glGetUniformLocation(m_id, "screen_width");
    m_screenHeightLocation = glGetUniformLocation(m_id, "screen_height");
}

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

    m_width = width;
    m_height = height;

    glUseProgram(0);
}

Raytracer::~Raytracer()
{
    glDeleteProgram(m_id);
}

void Raytracer::dispatch(Camera& cam)
{
    glUseProgram(m_id);

    loadVec3(m_camPosLocation, cam.getPosition());
    loadVec3(m_p0Location, cam.getp0());
    loadVec3(m_p0p1Location, cam.getp0p1());
    loadVec3(m_p0p2Location, cam.getp0p2());

    glDispatchCompute(m_width / m_workGroupSizeX, m_height / m_workGroupSizeY, 1);
    glUseProgram(0);
}

void Raytracer::getUniformLocations()
{
    m_camPosLocation = glGetUniformLocation(m_id, "cam_pos");
    m_p0Location = glGetUniformLocation(m_id, "p0");
    m_p0p1Location = glGetUniformLocation(m_id, "p0p1");
    m_p0p2Location = glGetUniformLocation(m_id, "p0p2");
}

void Raytracer::loadVec3(GLuint loc, const glm::vec3& vec)
{
    glUniform3f(loc, vec.x, vec.y, vec.z);
}

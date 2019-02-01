#include "quadshader.h"

#include "shaderloading.h"

Quadshader::Quadshader()
    : m_id(createQuadProgram("quad.vert", "quad.frag"))
{}

Quadshader::~Quadshader()
{
    glDeleteProgram(m_id);
}

void Quadshader::bind()
{
    glUseProgram(m_id);
}

void Quadshader::unbind()
{
    glUseProgram(0);
}

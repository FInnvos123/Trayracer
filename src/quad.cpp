#include "quad.h"

Quad::Quad()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    std::vector<GLfloat> vertices = {-1,-1,0,1,-1,0,1,1,0,-1,1,0};
    std::vector<GLfloat> uvs = {0,0,1,0,1,1,0,1};
    std::vector<GLuint> indices = {0,1,2,2,3,0};

    addVBO(0, 3, vertices);
    addVBO(1, 2, uvs);
    addEBO(indices);

    m_indicesCount = indices.size();
}

void Quad::bind()
{
    glBindVertexArray(m_vao);
}

void Quad::unbind()
{
    glBindVertexArray(0);
}

void Quad::addVBO(int n, int dim, const std::vector<GLfloat>& data)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 data.size() * sizeof(data[0]),
                 data.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(n,
                          dim,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (GLvoid*) 0);

    glEnableVertexAttribArray(n);
}

void Quad::addEBO(const std::vector<GLuint>& indices)
{
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(indices[0]),
                 indices.data(),
                 GL_STATIC_DRAW);
}

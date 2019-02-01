#ifndef QUAD_H
#define QUAD_H

#include <GL/glew.h>
#include <vector>

class Quad
{
    public:
        Quad();

        GLuint getIndicesCount() { return m_indicesCount; }

        void bind();
        void unbind();

    private:
        void addVBO(int n, int dim, const std::vector<GLfloat>& data);
        void addEBO(const std::vector<GLuint>& indices);

        GLuint m_vao = 0;
        GLuint m_indicesCount;
};

#endif /* ifndef QUAD_H */

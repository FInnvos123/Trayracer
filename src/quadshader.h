#ifndef QUADSHADER_H
#define QUADSHADER_H

#include <GL/glew.h>

class Quadshader
{
    public:
        Quadshader();
        ~Quadshader();

        void bind();
        void unbind();

    private:
        GLuint m_id;
};

#endif /* ifndef QUADSHADER_H */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

class Texture
{
    public:
        Texture(int width, int height);

        void bindImageTexture();
        void unbindImageTexture();

        void bindTexture();
        void unbindTexture();

    private:
        GLuint m_texture;
};

#endif /* ifndef TEXTURE_H */

#include "texture.h"

Texture::Texture(int width, int height)
{
    glGenTextures(1, &m_texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height,
                 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindImageTexture(0, m_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void Texture::bindImageTexture()
{
    glBindImageTexture(0, m_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void Texture::unbindImageTexture()
{
    glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void Texture::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::unbindTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

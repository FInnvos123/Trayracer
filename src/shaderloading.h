#ifndef SHADERLOADING_H
#define SHADERLOADING_H

#include <GL/glew.h>
#include <string>

GLuint createQuadProgram(const std::string& vertFile, const std::string& fragFile);

GLuint createComputeProgram(const std::string& file);

#endif /* ifndef SHADERLOADING_H */

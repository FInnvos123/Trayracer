#include "shaderloading.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::string readFile(const std::string& filename)
{
    std::ifstream file("data/shaders/" + filename + ".glsl");
    std::stringstream stringstream;

    if (!file.is_open()) {
        throw std::runtime_error("Could not open shader: " + filename + ".glsl");
    }

    stringstream << file.rdbuf();

    return stringstream.str();
}

GLuint compileShader(const GLchar* source, GLenum type)
{
    GLuint id = glCreateShader(type);

    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    GLint success;
    GLchar infolog[512];

    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(id, 512, nullptr, infolog);
        throw std::runtime_error("Error compiling shader: " + std::string(infolog));
    }

    return id;
}

GLuint createQuadProgram(const std::string& vertFile, const std::string& fragFile)
{
    std::string vertSource = readFile(vertFile);
    std::string fragSource = readFile(fragFile);

    GLuint vertID = compileShader(vertSource.c_str(), GL_VERTEX_SHADER);
    GLuint fragID = compileShader(fragSource.c_str(), GL_FRAGMENT_SHADER);

    GLuint programID = glCreateProgram();

    glAttachShader(programID, vertID);
    glAttachShader(programID, fragID);

    glLinkProgram(programID);

    return programID;
}

GLuint createComputeProgram(const std::string& file)
{
    std::string source = readFile(file);

    GLuint id = compileShader(source.c_str(), GL_COMPUTE_SHADER);

    GLuint programID = glCreateProgram();

    glAttachShader(programID, id);

    glLinkProgram(programID);

    return programID;
}

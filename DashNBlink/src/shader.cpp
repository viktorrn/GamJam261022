#include "shader.h"

#include "GL/glew.h"

#include <iostream>
#include <fstream>
#include <sstream>

unsigned int shader_compile(const char* source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Error message: " << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

void shader_create(shader* s, const char* path)
{
    std::ifstream stream(path);

    if (!stream.is_open())
    {
        std::cout << "Could not open file: " << path << std::endl;
        return;
    }

    int current = 0;
    std::stringstream ss[2];

    std::string line;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                current = 0;
            }

            else if (line.find("fragment") != std::string::npos)
            {
                current = 1;
            }
        }

        else {
            ss[current] << line << '\n';
        }
    }

    unsigned int programID = glCreateProgram();
    unsigned int vertexShaderID = shader_compile(ss[0].str().c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShaderID = shader_compile(ss[1].str().c_str(), GL_FRAGMENT_SHADER);

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
    glValidateProgram(programID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    s->id = programID;
}

void shader_bind(const shader* s)
{
    glUseProgram(s->id);
}

void shader_unbind()
{
    glUseProgram(0);
}

unsigned int shader_find_location(shader* s, const char* name)
{
    return glGetUniformLocation(s->id, name);
}

void shader_set_uniform1f(unsigned int location, float x)
{
    glUniform1f(location, x);
}

void shader_set_uniform2f(unsigned int location, float x, float y)
{
    glUniform2f(location, x, y);
}

void shader_set_uniform3f(unsigned int location, float x, float y, float z)
{
    glUniform3f(location, x, y, z);
}

void shader_destroy(shader* s)
{
    glUseProgram(0);
    glDeleteProgram(s->id);
}
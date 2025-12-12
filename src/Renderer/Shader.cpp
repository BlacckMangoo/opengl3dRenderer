#include "../../include/Shader.h"
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE

#include <iostream>

#include "glm/gtc/type_ptr.hpp"

Shader& Shader::Use()
{
    if (ID == 0) {
        std::cout << "Error: Attempting to use uninitialized shader" << std::endl;
        return *this;
    }
    glUseProgram(this->ID);
    return *this;
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource )
{
    unsigned int gShader = 0;
    int success;
    char infoLog[512];

    // vertex Shader
    const unsigned int sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, nullptr);
    glCompileShader(sVertex);
    // Check for vertex shader compile errors
    glGetShaderiv(sVertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(sVertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment Shader
    unsigned int sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, nullptr);
    glCompileShader(sFragment);
    // Check for fragment shader compile errors
    glGetShaderiv(sFragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(sFragment, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // if geometry shader source code is given, also compile geometry shader
    if (geometrySource != nullptr)
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, nullptr);
        glCompileShader(gShader);
        // Check for geometry shader compile errors
        glGetShaderiv(gShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(gShader, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    // shader program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, sVertex);
    glAttachShader(this->ID, sFragment);
    if (geometrySource != nullptr)
        glAttachShader(this->ID, gShader);
    glLinkProgram(this->ID);
    // Check for linking errors
    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->ID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != nullptr)
        glDeleteShader(gShader);
}

void Shader::CompileCompute(const char* computeSource)
{
    // compute Shader
    const unsigned int sCompute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(sCompute, 1, &computeSource, nullptr);
    glCompileShader(sCompute);

    // shader program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, sCompute);
    glLinkProgram(this->ID);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(sCompute);
}

void Shader::SetFloat(const char* name, float value, bool useShader)
{
    if (useShader)
        this->Use();
    GLint location = glGetUniformLocation(this->ID, name);
    if (location != -1) {
        glUniform1f(location, value);
    } 
}
void Shader::SetInteger(const char* name, int value, bool useShader)
{
    if (useShader)
        this->Use();
    GLint location = glGetUniformLocation(this->ID, name);
    if (location != -1) {
        glUniform1i(location, value);
    } 
}
void Shader::SetVector2f(const char* name, float x, float y, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::SetVector2f(const char* name, const glm::vec2& value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::SetVector3f(const char* name, float x, float y, float z, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::SetVector3f(const char* name, const glm::vec3& value, bool useShader)
{
    if (useShader)
        this->Use();
    GLint location = glGetUniformLocation(this->ID, name);
    if (location != -1) {
        glUniform3f(location, value.x, value.y, value.z);
    }
}
void Shader::SetVector4f(const char* name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::SetVector4f(const char* name, const glm::vec4& value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
    if (useShader)
        this->Use();
    GLint location = glGetUniformLocation(this->ID, name);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
    }
}
void Shader::SetMatrix3(const char* name, const glm::mat3& matrix, bool useShader)
{
    if (useShader)
        this->Use();
    GLint location = glGetUniformLocation(this->ID, name);
    if (location != -1) {
        glUniformMatrix3fv(location, 1, false, glm::value_ptr(matrix));
    }
}

//
// Created by xiangtao on 2018/10/27.
//

#include "shader.h"

#include <malloc.h>

#include "log.h"

static const char *kTag = "Shader";

Shader &Shader::Use() {
    glUseProgram(this->ID);
    return *this;
}

static GLuint LoadShader(GLenum type, const char *shaderSrc) {
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader(type);

    if (shader == 0) {
        return 0;
    }

    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char *infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));

            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            LOGE(kTag, "Error compiling shader:\n%s\n", infoLog);

            free(infoLog);
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;

}

void Shader::Compile(const GLchar *vertex_shader_source, const GLchar *fragment_shader_source) {
    GLuint vertex_shader_id;
    GLuint fragment_shader_id;
    GLint linked;

    // Load the vertex/fragment shaders
    vertex_shader_id = LoadShader(GL_VERTEX_SHADER, vertex_shader_source);

    if (vertex_shader_id == 0) {
        return;
    }

    fragment_shader_id = LoadShader(GL_FRAGMENT_SHADER, fragment_shader_source);

    if (fragment_shader_id == 0) {
        glDeleteShader(vertex_shader_id);
        return;
    }

    // Create the program object
    ID = glCreateProgram();

    if (ID == 0) {
        return;
    }

    glAttachShader(ID, vertex_shader_id);
    glAttachShader(ID, fragment_shader_id);

    // Link the program
    glLinkProgram(ID);

    // Check the link status
    glGetProgramiv(ID, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint infoLen = 0;

        glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char *infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));

            glGetProgramInfoLog(ID, infoLen, NULL, infoLog);
            LOGE(kTag, "Error linking program:\n%s\n", infoLog);

            free(infoLog);
        }

        glDeleteProgram(ID);
        return;
    }

    // Free up no longer needed shader resources
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}

void Shader::SetFloat(const GLchar *name, GLfloat value) {
    glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetInteger(const GLchar *name, GLint value) {
    glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetVector2f(const GLchar *name, GLfloat x, GLfloat y) {
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}

void Shader::SetVector2f(const GLchar *name, const glm::vec2 &value) {
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}

void Shader::SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z) {
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}

void Shader::SetVector3f(const GLchar *name, const glm::vec3 &value) {
    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}

void Shader::SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}

void Shader::SetVector4f(const GLchar *name, const glm::vec4 &value) {
    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4(const GLchar *name, const glm::mat4 &matrix) {
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}
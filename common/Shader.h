//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_SHADER_H
#define ANDROID_OPENGLES_3_0_SHADER_H

#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "log.h"

class Shader {
public:
    Shader() {}

    void Compile(const GLchar *vertex_shader_source, const GLchar *fragment_shader_source);

    Shader &Use() {
        glUseProgram(id_);
        return *this;
    }

    void Delete() {
        if (id_ != 0) {
            glDeleteProgram(id_);
            id_ = 0;
        }
    }

    void SetFloat(const GLchar *name, GLfloat value) {
        glUniform1f(glGetUniformLocation(this->id_, name), value);
    }

    void SetInteger(const GLchar *name, GLint value) {
        glUniform1i(glGetUniformLocation(this->id_, name), value);
    }

    void SetVector2f(const GLchar *name, GLfloat x, GLfloat y) {
        glUniform2f(glGetUniformLocation(this->id_, name), x, y);
    }

    void SetVector2f(const GLchar *name, const glm::vec2 &value) {
        glUniform2f(glGetUniformLocation(this->id_, name), value.x, value.y);
    }

    void SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z) {
        glUniform3f(glGetUniformLocation(this->id_, name), x, y, z);
    }

    void SetVector3f(const GLchar *name, const glm::vec3 &value) {
        glUniform3f(glGetUniformLocation(this->id_, name), value.x, value.y, value.z);
    }

    void SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
        glUniform4f(glGetUniformLocation(this->id_, name), x, y, z, w);
    }

    void SetVector4f(const GLchar *name, const glm::vec4 &value) {
        glUniform4f(glGetUniformLocation(this->id_, name), value.x, value.y, value.z, value.w);
    }

    void SetMatrix4(const GLchar *name, const glm::mat4 &matrix) {
        glUniformMatrix4fv(glGetUniformLocation(this->id_, name), 1, GL_FALSE,
                           glm::value_ptr(matrix));
    }

private:
    GLuint id_;
};


#endif //ANDROID_OPENGLES_3_0_SHADER_H

//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_SHADER_H
#define ANDROID_OPENGLES_3_0_SHADER_H


#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLES3/gl3.h>


class Shader {
public:
    GLuint ID;

    Shader() {}

    Shader &Use();

    void Compile(const GLchar *vertex_shader_source, const GLchar *fragment_shader_source);

    void SetFloat(const GLchar *name, GLfloat value);

    void SetInteger(const GLchar *name, GLint value);

    void SetVector2f(const GLchar *name, GLfloat x, GLfloat y);

    void SetVector2f(const GLchar *name, const glm::vec2 &value);

    void SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z);

    void SetVector3f(const GLchar *name, const glm::vec3 &value);

    void SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

    void SetVector4f(const GLchar *name, const glm::vec4 &value);

    void SetMatrix4(const GLchar *name, const glm::mat4 &matrix);
};


#endif //ANDROID_OPENGLES_3_0_SHADER_H

//
// Created by xiangtao on 2018/10/27.
//

#include "shader.h"

#include <malloc.h>

#include "log.h"

static const char *kTag = "Shader";

static GLuint LoadShader(GLenum type, const char *shader_src) {
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader(type);

    if (shader == 0) {
        return 0;
    }

    // Load the shader source
    glShaderSource(shader, 1, &shader_src, NULL);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint info_length = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);

        if (info_length > 1) {
            char *info_log = static_cast<char *>(malloc(sizeof(char) * info_length));

            glGetShaderInfoLog(shader, info_length, NULL, info_log);
            LOGE(kTag, "Error compiling shader:\n%s\n%s\n", shader_src, info_log);

            free(info_log);
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;

}

void Shader::Compile(const GLchar *vertex_shader_source, const GLchar *fragment_shader_source) {
    GLuint vertex_shader_id_;
    GLuint fragment_shader_id_;
    GLint linked;

    // Load the vertex/fragment shaders
    vertex_shader_id_ = LoadShader(GL_VERTEX_SHADER, vertex_shader_source);

    if (vertex_shader_id_ == 0) {
        return;
    }

    fragment_shader_id_ = LoadShader(GL_FRAGMENT_SHADER, fragment_shader_source);

    if (fragment_shader_id_ == 0) {
        glDeleteShader(vertex_shader_id_);
        return;
    }

    // Create the program object
    id_ = glCreateProgram();

    if (id_ == 0) {
        return;
    }

    glAttachShader(id_, vertex_shader_id_);
    glAttachShader(id_, fragment_shader_id_);

    // Link the program
    glLinkProgram(id_);

    // Check the link status
    glGetProgramiv(id_, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint info_length = 0;

        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &info_length);

        if (info_length > 1) {
            char *info_log = static_cast<char *>(malloc(sizeof(char) * info_length));

            glGetProgramInfoLog(id_, info_length, NULL, info_log);
            LOGE(kTag, "Error linking program:\n%s\n", info_log);

            free(info_log);
        }

        glDeleteProgram(id_);
        return;
    }

    // Free up no longer needed shader resources
    glDeleteShader(vertex_shader_id_);
    glDeleteShader(fragment_shader_id_);
}
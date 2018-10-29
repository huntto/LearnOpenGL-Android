//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_TEXTURE2D_H
#define ANDROID_OPENGLES_3_0_TEXTURE2D_H


#include <GLES3/gl3.h>
#include <string>
#include "log.h"

class Texture2D {
public:
    Texture2D()
            : width_(0), height_(0) {
        glGenTextures(1, &this->id_);
    }

    void Generate(const std::string &filename,
                  GLint wrap_s = GL_REPEAT,
                  GLint wrap_t = GL_REPEAT,
                  GLint filter_min = GL_LINEAR,
                  GLint filter_max = GL_LINEAR);

    void Delete() {
        if (id_ != 0) {
            glDeleteTextures(1, &id_);
            id_ = 0;
        }
    }

    void Bind() const {
        glBindTexture(GL_TEXTURE_2D, id_);
    }

    GLint get_width() const {
        return width_;
    }

    GLint get_height() const {
        return height_;
    }

private:
    GLuint id_;
    GLint width_;
    GLint height_;
};

#endif //ANDROID_OPENGLES_3_0_TEXTURE2D_H

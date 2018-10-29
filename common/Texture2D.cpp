//
// Created by xiangtao on 2018/10/27.
//

#include "texture2d.h"
#include "log.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

static const char *kTag = "Texture2D";

void Texture2D::Generate(const std::string &filename,
                         GLint wrap_s,
                         GLint wrap_t,
                         GLint filter_min,
                         GLint filter_max) {
    int channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename.c_str(), &width_, &height_, &channels, 0);

    GLuint internal_format;
    GLuint image_format;
    switch (channels) {
        case 1:
            image_format = GL_RED;
            internal_format = GL_RED;
            break;
        case 3:
            image_format = GL_RGB;
            internal_format = GL_RGB;
            break;
        case 4:
            image_format = GL_RGBA;
            internal_format = GL_RGBA;
            break;
        default:
            LOGE(kTag, "Wrong channels:%d", channels);
            goto end;
    }

    LOGE(kTag, "channels:%d, image_format:%d, internal_format:%d", channels, image_format, internal_format);
    glBindTexture(GL_TEXTURE_2D, this->id_);
    LOGE(kTag, "error1:%d", glGetError());
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_, height_, 0, image_format,
                 GL_UNSIGNED_BYTE, data);
    LOGE(kTag, "error2:%d", glGetError());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    LOGE(kTag, "error3:%d", glGetError());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    LOGE(kTag, "error4:%d", glGetError());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
    LOGE(kTag, "error5:%d", glGetError());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);
    LOGE(kTag, "error6:%d", glGetError());

    glBindTexture(GL_TEXTURE_2D, 0);

    end:
    stbi_image_free(data);
}
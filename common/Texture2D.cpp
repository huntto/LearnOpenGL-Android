//
// Created by xiangtao on 2018/10/27.
//

#include "Texture2D.h"
#include "log.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

static const char *kTag = "Texture2D";

Texture2D::Texture2D()
        : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT),
          Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR) {
    glGenTextures(1, &this->ID);
}

void Texture2D::Generate(std::string filename) {
    int channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename.c_str(), &Width, &Height, &channels, 0);

    switch (channels) {
        case 1:
            Image_Format = GL_RED;
            Internal_Format = GL_RED;
            break;
        case 3:
            Image_Format = GL_RGB;
            Internal_Format = GL_RGB;
            break;
        case 4:
            Image_Format = GL_RGBA;
            Internal_Format = GL_RGBA;
            break;
        default:
            LOGE(kTag, "Wrong channels:%d", channels);
            goto end;
    }

    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, Width, Height, 0, this->Image_Format,
                 GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

    glBindTexture(GL_TEXTURE_2D, 0);

    end:
    stbi_image_free(data);
}

void Texture2D::Bind() const {
    glBindTexture(GL_TEXTURE_2D, this->ID);
}
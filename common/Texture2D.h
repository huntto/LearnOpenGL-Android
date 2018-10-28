//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_TEXTURE2D_H
#define ANDROID_OPENGLES_3_0_TEXTURE2D_H


#include <GLES3/gl3.h>
#include <string>

class Texture2D {
public:
    GLuint ID;

    GLint Width, Height;

    GLuint Internal_Format;
    GLuint Image_Format;

    GLuint Wrap_S;
    GLuint Wrap_T;
    GLuint Filter_Min;
    GLuint Filter_Max;

    Texture2D();

    void Generate(std::string filename);

    void Bind() const;
};


#endif //ANDROID_OPENGLES_3_0_TEXTURE2D_H

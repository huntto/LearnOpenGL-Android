//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_TRIANGLE_H
#define ANDROID_OPENGLES_3_0_TRIANGLE_H

#include <shader.h>
#include <texture2d.h>

class MixTexture {
public:
    MixTexture();

    ~MixTexture();

    void Draw();

    void Update(float delta_time);

private:
    Shader shader_;
    GLuint vbo_;
    GLuint ebo_;
    GLuint vao_;
    Texture2D texture1_;
    Texture2D texture2_;
};

#endif //ANDROID_OPENGLES_3_0_TRIANGLE_H

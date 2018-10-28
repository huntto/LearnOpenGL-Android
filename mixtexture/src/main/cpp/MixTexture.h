//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_TRIANGLE_H
#define ANDROID_OPENGLES_3_0_TRIANGLE_H

#include <Shader.h>
#include <Texture2D.h>

class Triangle {
public:
    Triangle();

    ~Triangle();

    void Draw();

    void Update(float delta_time);

private:
    Shader shader_;
    GLuint VBO;
    GLuint EBO;
    GLuint VAO;
    Texture2D texture1_;
    Texture2D texture2_;
};

#endif //ANDROID_OPENGLES_3_0_TRIANGLE_H

//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_TRIANGLE_H
#define ANDROID_OPENGLES_3_0_TRIANGLE_H

#include <Shader.h>

class Triangle {
public:
    Triangle();

    ~Triangle();

    void Draw();

    void Update(float delta_time);

private:
    Shader shader_;
    GLuint VBO;
    GLuint VAO;
};

#endif //ANDROID_OPENGLES_3_0_TRIANGLE_H

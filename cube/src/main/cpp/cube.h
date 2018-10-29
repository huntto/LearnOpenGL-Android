//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_TRIANGLE_H
#define ANDROID_OPENGLES_3_0_TRIANGLE_H

#include <shader.h>
#include <texture2d.h>
#include <glm/glm.hpp>

class Cube {
public:
    Cube();

    ~Cube();

    void Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix);

    void Update(float delta_time);

private:
    Shader shader_;
    GLuint vbo_;
    GLuint ebo_;
    GLuint vao_;
    Texture2D texture1_;
    Texture2D texture2_;
    glm::mat4 model_matrix_;
};

#endif //ANDROID_OPENGLES_3_0_TRIANGLE_H

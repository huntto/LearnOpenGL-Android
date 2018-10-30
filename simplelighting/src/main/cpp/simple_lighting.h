//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_SIMPLE_LIGHTING_H
#define ANDROID_OPENGLES_3_0_SIMPLE_LIGHTING_H

#include <shader.h>
#include <texture2d.h>
#include <glm/glm.hpp>

class SimpleLighting {
public:
    SimpleLighting();

    ~SimpleLighting();

    void Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix);

    void Update(float delta_time);

private:
    Shader object_shader_;
    Shader light_shader_;

    GLuint vao_;
    GLuint vbo_;

    glm::mat4 object_model_matrix_;
    glm::mat4 light_model_matrix_;

    glm::vec3 light_pos_;
};

#endif //ANDROID_OPENGLES_3_0_SIMPLE_LIGHTING_H

//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_SIMPLE_LIGHTING_H
#define ANDROID_OPENGLES_3_0_SIMPLE_LIGHTING_H

#include <glm/glm.hpp>
#include <camera.h>

#include <shader.h>
#include <texture2d.h>

class MultiLights {
public:
    MultiLights();

    ~MultiLights();

    void Draw(const glm::vec3 &camera_pos,
              const glm::mat4 &projection_matrix,
              const glm::mat4 &view_matrix);

    void Update(float delta_time);

private:
    Shader object_shader_;
    Shader light_shader_;

    GLuint object_vao_;
    GLuint object_vbo_;

    GLuint light_vao_;
    GLuint light_vbo_;

    glm::mat4 object_model_matrix_;
    glm::mat4 light_model_matrix_;

    glm::vec3 light_pos_;
    glm::vec3 light_color_;

    Texture2D diffuse_map_;
    Texture2D specular_map_;
};

#endif //ANDROID_OPENGLES_3_0_SIMPLE_LIGHTING_H

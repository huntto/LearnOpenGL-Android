//
// Created by xiangtao on 2018/10/27.
//
#include "multi_lights.h"

#include <GLES3/gl3.h>
#include <log.h>
#include <resource_manager.h>
#include <camera.h>

const char *kTag = "Triangle";

MultiLights::MultiLights() {
    std::string object_vertex_shader_path
            = ResourceManager::ExtractAssetFileToInternal("object_vertex_shader.glsl", true);
    std::string object_fragment_shader_path
            = ResourceManager::ExtractAssetFileToInternal("object_fragment_shader.glsl", true);

    std::string light_vertex_shader_path
            = ResourceManager::ExtractAssetFileToInternal("light_vertex_shader.glsl", true);
    std::string light_fragment_shader_path
            = ResourceManager::ExtractAssetFileToInternal("light_fragment_shader.glsl", true);

    std::string container_path = ResourceManager::ExtractAssetFileToInternal("container2.png");
    std::string container_specular_path = ResourceManager::ExtractAssetFileToInternal("container2_specular.png");

    object_shader_ = ResourceManager::LoadShader(object_vertex_shader_path,
                                                 object_fragment_shader_path,
                                                 "object-shader");
    light_shader_ = ResourceManager::LoadShader(light_vertex_shader_path,
                                                light_fragment_shader_path,
                                                "light-shader");

    diffuse_map_.Generate(container_path);
    specular_map_.Generate(container_specular_path);

    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &object_vao_);
    glGenBuffers(1, &object_vbo_);

    glBindVertexArray(object_vao_);

    glBindBuffer(GL_ARRAY_BUFFER, object_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    glGenVertexArrays(1, &light_vao_);
    glGenBuffers(1, &light_vbo_);

    glBindVertexArray(light_vao_);

    glBindBuffer(GL_ARRAY_BUFFER, light_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    object_model_matrix_ = glm::mat4(1.0f);

    light_pos_ = glm::vec3(1.2f, 1.0f, 2.0f);
    light_model_matrix_ = glm::mat4(1.0f);
    light_model_matrix_ = glm::translate(light_model_matrix_, light_pos_);
    light_model_matrix_ = glm::scale(light_model_matrix_, glm::vec3(0.2f));

    light_color_ = glm::vec3(1.0f);
}

void MultiLights::Update(float delta_time) {

}

void MultiLights::Draw(const glm::vec3 &camera_pos,
                          const glm::mat4 &projection_matrix,
                          const glm::mat4 &view_matrix) {
    static glm::vec3 cube_positions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    object_shader_.Use();

    object_shader_.SetMatrix4("projection", projection_matrix);
    object_shader_.SetMatrix4("view", view_matrix);
    object_shader_.SetVector3f("viewPos", camera_pos);

    object_shader_.SetVector3f("light.direction", -0.2f, -1.0f, -0.3f);
    object_shader_.SetVector3f("light.ambient", 0.2f, 0.2f, 0.2f);
    object_shader_.SetVector3f("light.diffuse", 0.5f, 0.5f, 0.5f);
    object_shader_.SetVector3f("light.specular", 1.0f, 1.0f, 1.0f);

    object_shader_.SetInteger("material.diffuse", 0);
    object_shader_.SetInteger("material.specular", 1);
    object_shader_.SetFloat("material.shininess", 32.0f);

    glActiveTexture(GL_TEXTURE0);
    diffuse_map_.Bind();

    glActiveTexture(GL_TEXTURE1);
    specular_map_.Bind();

    glBindVertexArray(object_vao_);

    for (unsigned int i = 0; i < 10; i++) {
        glm::mat4 model(1.0);
        model = glm::translate(model, cube_positions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        object_shader_.SetMatrix4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    light_shader_.Use();
    light_shader_.SetVector3f("lightColor", light_color_);
    light_shader_.SetMatrix4("model", light_model_matrix_);
    light_shader_.SetMatrix4("projection", projection_matrix);
    light_shader_.SetMatrix4("view", view_matrix);

    glBindVertexArray(light_vao_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

MultiLights::~MultiLights() {

}
//
// Created by xiangtao on 2018/10/27.
//
#include "mix_texture.h"

#include <GLES3/gl3.h>
#include <log.h>
#include <resource_manager.h>

const char *kTag = "Triangle";

MixTexture::MixTexture() {
    std::string vertex_shader_path = ResourceManager::ExtractAssetToInternal("vertex_shader.glsl", true);
    std::string fragment_shader_path = ResourceManager::ExtractAssetToInternal("fragment_shader.glsl", true);
    std::string awesomeface_path = ResourceManager::ExtractAssetToInternal("awesomeface.png", true);
    std::string container_path = ResourceManager::ExtractAssetToInternal("container.jpg", true);


    shader_ = ResourceManager::LoadShader(vertex_shader_path, fragment_shader_path, "mix-textures");
    texture1_ = ResourceManager::LoadTexture(container_path, "container");
    texture2_ = ResourceManager::LoadTexture(awesomeface_path, "awesomeface");

    float vertices[] = {
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void MixTexture::Update(float delta_time) {

}

void MixTexture::Draw() {
    shader_.Use();

    glActiveTexture(GL_TEXTURE0);
    texture1_.Bind();
    glActiveTexture(GL_TEXTURE1);
    texture2_.Bind();

    shader_.SetInteger("texture1", 0);
    shader_.SetInteger("texture2", 1);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

MixTexture::~MixTexture() {

}
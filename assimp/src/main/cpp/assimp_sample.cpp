//
// Created by xiangtao on 2018/10/27.
//
#include "assimp_sample.h"

#include <GLES3/gl3.h>
#include <log.h>
#include <resource_manager.h>

#include "model.h"

const char *kTag = "Triangle";

AssimpSample::AssimpSample() {
    std::string shader_path = ResourceManager::ExtractAssetToInternal("shader", true);
    std::string obj_path = ResourceManager::ExtractAssetToInternal("nanosuit", true);

    shader_ = ResourceManager::LoadShader(shader_path + "/model_loading_vertex_shader.glsl",
                                                shader_path + "/model_loading_fragment_shader.glsl",
                                                "assimp_sample");

    model_.LoadModel(obj_path + "/nanosuit.obj");
}

void AssimpSample::Update(float delta_time) {

}

void AssimpSample::Draw(const glm::mat4 &projection_matrix,
                        const glm::mat4 &view_matrix) {
    shader_.Use();

    shader_.SetMatrix4("projection", projection_matrix);
    shader_.SetMatrix4("view", view_matrix);

    // render the loaded model
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
    shader_.SetMatrix4("model", model);
    model_.Draw(shader_);

}

AssimpSample::~AssimpSample() {

}
//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_TRIANGLE_H
#define ANDROID_OPENGLES_3_0_TRIANGLE_H

#include <shader.h>
#include <texture2d.h>
#include <glm/glm.hpp>

#include "model.h"

class AssimpSample {
public:
    AssimpSample();

    ~AssimpSample();

    void Draw(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix);

    void Update(float delta_time);

private:
    Shader shader_;
    Model  model_;

};

#endif //ANDROID_OPENGLES_3_0_TRIANGLE_H

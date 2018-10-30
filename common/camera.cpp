//
// Created by xiangtao on 2018/10/30.
//

#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : front_(glm::vec3(0.0f, 0.0f, -1.0f)),
          movement_speed_(kDefaultSpeed),
          movement_sensitivity_(kDefaultSensitivity),
          zoom_(kDefaultZoom) {

    position_ = position;
    world_up_ = up;
    yaw_ = yaw;
    pitch_ = pitch;
    UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ,
               float upX, float upY, float upZ,
               float yaw,
               float pitch) : front_(glm::vec3(0.0f, 0.0f, -1.0f)),
                              movement_speed_(kDefaultSpeed),
                              movement_sensitivity_(kDefaultSensitivity),
                              zoom_(kDefaultZoom) {
    position_ = glm::vec3(posX, posY, posZ);
    world_up_ = glm::vec3(upX, upY, upZ);
    yaw_ = yaw;
    pitch_ = pitch;
    UpdateCameraVectors();
}

void Camera::TurnAround(float xoffset, float yoffset, bool constrain_pitch) {
    xoffset *= movement_sensitivity_;
    yoffset *= movement_sensitivity_;

    yaw_ += xoffset;
    pitch_ += yoffset;

    if (constrain_pitch) {
        if (pitch_ > 89.0f)
            pitch_ = 89.0f;
        if (pitch_ < -89.0f)
            pitch_ = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
    glm::vec3 front;
    front.x = static_cast<float>(cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)));
    front.y = static_cast<float>(sin(glm::radians(pitch_)));
    front.z = static_cast<float>(sin(glm::radians(yaw_)) * cos(glm::radians(pitch_)));

    front_ = glm::normalize(front);
    right_ = glm::normalize(glm::cross(front_,
                                       world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
}
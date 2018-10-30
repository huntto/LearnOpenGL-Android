//
// Created by xiangtao on 2018/10/30.
//

#ifndef ANDROID_OPENGLES_3_0_CAMERA_H
#define ANDROID_OPENGLES_3_0_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const float kDefaultYaw = -90.0f;
static const float kDefaultPitch = 0.0f;
static const float kDefaultSpeed = 2.5f;
static const float kDefaultSensitivity = 0.1f;
static const float kDefaultZoom = 45.0f;

class Camera {
public:
    enum Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = kDefaultYaw,
           float pitch = kDefaultPitch);

    Camera(float posX, float posY, float posZ,
           float upX, float upY, float upZ,
           float yaw,
           float pitch);

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(position_, position_ + front_, up_);
    }

    void Move(Direction direction, float delta_time) {
        float velocity = movement_speed_ * delta_time;
        if (direction == FORWARD)
            position_ += front_ * velocity;
        if (direction == BACKWARD)
            position_ -= front_ * velocity;
        if (direction == LEFT)
            position_ -= right_ * velocity;
        if (direction == RIGHT)
            position_ += right_ * velocity;
    }

    void TurnAround(float xoffset, float yoffset, bool constrain_pitch = true);

    void Zoom(float offset) {
        if (zoom_ >= 1.0f && zoom_ <= 45.0f)
            zoom_ -= offset;
        if (zoom_ <= 1.0f)
            zoom_ = 1.0f;
        if (zoom_ >= 45.0f)
            zoom_ = 45.0f;
    }

    glm::vec3 get_position() {
        return position_;
    }

    float get_zoom() {
        return zoom_;
    }

private:
    void UpdateCameraVectors();

    float yaw_;
    float pitch_;

    float movement_speed_;
    float movement_sensitivity_;
    float zoom_;

    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;
};

#endif //ANDROID_OPENGLES_3_0_CAMERA_H

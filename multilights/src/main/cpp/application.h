//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_APP_H
#define ANDROID_OPENGLES_3_0_APP_H


#include <EGL/eglplatform.h>
#include <EGL/egl.h>
#include <glm/glm.hpp>

#include <memory>
#include <log.h>

#include "camera.h"


class TouchListener {
    virtual bool OnTouchDown(int32_t pid, float x, float y) = 0;

    virtual bool OnTouchMove(int32_t pid, float x, float y) = 0;

    virtual bool OnTouchUp(int32_t pid, float x, float y) = 0;

    virtual bool OnTouchCancel() = 0;
};

typedef std::function<bool(float, float)> ActionListener;

class Button : public TouchListener {
public:
    Button(const std::string &name = std::string(),
           float x = 0,
           float y = 0,
           float width = 0,
           float height = 0)
            : name_(name), x_(x), y_(y), width_(width), height_(height),
              pointer_id_(kInvalidPointerId) {}

    Button(const Button &) = default;

    Button &operator=(const Button &) = default;

    virtual bool OnTouchDown(int32_t pid, float x, float y) {
        if (x > x_ && x < x_ + width_ && y > y_ && y < y_ + height_) {
            pointer_id_ = pid;
            if (on_action_down_ != nullptr) {
                return on_action_down_(x, y);
            }
        }
        return false;
    }

    virtual bool OnTouchMove(int32_t pid, float x, float y) {
        if (pointer_id_ == pid) {
            if (on_action_move_ != nullptr) {
                return on_action_move_(x, y);
            }
        }
        return false;
    }

    virtual bool OnTouchUp(int32_t pid, float x, float y) {
        if (pointer_id_ == pid) {
            pointer_id_ = kInvalidPointerId;
            if (on_action_up_ != nullptr) {
                return on_action_up_(x, y);
            }
        }
        return false;
    }

    virtual bool OnTouchCancel() {
        if (pointer_id_ != kInvalidPointerId && on_action_up_ != nullptr) {
            pointer_id_ = kInvalidPointerId;
            return on_action_up_(kInvalidPointerId, kInvalidPointerId);
        }
        return false;
    }

    void set_action_listeners(ActionListener on_action_down,
                              ActionListener on_action_move,
                              ActionListener on_action_up) {
        on_action_down_ = on_action_down;
        on_action_move_ = on_action_move;
        on_action_up_ = on_action_up;
    }

    std::string get_name() {
        return name_;
    }

private:
    static const int kInvalidPointerId = -1;
    ActionListener on_action_down_;
    ActionListener on_action_move_;
    ActionListener on_action_up_;
    int32_t pointer_id_;
    float x_;
    float y_;
    float width_;
    float height_;
    std::string name_;
};

class Application : public TouchListener {
public:
    bool Init(EGLNativeWindowType egl_native_window, EGLNativeDisplayType egl_native_display);

    void Update(float delta_time);

    void Draw();

    void Destroy();

    virtual bool OnTouchDown(int32_t pid, float x, float y);

    virtual bool OnTouchMove(int32_t pid, float x, float y);

    virtual bool OnTouchUp(int32_t pid, float x, float y);

    virtual bool OnTouchCancel();

protected:
    bool CreateWindow(EGLNativeWindowType egl_native_window,
                      EGLNativeDisplayType egl_native_display);

    void UpdateWindowSize();

    EGLint width_;
    EGLint height_;
    EGLSurface egl_surface_;
    EGLDisplay egl_display_;
    void *data_;
    glm::mat4 projection_matrix_;
    glm::mat4 view_matrix_;
    Camera camera_;
    Button up_button_;
    Button down_button_;
    Button right_button_;
    Button left_button_;
    Button track_ball_button_;
    bool move_left_;
    bool move_right_;
    bool move_up_;
    bool move_down_;
    float track_ball_prev_x_;
    float track_ball_prev_y_;
    float track_ball_offset_x_;
    float track_ball_offset_y_;
};

#endif //ANDROID_OPENGLES_3_0_APP_H

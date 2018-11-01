#include "application.h"

#include <GLES3/gl3.h>
#include <malloc.h>
#include <EGL/egl.h>
#include <android/native_window.h>

#include "log.h"
#include "multi_lights.h"

static const char *kTag = "Application";

static EGLint GetContextRenderableType(EGLDisplay eglDisplay) {
#ifdef EGL_KHR_create_context
    const char *extensions = eglQueryString ( eglDisplay, EGL_EXTENSIONS );

       // check whether EGL_KHR_create_context is in the extension string
       if ( extensions != NULL && strstr( extensions, "EGL_KHR_create_context" ) )
       {
          // extension is supported
          return EGL_OPENGL_ES3_BIT_KHR;
       }
#endif
    // extension is not supported
    return EGL_OPENGL_ES2_BIT;
}

bool Application::CreateWindow(EGLNativeWindowType egl_native_window,
                               EGLNativeDisplayType egl_native_display) {

    egl_display_ = eglGetDisplay(egl_native_display);
    if (egl_display_ == EGL_NO_DISPLAY) {
        return GL_FALSE;
    }

    // Initialize EGL
    EGLint major_version;
    EGLint minor_version;
    if (!eglInitialize(egl_display_, &major_version, &minor_version)) {
        return GL_FALSE;
    }

    EGLint num_configs = 0;
    EGLint attrib_list[] = {
            EGL_RED_SIZE, 5,
            EGL_GREEN_SIZE, 6,
            EGL_BLUE_SIZE, 5,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 8,
            EGL_STENCIL_SIZE, EGL_DONT_CARE,
            EGL_SAMPLE_BUFFERS, 0,
            EGL_RENDERABLE_TYPE, GetContextRenderableType(egl_display_),
            EGL_NONE
    };

    // Choose config
    EGLConfig config;
    if (!eglChooseConfig(egl_display_, attrib_list, &config, 1, &num_configs)) {
        return GL_FALSE;
    }

    if (num_configs < 1) {
        return GL_FALSE;
    }

    // For Android, need to get the EGL_NATIVE_VISUAL_ID and set it using ANativeWindow_setBuffersGeometry
    EGLint format = 0;
    eglGetConfigAttrib(egl_display_, config, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(egl_native_window, 0, 0, format);


    // Create a surface
    egl_surface_ = eglCreateWindowSurface(egl_display_, config,
                                          egl_native_window, NULL);

    if (egl_surface_ == EGL_NO_SURFACE) {
        return GL_FALSE;
    }

    // Create a GL context
    EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    EGLContext egl_context = eglCreateContext(egl_display_, config,
                                              EGL_NO_CONTEXT, context_attribs);

    if (egl_context == EGL_NO_CONTEXT) {
        return GL_FALSE;
    }

    // Make the context current
    if (!eglMakeCurrent(egl_display_, egl_surface_,
                        egl_surface_, egl_context)) {
        return GL_FALSE;
    }

    return GL_TRUE;
}

void Application::Draw() {
    if (data_ != nullptr) {
        UpdateWindowSize();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


        glm::mat4 projection = glm::perspective(glm::radians(camera_.get_zoom()),
                                                (float) width_ / (float) height_, 0.1f, 100.0f);


        MultiLights *simple_lighting = static_cast<MultiLights *>(data_);
        simple_lighting->Draw(camera_, projection);

        eglSwapBuffers(egl_display_, egl_surface_);
    }
}

void Application::UpdateWindowSize() {
    eglQuerySurface(egl_display_, egl_surface_, EGL_WIDTH, &width_);
    eglQuerySurface(egl_display_, egl_surface_, EGL_HEIGHT, &height_);
    glViewport(0, 0, width_, height_);
}

bool Application::Init(EGLNativeWindowType egl_native_window,
                       EGLNativeDisplayType egl_native_display) {
    if (CreateWindow(egl_native_window, egl_native_display)) {
        UpdateWindowSize();

        float button_width = width_ / 6;
        float button_height = height_ / 3;
        left_button_ = Button("left", 0, height_ / 3,
                              button_width, button_height);
        right_button_ = Button("right", width_ / 6 * 2, height_ / 3,
                               button_width, button_height);
        up_button_ = Button("up", width_ / 6, 0,
                            button_width, button_height);
        down_button_ = Button("down", width_ / 6, height_ / 3 * 2,
                              button_width, button_height);
        track_ball_button_ = Button("track_ball", width_ / 2, 0,
                                    width_ / 2, height_);

        left_button_.set_action_listeners(
                [this](float, float) {
                    move_left_ = true;
                    return true;
                },
                [this](float, float) {
                    return true;
                },
                [this](float, float) {
                    move_left_ = false;
                    return true;
                });
        right_button_.set_action_listeners(
                [this](float, float) {
                    move_right_ = true;
                    return true;
                },
                [this](float, float) {
                    return true;
                },
                [this](float, float) {
                    move_right_ = false;
                    return true;
                });
        up_button_.set_action_listeners(
                [this](float, float) {
                    move_up_ = true;
                    return true;
                },
                [this](float, float) {
                    return true;
                },
                [this](float, float) {
                    move_up_ = false;
                    return true;
                });
        down_button_.set_action_listeners(
                [this](float, float) {
                    move_down_ = true;
                    return true;
                },
                [this](float, float) {
                    return true;
                },
                [this](float, float) {
                    move_down_ = false;
                    return true;
                });
        track_ball_button_.set_action_listeners(
                [this](float x, float y) {
                    track_ball_offset_x_ = 0;
                    track_ball_offset_y_ = 0;
                    track_ball_prev_x_ = x;
                    track_ball_prev_y_ = y;
                    return true;
                },
                [this](float x, float y) {
                    track_ball_offset_x_ = track_ball_prev_x_ - x;
                    track_ball_offset_y_ = track_ball_prev_y_ - y;
                    track_ball_prev_x_ = x;
                    track_ball_prev_y_ = y;
                    return true;
                },
                [this](float, float) {
                    track_ball_offset_x_ = 0;
                    track_ball_offset_y_ = 0;
                    return true;
                });
        MultiLights *simple_lighting = new MultiLights;
        data_ = simple_lighting;

        camera_ = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);
        return true;
    }
    return false;
}

void Application::Destroy() {
    if (data_ != nullptr) {
        MultiLights *simple_lighting = static_cast<MultiLights *>(data_);
        delete simple_lighting;
        data_ = nullptr;
    }
}

void Application::Update(float delta_time) {
    if (data_ != nullptr) {
        static float time = 0;
        time += delta_time;

        if (move_left_) {
            camera_.Move(Camera::Direction::LEFT, delta_time);
        }
        if (move_right_) {
            camera_.Move(Camera::Direction::RIGHT, delta_time);
        }
        if (move_down_) {
            camera_.Move(Camera::Direction::BACKWARD, delta_time);
        }
        if (move_up_) {
            camera_.Move(Camera::Direction::FORWARD, delta_time);
        }

        camera_.TurnAround(track_ball_offset_x_, track_ball_offset_y_);

        MultiLights *simple_lighting = static_cast<MultiLights *>(data_);
        simple_lighting->Update(delta_time);
    }
}

bool Application::OnTouchDown(int32_t pid, float x, float y) {
    LOGI(kTag, "OnTouchDown pid:%d, x:%f, y:%f", pid, x, y);
    left_button_.OnTouchDown(pid, x, y);
    right_button_.OnTouchDown(pid, x, y);
    up_button_.OnTouchDown(pid, x, y);
    down_button_.OnTouchDown(pid, x, y);
    track_ball_button_.OnTouchDown(pid, x, y);
    return true;
}

bool Application::OnTouchMove(int32_t pid, float x, float y) {
    LOGI(kTag, "OnTouchMove pid:%d, x:%f, y:%f", pid, x, y);
    left_button_.OnTouchMove(pid, x, y);
    right_button_.OnTouchMove(pid, x, y);
    up_button_.OnTouchMove(pid, x, y);
    down_button_.OnTouchMove(pid, x, y);
    track_ball_button_.OnTouchMove(pid, x, y);
    return true;
}

bool Application::OnTouchUp(int32_t pid, float x, float y) {
    LOGI(kTag, "OnTouchUp pid:%d, x:%f, y:%f", pid, x, y);
    left_button_.OnTouchUp(pid, x, y);
    right_button_.OnTouchUp(pid, x, y);
    up_button_.OnTouchUp(pid, x, y);
    down_button_.OnTouchUp(pid, x, y);
    track_ball_button_.OnTouchUp(pid, x, y);
    return true;
}

bool Application::OnTouchCancel() {
    LOGI(kTag, "OnTouchCancel");
    left_button_.OnTouchCancel();
    right_button_.OnTouchCancel();
    up_button_.OnTouchCancel();
    down_button_.OnTouchCancel();
    track_ball_button_.OnTouchCancel();
    return true;
}
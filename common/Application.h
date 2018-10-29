//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_APP_H
#define ANDROID_OPENGLES_3_0_APP_H


#include <EGL/eglplatform.h>
#include <EGL/egl.h>

class Application {
public:
    bool Init(EGLNativeWindowType egl_native_window, EGLNativeDisplayType egl_native_display);

    void Update(float delta_time);

    void Draw();

    void Destroy();

protected:
    bool CreateWindow(EGLNativeWindowType egl_native_window,
                      EGLNativeDisplayType egl_native_display);

    EGLint width_;
    EGLint height_;
    EGLSurface egl_surface_;
    EGLDisplay egl_display_;
    void *data_;

};

#endif //ANDROID_OPENGLES_3_0_APP_H

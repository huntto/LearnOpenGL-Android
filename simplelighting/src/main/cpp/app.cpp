#include "application.h"

#include <GLES3/gl3.h>
#include <malloc.h>
#include <EGL/egl.h>
#include <android/native_window.h>

#include "log.h"
#include "simple_lighting.h"

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
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, EGL_DONT_CARE,
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
        glm::mat4 view = camera_.GetViewMatrix();

        SimpleLighting *simple_lighting = static_cast<SimpleLighting *>(data_);
        simple_lighting->Draw(camera_.get_position(), projection, view);

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

        SimpleLighting *simple_lighting = new SimpleLighting;
        data_ = simple_lighting;

        camera_ = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

        glEnable(GL_DEPTH_TEST);
        return true;
    }
    return false;
}

void Application::Destroy() {
    if (data_ != nullptr) {
        SimpleLighting *simple_lighting = static_cast<SimpleLighting *>(data_);
        delete simple_lighting;
        data_ = nullptr;
    }
}

void Application::Update(float delta_time) {
    if (data_ != nullptr) {
        static float time = 0;
        time += delta_time;

        SimpleLighting *simple_lighting = static_cast<SimpleLighting *>(data_);
        simple_lighting->Update(delta_time);
    }
}
#include "Application.h"

#include <android/log.h>
#include <android_native_app_glue.h>
#include <time.h>
#include <string.h>
#include <EGL/egl.h>

#include "log.h"
#include "resource_manager.h"

static float GetCurrentTime() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    double time_in_seconds = time.tv_sec + (double) time.tv_nsec / 1e9;
    return (float) time_in_seconds;
}

static void HandleCommand(struct android_app *app, int32_t cmd) {
    Application *my_app = (Application *) app->userData;
    if (my_app == NULL) {
        return;
    }
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // the OS asked us to save the state of the app
            break;

        case APP_CMD_INIT_WINDOW:
            my_app->Init(app->window, EGL_DEFAULT_DISPLAY);
            break;

        case APP_CMD_TERM_WINDOW:
            my_app->Destroy();
            app->userData = NULL;
            delete my_app;
            break;

        case APP_CMD_LOST_FOCUS:
            // if the app lost focus, avoid unnecessary processing (like monitoring the accelerometer)
            break;

        case APP_CMD_GAINED_FOCUS:
            // bring back a certain functionality, like monitoring the accelerometer
            break;

        default:
            break;
    }
}

void android_main(struct android_app *app) {
    Application *my_app = new Application;
    float last_time;

    ResourceManager::context = app;
    app->onAppCmd = HandleCommand;
    app->userData = my_app;

    last_time = GetCurrentTime();

    while (1) {
        int events;
        struct android_poll_source *source;

        while (ALooper_pollAll(0, NULL, &events, (void **) &source) >= 0) {

            if (source != NULL) {
                source->process(app, source);
            }

            if (app->destroyRequested != 0) {
                return;
            }
        }

        float current_time = GetCurrentTime();
        float delta_time = (current_time - last_time);
        last_time = current_time;
        my_app->Update(delta_time);

        my_app->Draw();
    }
}
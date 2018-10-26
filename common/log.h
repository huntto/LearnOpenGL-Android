//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_LOG_H
#define ANDROID_OPENGLES_3_0_LOG_H

#include <android/log.h>

#define LOGI(TAG, ...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(TAG, ...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOGW(TAG, ...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)

#endif //ANDROID_OPENGLES_3_0_LOG_H

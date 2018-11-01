cmake_minimum_required(VERSION 3.4.1)

set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")

add_library(
        native_app_glue STATIC

        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")


set(COMMON_SRC_DIR ${CMAKE_SOURCE_DIR}/../../../../common)
set(EXTERNALS_SRC_DIR ${CMAKE_SOURCE_DIR}/../../../../externals)

aux_source_directory(${COMMON_SRC_DIR} COMMON_SRC)
include_directories(${COMMON_SRC_DIR}
        ${EXTERNALS_SRC_DIR}/stb
        ${EXTERNALS_SRC_DIR}/glm)

set(LIB_NAME multi_lights)
add_library(
        ${LIB_NAME} SHARED
        application.cpp
        android_main.cpp
        multi_lights.cpp
        ${COMMON_SRC}
)

target_include_directories(
        ${LIB_NAME}
        PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)

target_link_libraries(
        ${LIB_NAME}

        android
        EGL
        GLESv3
        native_app_glue
        log)
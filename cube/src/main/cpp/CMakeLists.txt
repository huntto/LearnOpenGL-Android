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

add_library(
        cube SHARED
        app.cpp
        cube.cpp
        ${COMMON_SRC}
)

target_include_directories(
        cube
        PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)

target_link_libraries(
        cube

        android
        EGL
        GLESv3
        native_app_glue
        log)
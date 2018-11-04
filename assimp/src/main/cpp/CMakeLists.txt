cmake_minimum_required(VERSION 3.4.1)

set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
set(ANDROID_STL c++_shared)
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
        ${EXTERNALS_SRC_DIR}/glm
        ${EXTERNALS_SRC_DIR}/assimp/include)

add_library(IrrXML STATIC IMPORTED)
set_target_properties(IrrXML PROPERTIES IMPORTED_LOCATION
        ${EXTERNALS_SRC_DIR}/assimp/lib/${ANDROID_ABI}/libIrrXML.a)

add_library(assimp SHARED IMPORTED)
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION
        ${EXTERNALS_SRC_DIR}/assimp/lib/${ANDROID_ABI}/libassimp.so)

set(LIB_NAME assimp_sample)
add_library(
        ${LIB_NAME} SHARED
        application.cpp
        assimp_sample.cpp
        android_main.cpp
        mesh.cpp
        model.cpp
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
        assimp
        IrrXML
        log)
//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_RESOURCE_MANAGER_H
#define ANDROID_OPENGLES_3_0_RESOURCE_MANAGER_H


#include <string>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>
#include <map>

#include "shader.h"
#include "texture2d.h"

class ResourceManager {
public:
    static std::string ExtractAssetFileToInternal(const std::string &asset_filename,
                                                  bool force_replace = false);

    static Shader LoadShader(const std::string &vertex_shader_path,
                             const std::string &fragment_shader_path,
                             const std::string &name);

    static Shader GetShader(const std::string &name);

    static Texture2D LoadTexture(const std::string &filepath,
                                 const std::string &name);

    static Texture2D GetTexture(const std::string &name);

    static void Clear();

    static void *context;

private:
    ResourceManager() {}

    static Shader LoadShaderFromFile(const std::string &vertex_shader_path,
                                     const std::string &fragment_shader_path);

    static Texture2D LoadTextureFromFile(const std::string &filepath);

    static std::map<std::string, Shader> shaders_;
    static std::map<std::string, Texture2D> textures_;

};

#endif //ANDROID_OPENGLES_3_0_RESOURCE_MANAGER_H

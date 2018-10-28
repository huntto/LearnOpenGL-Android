//
// Created by xiangtao on 2018/10/27.
//

#ifndef ANDROID_OPENGLES_3_0_RESOURCE_MANAGER_H
#define ANDROID_OPENGLES_3_0_RESOURCE_MANAGER_H


#include <string>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>
#include <map>

#include "Shader.h"
#include "Texture2D.h"

class ResourceManager {
public:
    static void *Context;
    // Resource storage
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;

    static std::string ExtractAssetFileToInternal(std::string asset_filename, bool force_replace = false);

    static Shader
    LoadShader(std::string vertex_shader_path, std::string fragment_shader_path,
               std::string name);

    static Shader GetShader(std::string name);

    static Texture2D LoadTexture(std::string filepath, std::string name);

    static Texture2D GetTexture(std::string name);

    static void Clear();

private:
    ResourceManager() {}

    static Shader
    LoadShaderFromFile(std::string vertex_shader_path, std::string fragment_shader_path);

    static Texture2D LoadTextureFromFile(std::string filepath);

};

#endif //ANDROID_OPENGLES_3_0_RESOURCE_MANAGER_H

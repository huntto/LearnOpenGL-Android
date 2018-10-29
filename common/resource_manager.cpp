//
// Created by xiangtao on 2018/10/27.
//

#include "resource_manager.h"

#include <android/asset_manager.h>
#include <android_native_app_glue.h>

#include <fstream>
#include <sstream>

#include "log.h"

static const char *kTag = "ResourceManager";

void *ResourceManager::context = nullptr;

std::map<std::string, Texture2D>    ResourceManager::textures_;
std::map<std::string, Shader>       ResourceManager::shaders_;

std::string ResourceManager::ExtractAssetFileToInternal(const std::string &asset_filename,
                                                        bool force_replace) {

    if (context == nullptr) {
        LOGE(kTag, "Context is null.");
        return std::string();
    }
    android_app *app = static_cast<android_app *>(context);

    std::string filename = app->activity->internalDataPath;
    filename += "/" + asset_filename;

    FILE *file = fopen(filename.c_str(), "rb");
    if (file && !force_replace) {
        fclose(file);
        return filename;
    }

    AAsset *asset = AAssetManager_open(app->activity->assetManager, asset_filename.c_str(),
                                       AASSET_MODE_STREAMING);

    char buf[BUFSIZ];
    int nb_read = 0;
    if (asset != NULL) {
        FILE *out = fopen(filename.c_str(), "w");
        while ((nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0) {
            fwrite(buf, nb_read, 1, out);
        }
        fclose(out);
        AAsset_close(asset);
        return filename;
    } else {
        LOGE(kTag, "Extract asset file:%s to internal failed.", asset_filename.c_str());
        return std::string();
    }
}


Shader ResourceManager::LoadShader(const std::string &vertex_shader_path,
                                   const std::string &fragment_shader_path,
                                   const std::string &name) {
    shaders_[name] = LoadShaderFromFile(vertex_shader_path, fragment_shader_path);
    return shaders_[name];
}

Shader ResourceManager::GetShader(const std::string &name) {
    return shaders_[name];
}

Texture2D ResourceManager::LoadTexture(const std::string &filepath,
                                       const std::string &name) {
    textures_[name] = LoadTextureFromFile(filepath);
    return textures_[name];
}

Texture2D ResourceManager::GetTexture(const std::string &name) {
    return textures_[name];
}

void ResourceManager::Clear() {
    for (auto iter : shaders_)
        iter.second.Delete();

    for (auto iter : textures_)
        iter.second.Delete();
}

static std::string ReadStringFromFile(const std::string &filepath) {
    std::string data;
    try {
        std::ifstream file_stream(filepath);
        std::stringstream file_string_stream;
        file_string_stream << file_stream.rdbuf();
        file_stream.close();
        return file_string_stream.str();
    } catch (std::exception e) {
        LOGE(kTag, "Failed to read string from file:%s", filepath.c_str());
    }
    return std::string();
}

Shader ResourceManager::LoadShaderFromFile(const std::string &vertex_shader_path,
                                           const std::string &fragment_shader_path) {
    std::string vertex_shader_src = ReadStringFromFile(vertex_shader_path);
    std::string fragment_shader_src = ReadStringFromFile(fragment_shader_path);

    Shader shader;
    shader.Compile(vertex_shader_src.c_str(), fragment_shader_src.c_str());
    return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const std::string &filepath) {
    Texture2D texture;

    texture.Generate(filepath);

    return texture;
}
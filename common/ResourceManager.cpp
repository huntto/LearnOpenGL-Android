//
// Created by xiangtao on 2018/10/27.
//

#include "ResourceManager.h"

#include <android/asset_manager.h>
#include <android_native_app_glue.h>
#include <fstream>
#include <sstream>

#include "log.h"

static const char *kTag = "ResourceManager";

void *ResourceManager::Context = nullptr;
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;

std::string
ResourceManager::ExtractAssetFileToInternal(std::string asset_filename, bool force_replace) {

    if (Context == nullptr) {
        LOGE(kTag, "Context is null.");
        return std::string();
    }
    android_app *app = static_cast<android_app *>(Context);

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


Shader
ResourceManager::LoadShader(std::string vertex_shader_path, std::string fragment_shader_path,
                            std::string name) {
    Shaders[name] = LoadShaderFromFile(vertex_shader_path, fragment_shader_path);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name) {
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(std::string filepath, std::string name) {
    Textures[name] = LoadTextureFromFile(filepath);
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name) {
    return Textures[name];
}

void ResourceManager::Clear() {
    // (Properly) delete all shaders
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (Properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

static std::string ReadStringFromFile(std::string filepath) {
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

Shader ResourceManager::LoadShaderFromFile(std::string vertex_shader_path,
                                           std::string fragment_shader_path) {
    std::string vertex_shader_src = ReadStringFromFile(vertex_shader_path);
    std::string fragment_shader_src = ReadStringFromFile(fragment_shader_path);

    Shader shader;
    shader.Compile(vertex_shader_src.c_str(), fragment_shader_src.c_str());
    return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(std::string filepath) {
    Texture2D texture;

    texture.Generate(filepath);

    return texture;
}
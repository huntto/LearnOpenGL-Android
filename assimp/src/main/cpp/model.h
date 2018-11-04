//
// Created by xiangtao on 2018/11/2.
//

#ifndef ANDROID_OPENGLES_3_0_MODEL_H
#define ANDROID_OPENGLES_3_0_MODEL_H

#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
    Model() {}

    void LoadModel(std::string path);

    void Draw(Shader shader) {
        for (int i = 0; i < meshes_.size(); ++i) {
            meshes_[i].Draw(shader);
        }
    }

private:

    void ProcessNode(aiNode *node, const aiScene *scene);

    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> LoadMaterialTextures(aiMaterial *mat,
                                              aiTextureType type,
                                              std::string type_name);

    std::vector<Texture> textures_loaded_;
    std::vector<Mesh> meshes_;
    std::string directory_;
    bool gamma_correction_;

};

#endif //ANDROID_OPENGLES_3_0_MODEL_H

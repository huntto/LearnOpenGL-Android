//
// Created by xiangtao on 2018/11/1.
//

#ifndef ANDROID_OPENGLES_3_0_MESH_H
#define ANDROID_OPENGLES_3_0_MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <shader.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::vector<Texture> textures);

    void Draw(Shader shader);

private:
    void SetupMesh();

    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture> textures_;

    unsigned int vao_;
    unsigned int vbo_;
    unsigned int ebo_;
};

#endif //ANDROID_OPENGLES_3_0_MESH_H

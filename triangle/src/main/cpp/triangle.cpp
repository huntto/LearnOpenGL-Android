//
// Created by xiangtao on 2018/10/27.
//

#include <GLES3/gl3.h>
#include "triangle.h"

Triangle::Triangle() {

    char vertex_shader_src[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec4 vPosition;  \n"
            "void main()                              \n"
            "{                                        \n"
            "   gl_Position = vPosition;              \n"
            "}                                        \n";

    char fragment_shader_src[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "out vec4 fragColor;                          \n"
            "void main()                                  \n"
            "{                                            \n"
            "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
            "}                                            \n";


    shader_.Compile(vertex_shader_src, fragment_shader_src);
}

void Triangle::Update(float delta_time) {

}

void Triangle::Draw() {
    shader_.Use();
    static GLfloat vertices[] = {
            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
    };

    shader_.Use();

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

Triangle::~Triangle() {

}
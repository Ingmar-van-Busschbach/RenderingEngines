#pragma once

#include <vector>
#include <glad/glad.h>
#include "../vertex.h"
#include "texture.h"

namespace core {
    class Mesh {
    private:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
        void render();
        void renderSkyBox(Texture texture);
        static Mesh generateQuad();
        static Mesh generateSkyBox();
    private:
        void setupBuffers();
    };
}
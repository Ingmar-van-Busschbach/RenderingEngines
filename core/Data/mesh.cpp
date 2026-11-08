#include "mesh.h"

namespace core {
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices) : vertices(vertices), indices(indices) {
        printf("Loaded Mesh with %zi vertices\n", vertices.size());
        setupBuffers();
    }

    void Mesh::setupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(Vertex) * vertices.size()), &vertices[0],
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(unsigned int) * indices.size()),
                     &indices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, uv));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, tangent));
        glBindVertexArray(0);
    }

    Mesh Mesh::generateQuad() {
        const glm::vec3 pos[] = {
                glm::vec3(-1.0f, -1.0f, 0.0f),
                glm::vec3(1.0f, -1.0f, 0.0f),
                glm::vec3(-1.0f, 1.0f, 0.0f),
                glm::vec3(1.0f, -1.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::vec3(-1.0f, 1.0f, 0.0f),
        };

        const glm::vec3 normals[] = {
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
        };
        const glm::vec2 uvs[] = {
                glm::vec2(0, 0),
                glm::vec2(1, 0),
                glm::vec2(0, 1),
                glm::vec2(1, 0),
                glm::vec2(1, 1),
                glm::vec2(0, 1)
        };
        const std::vector<GLuint> indices = {0, 1, 2, 3, 4, 5};

        std::vector<Vertex> vertexVector;
        vertexVector.reserve(6);
        for (int i = 0; i < 6; ++i) {
            vertexVector.emplace_back(pos[i], normals[i], uvs[i], glm::vec3(1.0f, 0.0f, 0.0f));
        }

        return Mesh(vertexVector, indices);
    }

    Mesh Mesh::generateSkyBox()
    {
        const glm::vec3 pos[] = {
            //   Coordinates
            glm::vec3(-1.0f, -1.0f,  1.0f),//        7--------6
            glm::vec3( 1.0f, -1.0f,  1.0f),//       /|       /|
            glm::vec3( 1.0f, -1.0f, -1.0f),//      4--------5 |
            glm::vec3(-1.0f, -1.0f, -1.0f),//      | |      | |
            glm::vec3(-1.0f,  1.0f,  1.0f),//      | 3------|-2
            glm::vec3( 1.0f,  1.0f,  1.0f),//      |/       |/
            glm::vec3( 1.0f,  1.0f, -1.0f),//      0--------1
            glm::vec3(-1.0f,  1.0f, -1.0f)
        };
        const std::vector<GLuint> indices = {
            // Right
            1, 6, 2,
            6, 1, 5,
            // Left
            0, 7, 4,
            7, 0, 3,
            // Top
            4, 6, 5,
            6, 4, 7,
            // Bottom
            0, 2, 3,
            2, 0, 1,
            // Back
            0, 5, 1,
            5, 0, 4,
            // Front
            3, 6, 7,
            6, 3, 2
        };
        std::vector<Vertex> vertexVector;
        vertexVector.reserve(8);
        for (int i = 0; i < 8; ++i) {
            vertexVector.emplace_back(pos[i], glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0, 0), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        return Mesh(vertexVector, indices);
    }

    void Mesh::render() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    }
    void Mesh::renderSkyBox(Texture texture) {
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture.getId());
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    }
}
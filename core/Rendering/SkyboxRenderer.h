//
// Created by Gebruiker on 11/7/2026.
//

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "MatrixShaderData.h"
#include "../Data/texture.h"
#include "../Data/mesh.h"

namespace core
{
    namespace rendering
    {
        class SkyboxRenderer
        {
            public:
            GLuint shaderProgram;
            MatrixShaderData matrixShaderData;
            GLint textureUniform;
            SkyboxRenderer(GLuint shaderProgram);
            ~SkyboxRenderer();
            void Render(Mesh mesh, Texture texture);
            void ApplyMatrices(glm::mat4 view, glm::mat4 projection);
        };
    } // rendering
} // core

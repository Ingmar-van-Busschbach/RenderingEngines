//
// Created by Gebruiker on 10/28/2026.
//

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace core::rendering
{
    class MatrixShaderData
    {
        public:
        MatrixShaderData(GLuint shaderProgram);
        ~MatrixShaderData();
        GLuint shaderProgram;
        GLint modelMatrixUniform;
        GLint viewMatrixUniform;
        GLint projectionMatrixUniform;

        void GetUniforms();
        void ApplyUniforms(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) const;
    };
} // core::rendering

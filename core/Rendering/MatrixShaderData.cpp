//
// Created by Gebruiker on 10/28/2026.
//

#include "MatrixShaderData.h"
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace core::rendering
{
    MatrixShaderData::MatrixShaderData(GLuint shaderProgram)
    {
        this->shaderProgram = shaderProgram;
    }

    MatrixShaderData::~MatrixShaderData() = default;

    void MatrixShaderData::GetUniforms()
    {
        std::string uniformLocation = "matrixData.modelMatrix";
        modelMatrixUniform = glGetUniformLocation(shaderProgram, uniformLocation.c_str());
        uniformLocation = "matrixData.viewMatrix";
        viewMatrixUniform = glGetUniformLocation(shaderProgram, uniformLocation.c_str());
        uniformLocation = "matrixData.projectionMatrix";
        projectionMatrixUniform = glGetUniformLocation(shaderProgram, uniformLocation.c_str());
    }

    void MatrixShaderData::ApplyUniforms(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) const
    {
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }
} // core::rendering
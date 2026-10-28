//
// Created by Gebruiker on 10/26/2026.
//

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../core/model.h"
#include "LightShaderData.h"
#include "MatrixShaderData.h"
#include "CameraShaderData.h"
#include "../core/texture.h"

namespace core::rendering
{
    class ModelRenderer
    {
    public:
        ModelRenderer();
        ~ModelRenderer();
        std::vector<unsigned int> shaderPrograms;
        std::vector<core::rendering::LightShaderData> lightShaderDatas;
        std::vector<core::rendering::MatrixShaderData> matrixShaderDatas;
        std::vector<core::rendering::CameraShaderData> cameraShaderDatas;
        std::vector<GLint> textureUniforms;
        std::vector<GLint> matrixUniforms;
        const unsigned int GenerateShaderProgram(GLuint vertexShader, GLuint fragmentShader);
        void GenerateShaderDatas(int lightCount);
        void ApplyLightShaderData(const std::vector<objects::Light*>& lights);
        void ApplyMatrixShaderData(int index, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const;
        void ApplyCamareaShaderData(glm::vec3 cameraPosition) const;
        void Render(core::Model model, int shaderProgramIndex, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, core::Texture texture) const;
    };
}


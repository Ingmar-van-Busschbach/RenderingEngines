//
// Created by Gebruiker on 10/26/2026.
//
#include "ModelRenderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "MaterialShaderData.h"
#include "../Objects/model.h"
#include "../Data/material.h"

namespace core::rendering
{
    ModelRenderer::ModelRenderer()
    {

    }
    ModelRenderer::~ModelRenderer()
    {
        for (int i = 0; i < shaderPrograms.size(); i++)
        {
            glDeleteProgram(shaderPrograms.at(i));
        }
        shaderPrograms.clear();
        lightShaderDatas.clear();
        matrixShaderDatas.clear();
        cameraShaderDatas.clear();
        textureUniforms.clear();
        matrixUniforms.clear();
    }

    const unsigned int ModelRenderer::GenerateShaderProgram(GLuint vertexShader, GLuint fragmentShader)
    {
        int success;
        char infoLog[512];
        const unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            printf("Error! Making Shader Program: %s\n", infoLog);
            return 0;
        }
        shaderPrograms.push_back(shaderProgram);
        return shaderProgram;
    }

    void ModelRenderer::GenerateShaderDatas(int lightCount)
    {
        for (int i = 0; i < shaderPrograms.size(); i++)
        {
            lightShaderDatas.push_back(core::rendering::LightShaderData(lightCount, shaderPrograms.at(i)));
            lightShaderDatas.at(i).GetUniforms();
            matrixShaderDatas.push_back(core::rendering::MatrixShaderData(shaderPrograms.at(i)));
            matrixShaderDatas.at(i).GetUniforms();
            cameraShaderDatas.push_back(core::rendering::CameraShaderData(shaderPrograms.at(i)));
            cameraShaderDatas.at(i).GetUniforms();
            materialShaderDatas.push_back(core::rendering::MaterialShaderData(shaderPrograms.at(i)));
            materialShaderDatas.at(i).GetUniforms();
        }
    }

    void ModelRenderer::ApplyLightShaderData(const std::vector<objects::Light*>& lights)
    {
        for (int i = 0; i < lightShaderDatas.size(); i++)
        {
            lightShaderDatas.at(i).ApplyUniforms(lights);
        }
    }

    void ModelRenderer::ApplyMatrixShaderData(const int index, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const
    {
        matrixShaderDatas.at(index).ApplyUniforms(modelMatrix, viewMatrix, projectionMatrix);
    }

    void ModelRenderer::ApplyCamaraShaderData(glm::vec3 cameraPosition) const
    {
        for (int i = 0; i < cameraShaderDatas.size(); i++)
        {
            cameraShaderDatas.at(i).ApplyUniforms(cameraPosition);
        }
    }

    void ModelRenderer::ApplyMaterialShaderData(int shaderProgramIndex, core::data::Material material) const
    {
        materialShaderDatas.at(shaderProgramIndex).ApplyUniforms(material);
    }

    void ModelRenderer::Render(core::Model model, int shaderProgramIndex, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, core::data::Material material) const
    {
        glUseProgram(shaderPrograms.at(shaderProgramIndex));
        this->ApplyMatrixShaderData(shaderProgramIndex, model.getModelMatrix(), viewMatrix, projectionMatrix);
        this->ApplyMaterialShaderData(shaderProgramIndex, material);
        model.render();
        glBindVertexArray(0);
    }
}

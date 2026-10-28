//
// Created by Gebruiker on 10/26/2026.
//
#include "ModelRenderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "../core/model.h"
#include "../core/texture.h"

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
            textureUniforms.push_back(glGetUniformLocation(shaderPrograms.at(i), "text"));
            matrixUniforms.push_back(glGetUniformLocation(shaderPrograms.at(i), "mvpMatrix"));
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

    void ModelRenderer::ApplyCamareaShaderData(glm::vec3 cameraPosition) const
    {
        for (int i = 0; i < cameraShaderDatas.size(); i++)
        {
            cameraShaderDatas.at(i).ApplyUniforms(cameraPosition);
        }
    }

    void ModelRenderer::Render(core::Model model, int shaderProgramIndex, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, core::Texture texture) const
    {
        glUseProgram(shaderPrograms.at(shaderProgramIndex));
        this->ApplyMatrixShaderData(shaderProgramIndex, model.getModelMatrix(), viewMatrix, projectionMatrix);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(textureUniforms.at(shaderProgramIndex), 0);
        glBindTexture(GL_TEXTURE_2D, texture.getId());
        model.render();
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }
}

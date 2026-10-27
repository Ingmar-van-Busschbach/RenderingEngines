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

    void ModelRenderer::GenerateLightShaderDatas(int lightCount)
    {
        for (int i = 0; i < shaderPrograms.size(); i++)
        {
            lightShaderDatas.push_back(new core::rendering::LightShaderData(lightCount, shaderPrograms.at(i)));
            lightShaderDatas.at(i)->GetUniforms();
            textureUniforms.push_back(glGetUniformLocation(shaderPrograms.at(i), "text"));
            matrixUniforms.push_back(glGetUniformLocation(shaderPrograms.at(i), "mvpMatrix"));
        }
    }

    void ModelRenderer::ApplyLightShaderData(const std::vector<objects::Light*>& lights)
    {
        for (int i = 0; i < lightShaderDatas.size(); i++)
        {
            lightShaderDatas.at(i)->ApplyUniforms(lights);
        }
    }

    void ModelRenderer::Render(core::Model model, int shaderProgramIndex, glm::mat4 matrix, core::Texture texture) const
    {
        glUseProgram(shaderPrograms.at(shaderProgramIndex));
        glUniformMatrix4fv(matrixUniforms.at(shaderProgramIndex), 1, GL_FALSE, glm::value_ptr(matrix * model.getModelMatrix()));
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(textureUniforms.at(shaderProgramIndex), 0);
        glBindTexture(GL_TEXTURE_2D, texture.getId());
        model.render();
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }
}

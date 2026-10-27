//
// Created by Gebruiker on 10/26/2026.
//

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../core/model.h"
#include "LightShaderData.h"
#include "../core/texture.h"

namespace core::rendering
{
    class ModelRenderer
    {
    public:
        ModelRenderer();
        ~ModelRenderer();
        std::vector<unsigned int> shaderPrograms;
        std::vector<core::rendering::LightShaderData*> lightShaderDatas;
        std::vector<GLint> textureUniforms;
        std::vector<GLint> matrixUniforms;
        const unsigned int GenerateShaderProgram(GLuint vertexShader, GLuint fragmentShader);
        void GenerateLightShaderDatas(int lightCount);
        void ApplyLightShaderData(const std::vector<objects::Light*>& lights);
        void Render(core::Model , int shaderProgramIndex, glm::mat4 matrix, core::Texture texture) const;
    };
}


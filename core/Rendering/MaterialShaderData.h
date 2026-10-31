//
// Created by Gebruiker on 10/30/2026.
//

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../Data/Material.h"

namespace core::rendering
{
    class MaterialShaderData
    {
        public:
        GLuint shaderProgram;
        GLint materialTypeUniform;
        GLint albedoTextureUniform;
        GLint specularTextureUniform;
        GLint normalTextureUniform;
        GLint albedoColorUniform;
        GLint specularIntensityUniform;

        MaterialShaderData(GLuint shaderProgram);
        ~MaterialShaderData();

        void GetUniforms();
        void ApplyUniforms(data::Material material) const;
    };
} // core
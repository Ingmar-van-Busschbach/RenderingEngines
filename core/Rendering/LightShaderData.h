//
// Created by Gebruiker on 10/26/2026.
//

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../Objects/Light.h"

namespace core::rendering
{
    class LightShaderData
    {
    public:
        LightShaderData(int lightCount, GLuint shaderProgram);
        ~LightShaderData();
        int lightCount;
        GLuint shaderProgram;
        GLint lightCountUniform;
        std::vector<GLint> lightTypeUniforms;
        std::vector<GLint> lightPositionUniforms;
        std::vector<GLint> lightDirectionUniforms;
        std::vector<GLint> lightColorUniforms;
        std::vector<GLint> lightIntensityUniforms;
        std::vector<GLint> lightAttenuationUniforms;
        std::vector<GLint> spotLightAngleUniforms;
        std::vector<GLint> spotLightFalloffUniforms;

        void GetUniforms();
        void ApplyUniforms(std::vector<objects::Light*> lights) const;
    };
}

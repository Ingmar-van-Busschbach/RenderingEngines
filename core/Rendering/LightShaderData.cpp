//
// Created by Gebruiker on 10/26/2026.
//
#include "LightShaderData.h"
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace core::rendering
{
    LightShaderData::LightShaderData(int lightCount, GLuint shaderProgram)
    {
        this->lightCount = lightCount;
        this->shaderProgram = shaderProgram;
        this->lightCountUniform = glGetUniformLocation(shaderProgram, "lightCount");
    }
    LightShaderData::~LightShaderData()
    {
        this->lightTypeUniforms.clear();
        this->lightPositionUniforms.clear();
        this->lightDirectionUniforms.clear();
        this->lightColorUniforms.clear();
        this->lightAttenuationUniforms.clear();
        this->spotLightAngleUniforms.clear();
        this->spotLightFalloffUniforms.clear();
    }

    void LightShaderData::GetUniforms()
    {
        for (int i = 0; i < lightCount ; i++)
        {
            std::string uniformLocation = "lights[" + std::to_string(i) + "].lightType";
            lightTypeUniforms.push_back(glGetUniformLocation(shaderProgram, uniformLocation.c_str()));
            uniformLocation = "lights[" + std::to_string(i) + "].lightPosition";
            lightPositionUniforms.push_back(glGetUniformLocation(shaderProgram, uniformLocation.c_str()));
            uniformLocation = "lights[" + std::to_string(i) + "].lightDirection";
            lightDirectionUniforms.push_back(glGetUniformLocation(shaderProgram, uniformLocation.c_str()));
            uniformLocation = "lights[" + std::to_string(i) + "].lightColor";
            lightColorUniforms.push_back(glGetUniformLocation(shaderProgram, uniformLocation.c_str()));
            uniformLocation = "lights[" + std::to_string(i) + "].lightIntensity";
            lightIntensityUniforms.push_back(glGetUniformLocation(shaderProgram, uniformLocation.c_str()));
            uniformLocation = "lights[" + std::to_string(i) + "].lightAttenuation";
            lightAttenuationUniforms.push_back(glGetUniformLocation(shaderProgram, uniformLocation.c_str()));
            uniformLocation = "lights[" + std::to_string(i) + "].spotLightAngle";
            spotLightAngleUniforms.push_back(glGetUniformLocation(shaderProgram, uniformLocation.c_str()));
            uniformLocation = "lights[" + std::to_string(i) + "].spotLightFalloff";
            spotLightFalloffUniforms.push_back(glGetUniformLocation(shaderProgram, uniformLocation.c_str()));
        }
    }

    void LightShaderData::ApplyUniforms(std::vector<objects::Light*> lights) const
    {
        glUseProgram(shaderProgram);
        glUniform1i(lightCountUniform, lightCount);
        for (int i = 0; i < lightCount ; i++)
        {
            glUniform1i(lightTypeUniforms.at(i), lights.at(i)->GetLightType());
            glUniform3fv(lightPositionUniforms.at(i), 1, glm::value_ptr(lights.at(i)->GetLightPosition()));
            glUniform3fv(lightDirectionUniforms.at(i), 1, glm::value_ptr(lights.at(i)->GetLightDirection()));
            glUniform4fv(lightColorUniforms.at(i), 1, glm::value_ptr(lights.at(i)->GetLightColor()));
            glUniform1f(lightIntensityUniforms.at(i), lights.at(i)->GetLightIntensity());
            glUniform3fv(lightAttenuationUniforms.at(i), 1, glm::value_ptr(lights.at(i)->GetLightAttenuation()));
            glUniform1f(spotLightAngleUniforms.at(i), lights.at(i)->GetSpotLightAngle());
            glUniform1f(spotLightFalloffUniforms.at(i), lights.at(i)->GetSpotLightFalloff());
        }
    }
}

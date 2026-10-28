//
// Created by Gebruiker on 10/28/2026.
//

#include "CameraShaderData.h"
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace core::rendering
{
    CameraShaderData::CameraShaderData(GLuint shaderProgram)
    {
        this->shaderProgram = shaderProgram;
    }

    CameraShaderData::~CameraShaderData() = default;

    void CameraShaderData::GetUniforms()
    {
        std::string uniformLocation = "cameraData.cameraPosition";
        cameraPositionUniform = glGetUniformLocation(shaderProgram, uniformLocation.c_str());
    }

    void CameraShaderData::ApplyUniforms(glm::vec3 cameraPosition) const
    {
        glUseProgram(shaderProgram);
        glUniform3fv(cameraPositionUniform, 1, glm::value_ptr(cameraPosition));
    }
} // core
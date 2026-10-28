//
// Created by Gebruiker on 10/28/2026.
//

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace core::rendering
{
    class CameraShaderData
    {
        public:
        CameraShaderData(GLuint shaderProgram);
        ~CameraShaderData();
        GLuint shaderProgram;
        GLint cameraPositionUniform;
        void GetUniforms();
        void ApplyUniforms(glm::vec3 cameraPosition) const;
    };
} // core

//
// Created by Gebruiker on 11/10/2026.
//

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace core
{
    namespace rendering
    {
        class TimeShaderData
        {
            public:
            GLuint shaderProgram;
            GLint timeUniform;
            GLint deltaTimeUniform;

            TimeShaderData(GLuint shaderProgram);
            ~TimeShaderData();

            void GetUniforms();
            void ApplyUniforms(float time, float deltaTime) const;
        };
    } // rendering
} // core


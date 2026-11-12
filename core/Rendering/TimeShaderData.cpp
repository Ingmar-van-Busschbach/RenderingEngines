//
// Created by Gebruiker on 11/10/2026.
//

#include "TimeShaderData.h"
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace core
{
    namespace rendering
    {
        TimeShaderData::TimeShaderData(GLuint shaderProgram)
        {
            this->shaderProgram = shaderProgram;
        }

        TimeShaderData::~TimeShaderData() = default;

        void TimeShaderData::GetUniforms()
        {
            std::string uniformLocation = "time.time";
            timeUniform = glGetUniformLocation(shaderProgram, uniformLocation.c_str());
            uniformLocation = "time.deltaTime";
            deltaTimeUniform = glGetUniformLocation(shaderProgram, uniformLocation.c_str());
        }

        void TimeShaderData::ApplyUniforms(float time, float deltaTime) const
        {
            glUseProgram(shaderProgram);
            glUniform1f(timeUniform, time);
            glUniform1f(deltaTimeUniform, deltaTime);
        }
    } // rendering
} // core
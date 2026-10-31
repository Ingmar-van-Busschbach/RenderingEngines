//
// Created by Gebruiker on 10/30/2026.
//

#include "MaterialShaderData.h"

#include <glm/gtc/type_ptr.hpp>

namespace core::rendering
{
    MaterialShaderData::MaterialShaderData(GLuint shaderProgram)
    {
        this->shaderProgram = shaderProgram;
    }
    MaterialShaderData::~MaterialShaderData() = default;

    void MaterialShaderData::GetUniforms()
    {
        std::string uniformLocation = "material.materialType";
        materialTypeUniform = glGetUniformLocation(shaderProgram, uniformLocation.c_str());
        uniformLocation = "material.albedoTexture";
        albedoTextureUniform = glGetUniformLocation(shaderProgram, uniformLocation.c_str());
        uniformLocation = "material.specularTexture";
        specularTextureUniform = glGetUniformLocation(shaderProgram, uniformLocation.c_str());
        uniformLocation = "material.normalTexture";
        normalTextureUniform = glGetUniformLocation(shaderProgram, uniformLocation.c_str());
        uniformLocation = "material.albedoColor";
        albedoColorUniform = glGetUniformLocation(shaderProgram, uniformLocation.c_str());
        uniformLocation = "material.specularIntensity";
        specularIntensityUniform = glGetUniformLocation(shaderProgram, uniformLocation.c_str());
    }
    void MaterialShaderData::ApplyUniforms(data::Material material) const
    {
        glUseProgram(shaderProgram);

        glUniform1i(materialTypeUniform, material.materialType);

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(albedoTextureUniform, 0);
        glBindTexture(GL_TEXTURE_2D, material.albedoTextureID);
        glActiveTexture(GL_TEXTURE0 + 1);
        glUniform1i(specularTextureUniform, 1);
        glBindTexture(GL_TEXTURE_2D, material.specularTextureID);
        glActiveTexture(GL_TEXTURE0 + 2);
        glUniform1i(normalTextureUniform, 2);
        glBindTexture(GL_TEXTURE_2D, material.normalTextureID);

        glUniform3fv(albedoColorUniform, 1, glm::value_ptr(material.albedoColor));
        glUniform1f(specularIntensityUniform, material.specularIntensity);
    }
} // core
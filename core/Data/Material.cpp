//
// Created by Gebruiker on 10/30/2026.
//

#include "Material.h"

namespace core::data
{
    Material::Material(MaterialType materialType, GLuint albedoTextureID, GLuint specularTextureID, GLuint normalTextureID, glm::vec3 albedoColor, float specularIntensity)
    {
        this->materialType = materialType;
        this->albedoTextureID = albedoTextureID;
        this->specularTextureID = specularTextureID;
        this->normalTextureID = normalTextureID;
        this->albedoColor = albedoColor;
        this->specularIntensity = specularIntensity;
    }

    Material::~Material() = default;
} // core
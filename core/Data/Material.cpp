//
// Created by Gebruiker on 10/30/2026.
//

#include "Material.h"

namespace core::data
{
    Material::Material(MaterialType materialType, GLuint albedoTextureID, GLuint specularTextureID, GLuint normalTextureID,  GLuint metalTextureID, GLuint roughnessTextureID, glm::vec3 albedoColor, float specularIntensity, float metallic, float roughness)
    {
        this->materialType = materialType;
        this->albedoTextureID = albedoTextureID;
        this->specularTextureID = specularTextureID;
        this->normalTextureID = normalTextureID;
        this->metalTextureID = metalTextureID;
        this->roughnessTextureID = roughnessTextureID;
        this->albedoColor = albedoColor;
        this->specularIntensity = specularIntensity;
        this->metallic = metallic;
        this->roughness = roughness;
    }

    Material::~Material() = default;
} // core
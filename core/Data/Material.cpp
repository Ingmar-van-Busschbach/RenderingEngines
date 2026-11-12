//
// Created by Gebruiker on 10/30/2026.
//

#include "Material.h"

namespace core::data
{
    Material::Material(MaterialType materialType, GLuint albedoTextureID, GLuint specularTextureID, GLuint normalTextureID,  GLuint metallicTextureID, GLuint roughnessTextureID, GLuint aoTextureID, GLuint emissiveTextureID, glm::vec3 albedoColor, float specularIntensity, float metallic, float roughness, glm::vec3 emissiveColor)
    {
        this->materialType = materialType;
        this->albedoTextureID = albedoTextureID;
        this->specularTextureID = specularTextureID;
        this->normalTextureID = normalTextureID;
        this->metallicTextureID = metallicTextureID;
        this->roughnessTextureID = roughnessTextureID;
        this->aoTextureID = aoTextureID;
        this->emissiveTextureID = emissiveTextureID;
        this->albedoColor = albedoColor;
        this->specularIntensity = specularIntensity;
        this->metallic = metallic;
        this->roughness = roughness;
        this->emissiveColor = emissiveColor;
    }

    Material::~Material() = default;
} // core
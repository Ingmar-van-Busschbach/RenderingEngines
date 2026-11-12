//
// Created by Gebruiker on 10/30/2026.
//

#pragma once

#include "texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
namespace core::data
{
    enum MaterialType
    {
        ADSWithTextures = 0,
        ADSWithoutTextures = 1,
        ADSWithOnlyAlbedoTexture = 2,
        PBRWithTextures = 3,
        PBRWithoutTextures = 4,
        PBRWithOnlyAlbedoTexture = 5,
        PBRWithOnlyAlbedoORMTexture = 6
    };
    class Material
    {
        public:
        MaterialType materialType;
        GLuint albedoTextureID;
        GLuint specularTextureID;
        GLuint normalTextureID;
        GLuint metallicTextureID;
        GLuint roughnessTextureID;
        GLuint aoTextureID;
        GLuint emissiveTextureID;

        glm::vec3 albedoColor;
        float specularIntensity;

        float metallic;
        float roughness;

        glm::vec3 emissiveColor;

        Material(MaterialType materialType, GLuint albedoTextureID, GLuint specularTextureID, GLuint normalTextureID, GLuint metallicTextureID, GLuint roughnessTextureID, GLuint aoTextureID, GLuint emissiveTextureID, glm::vec3 albedoColor = glm::vec3(0,0,0), float specularIntensity = 0, float metallic = 0, float roughness = 0, glm::vec3 emissiveColor = glm::vec3(0,0,0));
        ~Material();
    };
} // core

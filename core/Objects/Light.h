//
// Created by Gebruiker on 10/23/2026.
//

#pragma once

#include "glm/glm.hpp"

namespace core::objects
{
    enum LightType
    {
        Directional = 0,
        Point = 1,
        Spot = 2,
        Ambient = 3,
    };
    class Light
    {
        private:
        LightType lightType;
        glm::vec3 lightPosition;
        glm::vec3 lightDirection;
        glm::vec4 lightColor;
        float lightIntensity;
        glm::vec3 lightAttenuation;
        float spotLightAngle;
        float spotLightFalloff;

        public:
        Light(LightType lightType, glm::vec3 lightPosition, glm::vec3 lightDirection, glm::vec4 lightColor, float lightIntensity, glm::vec3 lightAttenuation = glm::vec3(0,0,0), float spotLightAngle = 0.0f, float spotLightFalloff = 0.0f);
        ~Light();
        int GetLightType() const;
        glm::vec3 GetLightPosition() const;
        glm::vec3 GetLightDirection() const;
        glm::vec4 GetLightColor() const;
        float GetLightIntensity() const;
        glm::vec3 GetLightAttenuation() const;
        float GetSpotLightAngle() const;
        float GetSpotLightFalloff() const;
    };
}


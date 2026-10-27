//
// Created by Gebruiker on 10/23/2026.
//

#include "Light.h"

namespace core::objects
{
    Light::Light(LightType lightType, glm::vec3 lightPosition, glm::vec3 lightDirection, glm::vec4 lightColor, float lightIntensity, glm::vec3 lightAttenuation, float spotLightAngle, float spotLightFalloff)
    {
        this->lightType = lightType;
        this->lightPosition = lightPosition;
        this->lightDirection = lightDirection;
        this->lightColor = lightColor;
        this->lightIntensity = lightIntensity;
        this->lightAttenuation = lightAttenuation;
        this->spotLightAngle = spotLightAngle;
        this->spotLightFalloff = spotLightFalloff;
    }
    Light::~Light() = default;
    int Light::GetLightType() const
    {
        return lightType;
    }
    glm::vec3 Light::GetLightPosition() const
    {
        return lightPosition;
    }
    glm::vec3 Light::GetLightDirection() const
    {
        return lightDirection;
    }
    glm::vec4 Light::GetLightColor() const
    {
        return lightColor;
    }
    float Light::GetLightIntensity() const
    {
        return lightIntensity;
    }
    glm::vec3 Light::GetLightAttenuation() const
    {
        return lightAttenuation;
    }
    float Light::GetSpotLightAngle() const
    {
        return spotLightAngle;
    }
    float Light::GetSpotLightFalloff() const
    {
        return spotLightFalloff;
    }
}
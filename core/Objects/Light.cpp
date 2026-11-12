//
// Created by Gebruiker on 10/23/2026.
//

#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>

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
        if (lightType == 0)
        {
            printf("Created Directional Light \n");
        }
        else if (lightType == 1)
        {
            printf("Created Point Light \n");
        }
        else if (lightType == 2)
        {
            printf("Created Spot Light \n");
        }
        else if (lightType == 3)
        {
            printf("Created Ambient Light \n");
        }
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

    void Light::SetPosition(glm::vec3 position)
    {
        this->lightPosition = position;
    }
    void Light::Translate(glm::vec3 direction)
    {
        this->lightPosition = this->lightPosition + direction;
    }

    void Light::SetDirection(glm::vec3 direction)
    {
        this->lightDirection = direction;
    }
}

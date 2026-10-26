//
// Created by Gebruiker on 10/23/2026.
//

#pragma once



namespace core
{
    class UserConfig
    {
    public:
        static float cameraRotationSpeed;
        static float cameraTranslationSpeed;
        static float cameraBoostMultiplier;
    };
}
float core::UserConfig::cameraTranslationSpeed = 5.0f;
float core::UserConfig::cameraRotationSpeed = 25.0f;
float core::UserConfig::cameraBoostMultiplier = 3.0f;
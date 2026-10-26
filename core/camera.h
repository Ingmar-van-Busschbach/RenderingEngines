//
// Created by Gebruiker on 10/9/2026.
//

#pragma once

#include "glm/glm.hpp"

namespace core
{
    class Camera
    {
        public:
        glm::vec3 cameraPos;
        glm::vec3 cameraTarget;
        glm::vec3 cameraForward;
        glm::vec3 cameraRight;
        glm::vec3 cameraUp;
        glm::vec3 rotation;
        glm::mat4 modelMatrix;
        Camera(glm::vec3 position, glm::vec3 target);
        void UpdateAxis();
        void Translate(glm::vec3 translation);
        void Rotate(float x, float y, float z);

        private:
        glm::mat4 translationMatrix;
        glm::mat4 rotationMatrix;
    };
} // core
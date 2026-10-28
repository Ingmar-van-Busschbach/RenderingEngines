//
// Created by Gebruiker on 10/9/2026.
//

#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "glm/glm.hpp"
namespace core::objects
{
    Camera::Camera(glm::vec3 position, glm::vec3 target)
    {
        cameraPos = position;
        cameraTarget = target;
        cameraForward = glm::normalize(cameraPos - cameraTarget);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        cameraRight = glm::normalize(glm::cross(up, cameraForward));
        cameraUp = glm::cross(cameraForward, cameraRight);
        rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        translationMatrix = glm::mat4(1.0f);
        translationMatrix = glm::translate(translationMatrix, -position);
        rotationMatrix = glm::mat4(1.0f);
        modelMatrix = rotationMatrix * translationMatrix;
    }

    void Camera::UpdateAxis()
    {
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        cameraRight = glm::normalize(glm::cross(up, cameraForward));
        cameraUp = glm::cross(cameraForward, cameraRight);
    }

    void Camera::Translate(glm::vec3 translation)
    {
        glm::vec4 translation4 = glm::vec4(translation, 1.0f);
        translation4 = translation4 * rotationMatrix;
        translationMatrix = glm::translate(translationMatrix, glm::vec3(translation4.x, translation4.y, translation4.z));
        modelMatrix = rotationMatrix * translationMatrix;
    }


    void Camera::Rotate(float x, float y, float z)
    {
        rotation += glm::vec3(x, y, z);
        rotationMatrix = glm::mat4(1.0f);
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), cameraForward);
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), cameraRight);
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), cameraUp);
        modelMatrix = rotationMatrix * translationMatrix;
    }
} // core
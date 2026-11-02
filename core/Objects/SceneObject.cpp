//
// Created by Gebruiker on 11/2/2026.
//

#include "SceneObject.h"

namespace core::objects
{
    SceneObject::SceneObject(Model* model, data::Material* material, int shaderProgramIndex)
    {
        this->model = model;
        this->material = material;
        this->shaderProgramIndex = shaderProgramIndex;
    }

    SceneObject::~SceneObject() = default;
} // core
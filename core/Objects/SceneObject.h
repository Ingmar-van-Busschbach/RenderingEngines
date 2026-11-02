//
// Created by Gebruiker on 11/2/2026.
//

#pragma once
#include "model.h"
#include "../Data/Material.h"

namespace core::objects
{
    class SceneObject
    {
        public:
        Model* model;
        data::Material* material;
        int shaderProgramIndex;

        SceneObject(Model* model, data::Material* material, int shaderProgramIndex);
        ~SceneObject();
    };
} //core

//
// Created by Gebruiker on 11/2/2026.
//

#pragma once
#include <vector>
#include "Objects/SceneObject.h"
#include "Objects/Light.h"

namespace core
{
    class Scene
    {
    public:
        std::vector<objects::SceneObject*> sceneObjects;
        std::vector<objects::Light*> lights;

        Scene();
        ~Scene();
    };
} // core

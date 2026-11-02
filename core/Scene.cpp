//
// Created by Gebruiker on 11/2/2026.
//

#include "Scene.h"

namespace core
{
    Scene::Scene()
    {

    }
    Scene::~Scene()
    {
        this->sceneObjects.clear();
        this->lights.clear();
    }
} // core
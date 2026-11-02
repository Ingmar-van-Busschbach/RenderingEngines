//
// Created by Gebruiker on 11/2/2026.
//

#include "Scene.h"

namespace core
{
    Scene::Scene()
    {
        printf("Initialized Empty Scene\n");
    }
    Scene::Scene(std::vector<objects::SceneObject*> sceneObjects, std::vector<objects::Light*> lights)
    {
        this->sceneObjects = sceneObjects;
        this->lights = lights;
        printf("Initialized Scene with %i objects and %i lights\n", sceneObjects.size(), lights.size());
    }

    Scene::~Scene()
    {
        this->sceneObjects.clear();
        this->lights.clear();
    }
} // core
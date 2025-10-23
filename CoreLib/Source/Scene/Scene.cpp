#include "Scene.h"

namespace WW {
    Scene::Scene()
    {
    }
    
    Scene::~Scene()
    {
        objects.clear();
    }
    
    void Scene::AddObject(std::shared_ptr<Object> o)
    {
        objects.push_back(o);
    }

    std::shared_ptr<Object> Scene::CreateObject(const std::string &name)
    {
        auto obj = std::make_shared<Object>(name);
        AddObject(obj);
        return obj;
    }

    void Scene::Render()
    {
        for(auto& obj : objects) 
        {
            obj->Render();
        }
    }
}
#pragma once

#include "Scene/Scene.h"
#include <memory>

namespace YAML
{
    class Emitter;
}

namespace WW
{
    class SceneSerializer
    {
    public:
        SceneSerializer(std::shared_ptr<Scene> scene);
        ~SceneSerializer() = default;

        void Serialize(const std::string& filePath);
        void Deserialize(const std::string& filePath);

    private:
        std::shared_ptr<Scene> scene;
    };
}
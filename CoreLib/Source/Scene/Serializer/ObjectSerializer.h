#pragma once

#include "Scene/Object.h"

namespace YAML
{
    class Emitter;
    class Node;
}

namespace WW
{
    class ObjectSerializer
    {
    public:
        ObjectSerializer(std::shared_ptr<Object> object);
        ~ObjectSerializer() = default;

        void Serialize(YAML::Emitter &out);
        void Deserialize(YAML::Node& node);

    private:
        std::shared_ptr<Object> object;
    };
}
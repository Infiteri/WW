#pragma once

#include "Scene/Object.h"

namespace YAML
{
    class Emitter;
    class Node;
}

namespace WW
{
    class ComponentSerializer
    {
    public:
        ComponentSerializer(std::shared_ptr<Object> object);
        ~ComponentSerializer() = default;

        void Serialize(YAML::Emitter& out);
        void Deserialize(YAML::Node& node);

    private:
        std::shared_ptr<Object> object;

        // keeps track of how many components are of each kind
        struct Count {
            int BackgroundLayer = 0;
            int Shape = 0;
        };
        Count count;

        void SerializeComponentCount(YAML::Emitter& out);
    };
}
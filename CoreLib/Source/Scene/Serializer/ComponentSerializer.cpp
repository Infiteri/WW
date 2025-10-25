#include "ComponentSerializer.h"
#include "SerializerUtils.h"

#include <yaml-cpp/yaml.h>

#define GET_COMPONENT_COUNT(type) count.type = object->GetComponents<type##Component>().size()

#define SERIALIZE_COMPONENT_CALLBACK(type)                                                         \
    {                                                                                              \
        auto list = object->GetComponents<type##Component>();                                      \
        for (int i = 0; i < list.size(); i++)                                                      \
            Serialize##type##Component(out, list[i], i);                                           \
    }

namespace WW
{
    static int GetNodeCount(const std::string &name, YAML::Node node)
    {
        std::string fmt = name + "Count";
        if (node[fmt])
            return node[fmt].as<int>();
        else
            return 0;
    };

    static void SerializeBackgroundLayerComponent(
        YAML::Emitter &out, std::shared_ptr<BackgroundLayerComponent> component, int index)
    {
        out << YAML::Key << "BackgroundLayerComponent " + std::to_string(index);
        out << YAML::BeginMap;
        out << YAML::EndMap;
    }

    static void SerializeShapeComponent(YAML::Emitter &out,
                                        std::shared_ptr<ShapeComponent> component, int index)
    {
        out << YAML::Key << "ShapeComponent " + std::to_string(index);
        out << YAML::BeginMap;
        SERIALIZE_FIELD("Type", (int)component->GetType());
        out << YAML::Key << "Transform";
        SerializerUtils::SerializeTransform(out, component->GetTransform());

        out << YAML::EndMap;
    }

    ComponentSerializer::ComponentSerializer(std::shared_ptr<Object> object) : object(object) {}

    void ComponentSerializer::Serialize(YAML::Emitter &out)
    {
        if (!object)
            return;

        SerializeComponentCount(out);

        SERIALIZE_COMPONENT_CALLBACK(BackgroundLayer);
        SERIALIZE_COMPONENT_CALLBACK(Shape);
    }

    void ComponentSerializer::Deserialize(YAML::Node &node) {}

    void ComponentSerializer::SerializeComponentCount(YAML::Emitter &out)
    {
        GET_COMPONENT_COUNT(BackgroundLayer);
        GET_COMPONENT_COUNT(Shape);

        // serialize
        SERIALIZE_FIELD("BackgroundLayerCount", count.BackgroundLayer);
        SERIALIZE_FIELD("ShapeCount", count.Shape);
    }
} // namespace WW

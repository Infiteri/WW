#include "ObjectSerializer.h"
#include "ComponentSerializer.h"
#include "SerializerUtils.h"

#include <yaml-cpp/yaml.h>

namespace WW
{
    ObjectSerializer::ObjectSerializer(std::shared_ptr<Object> object)
        : object(object)
    {
    }

    void ObjectSerializer::Serialize(YAML::Emitter &out)
    {
        if (!object)
            return;

        out << YAML::BeginMap;

        SERIALIZE_FIELD("Name", object->GetName().c_str());
        SERIALIZE_FIELD("UUID", object->GetID().Get());

        ComponentSerializer componentSerializer(object);
        componentSerializer.Serialize(out);

        out << YAML::EndMap;
    }

    void ObjectSerializer::Deserialize(YAML::Node& node)
    {
    }
}
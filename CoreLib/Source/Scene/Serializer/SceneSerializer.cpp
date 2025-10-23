#include "SceneSerializer.h"
#include "ObjectSerializer.h"
#include "SerializerUtils.h"

#include <yaml-cpp/yaml.h>

namespace WW
{
    SceneSerializer::SceneSerializer(std::shared_ptr<Scene> scene)
        : scene(scene)
    {
    }

    void SceneSerializer::Serialize(const std::string &filePath)
    {
        if (!scene)
            return;

        YAML::Emitter out;
        out << YAML::BeginMap;
        SERIALIZE_FIELD("Scene", YAML::BeginMap);

        // serialize objects
        SERIALIZE_FIELD("Objects", YAML::BeginSeq);
        for (auto &obj : scene->GetObjects())
        {
            ObjectSerializer objSerializer(obj);
            objSerializer.Serialize(out);
        }

        out << YAML::EndMap;

        SerializerUtils::SaveEmitterToFile(out, filePath);
    }

    void SceneSerializer::Deserialize(const std::string &filePath)
    {
    }

}
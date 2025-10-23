#include "SerializerUtils.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace WW
{
    namespace SerializerUtils
    {
        void SaveEmitterToFile(YAML::Emitter &em, const std::string &path)
        {
            std::ofstream stream{path};
            stream << em.c_str();
        }

        void SerializeTransform(YAML::Emitter &em, const Transform &t)
        {
            em << YAML::Value << YAML::Flow;
            em << YAML::BeginSeq << t.Position.x << t.Position.y << t.Position.z;
            em << t.Rotation.x << t.Rotation.y << t.Rotation.z;
            em << t.Scale.x << t.Scale.y << t.Scale.z << YAML::EndSeq;
        }
    }
}
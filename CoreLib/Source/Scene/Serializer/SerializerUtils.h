#pragma once

#include "Math/Transform.h"
#include <string>

#define SERIALIZE_FIELD(name, value) out << YAML::Key << name << YAML::Value << value

namespace YAML
{
    class Emitter;
}

namespace WW
{
    namespace SerializerUtils
    {
        void SaveEmitterToFile(YAML::Emitter &em, const std::string &path);
        void SerializeTransform(YAML::Emitter &em, const Transform& t)
    ;}
}
#pragma once

#include "Color.h"
#include <string>

namespace WW
{
    struct Material
    {
        Color Color;
        std::string ColorTexturePath;

        Material() : Color(255, 255, 255, 255), ColorTexturePath("") {}
    };
}
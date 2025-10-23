#include "Object.h"

namespace WW
{
    Object::Object(const std::string &name)
        : name(name)
    {
    }

    Object::~Object()
    {
        components.clear();
    }

    void Object::Render()
    {
        for (auto &c : components)
        {
            c->Render();
        }
    }
}
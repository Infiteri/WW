#pragma once

#include "Math/Transform.h"
#include "Components.h"
#include "Core/UUID.h"

#include <string>
#include <vector>

namespace WW
{
    class Object
    {
    public:
        Object(const std::string &name = "New Object");
        ~Object();

        inline UUID GetID() const { return id; };

        void Render();

        inline Transform &GetTransform() { return transform; };
        inline const std::string &GetName() const { return name; };

        template <typename T, typename... Args>
        std::shared_ptr<T> AddComponent(Args... args)
        {
            std::shared_ptr<T> c = std::make_shared<T>(args...);
            c->owner = this;
            components.push_back(c);
            return c;
        };

        template <typename T>
        T *GetComponent()
        {
            for (auto comp : components)
            {
                T *tc = dynamic_cast<T *>(comp);
                if (tc)
                    return tc;
            }

            return nullptr;
        };

        template <typename T>
        std::vector<std::shared_ptr<T>> GetComponents()
        {
            std::vector<std::shared_ptr<T>> comps;

            if (!components.data())
            {
                return comps;
            }

            for (auto &comp : components)
            {
                if (!comp)
                    continue; // skip null pointers

                auto tc = std::dynamic_pointer_cast<T>(comp);
                if (tc)
                    comps.push_back(tc);
            }

            return comps;
        }

        template <typename T>
        bool HasComponent() { return GetComponent<T>() != nullptr; };

        template <typename T>
        void RemoveComponent(int index = 0)
        {
            int currentIndex = -1;
            for (auto it = components.begin(); it != components.end(); it++)
            {
                T *tc = dynamic_cast<T *>(*it);

                if (tc)
                {
                    currentIndex++;

                    if (currentIndex == index)
                    {
                        delete tc;
                        components.erase(it);
                        break;
                    }
                }
            }
        };

        template <typename T>
        void RemoveComponents()
        {
            for (auto it = components.begin(); it != components.end(); it++)
            {
                T *tc = dynamic_cast<T *>(*it);

                if (tc)
                {
                    delete tc;
                    components.erase(it);
                }
            }
        };

    private:
        UUID id;
        Transform transform;
        std::string name;
        std::vector<std::shared_ptr<Component>> components;
    };
}
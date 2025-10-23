#pragma once

#include "BackgroundLayer.h"
#include "Material.h"
#include "Math/Transform.h"
#include <memory>

namespace WW
{
    class Object;
    class Component
    {
    protected:
        Object *owner;
        friend class Object;

    public:
        Component() {};
        virtual ~Component() {};

        inline Object *GetOwner() { return owner; };

        virtual void Render() {};

        virtual void From(Component *comp) {}
    };

    class BackgroundLayerComponent : public Component
    {
    public:
        BackgroundLayerComponent();
        ~BackgroundLayerComponent() = default;

        void Render();

        inline BackgroundLayer *GetLayer() { return layer.get(); };

    private:
        std::unique_ptr<BackgroundLayer> layer;
    };

    enum class ShapeType
    {
        Box
    };

    class ShapeComponent : public Component
    {
    public:
        ShapeComponent(ShapeType shapeType = ShapeType::Box);
        ~ShapeComponent();

        void SetType(ShapeType type);
        void Render();

        inline ShapeType GetType() const { return type; };
        inline Transform &GetTransform() { return transform; };
        inline Material &GetMaterial() { return material; };

        void SetBoxSize(const Vector3& size);

    private:
        ShapeType type;
        Transform transform;
        Material material;

        void ResetValuesTo(float val);

        static const int dataFieldCount = 3;

        // note: these floats represent all data needed by the shape
        union
        {
            float data[dataFieldCount];
            Vector3 boxSize;
        };
    };
}
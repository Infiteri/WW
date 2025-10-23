#include "Components.h"
#include "Renderer.h"

namespace WW
{
    BackgroundLayerComponent::BackgroundLayerComponent()
    {
        layer = std::make_unique<BackgroundLayer>();
    }

    void BackgroundLayerComponent::Render()
    {
        layer->Render();
    }

    ShapeComponent::ShapeComponent(ShapeType shapeType)
    {
        SetType(shapeType);
    }

    ShapeComponent::~ShapeComponent()
    {
    }

    void ShapeComponent::SetType(ShapeType type)
    {
        ResetValuesTo(1);
        this->type = type;
    }

    void ShapeComponent::Render()
    {
        switch (type)
        {
        case ShapeType::Box:
        {
            Vector3 scale = transform.Scale;
            transform.Scale = transform.Scale * boxSize;
            Renderer::RenderCube(transform, material);
            transform.Scale = scale;
        }
        break;
        }
    }

    void ShapeComponent::ResetValuesTo(float val)
    {
        for (int i = 0; i < dataFieldCount; i++)
            data[i] = val;
    }

    void ShapeComponent::SetBoxSize(const Vector3 &size)
    {
        boxSize = size;
    }

}
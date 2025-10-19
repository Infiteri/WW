#pragma once

namespace WW
{
    class IDisplay
    {
    public:
        IDisplay() = default;
        virtual ~IDisplay() = default;

        virtual void Init() = 0;
        virtual void PreRender() = 0;
        virtual void PostRender() = 0;
        virtual void Shutdown() = 0;
        virtual bool WantsUpdate() = 0;
    };
} // namespace WW

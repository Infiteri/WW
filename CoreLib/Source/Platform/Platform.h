#pragma once

namespace WW
{
    namespace Platform
    {

        struct State
        {
            float TotalTime = 0.0f;
        };

        void UpdateTotalTime();
        float GetDeltaTime();
        float GetTotalTime();
    }
}
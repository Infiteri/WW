#include "Platform.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <chrono>
#endif

namespace WW
{
    namespace Platform
    {
        static State state;

        void UpdateTotalTime()
        {
            state.TotalTime += GetDeltaTime();
        }

#ifdef _WIN32
        float GetDeltaTime()
        {
            static LARGE_INTEGER frequency;
            static LARGE_INTEGER lastTime;
            static bool initialized = false;

            if (!initialized)
            {
                QueryPerformanceFrequency(&frequency);
                QueryPerformanceCounter(&lastTime);
                initialized = true;
                return 0.0f;
            }

            LARGE_INTEGER now;
            QueryPerformanceCounter(&now);

            float delta = static_cast<float>(now.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
            lastTime = now;

            return delta;
        }
        #else
        float GetDeltaTime()
        {
            using clock = std::chrono::high_resolution_clock;
            static auto lastTime = clock::now();
            
            auto now = clock::now();
            std::chrono::duration<float> delta = now - lastTime;
            lastTime = now;
            
            return delta.count();
        }
        #endif
        
        float GetTotalTime()
        {
            return state.TotalTime;
        }

    } // namespace Platform
} // namespace WW

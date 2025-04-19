#include "Encin/Encin.h"

namespace GamEncin
{
    namespace Time
    {
#pragma region ScopedIntervalTimer

        ScopedIntervalTimer::ScopedIntervalTimer(const string label)
        {
            this->label = label;
            start = std::chrono::high_resolution_clock::now();
        }

        ScopedIntervalTimer::~ScopedIntervalTimer()
        {
            PrintPastTime();
        }

        void ScopedIntervalTimer::PrintPastTime(string addNote)
        {
            auto endTime = std::chrono::high_resolution_clock::now();
            double durationMs = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - start).count() / 1000000.0;
            printf("%s : %s : %f miliseconds\n", label.c_str(), addNote.c_str(), durationMs);
            start = endTime;
        }

#pragma endregion

#pragma region Timer

        Timer::Timer(const string label)
        {
            this->label = label;
            Start();
        }

        void Timer::Start()
        {
            if(isRunning)
            {
                return;
            }

            start = std::chrono::high_resolution_clock::now();
            isRunning = true;
        }

        void Timer::Stop()
        {
            if(!isRunning)
            {
                return;
            }

            end = std::chrono::high_resolution_clock::now();
            isRunning = false;
        }

        void Timer::Reset()
        {
            Stop();
            Start();
        }

        void Timer::PrintPastTime(string addNote)
        {
            auto endTime = isRunning ? std::chrono::high_resolution_clock::now() : end;
            double durationMs = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - start).count() / 1000000.0;
            printf("%s : %s : %f miliseconds\n", label.c_str(), addNote.c_str(), durationMs);
        }

#pragma endregion
    }
}

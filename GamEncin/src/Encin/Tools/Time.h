#pragma once
#include "Encin/Tools/MathYaman.h"

#include <chrono>

namespace GamEncin
{
    namespace Time
    {
        /// <summary>
        /// A simple timer for limited scopes. Create it as a local variable where you want to start the timer and it will automatically print the time taken by the scope when the scope is terminated. Printing intervals will reset the timer. Simply use PrintPastTime() to print the time taken since the last print.
        /// </summary>
        /// <param name="label:">The label of the timer. Name shown while printing.</param>
        class ScopedIntervalTimer
        {
        public:
            ScopedIntervalTimer(const string label = "Default Scoped Timer");
            ~ScopedIntervalTimer();

            void PrintPastTime(string addNote = "");

        private:
            string label;
            std::chrono::high_resolution_clock::time_point start;
        };

        /// <summary>
        /// A general, multi purpuse timer. Can be started, stopped, reset and printed. Starts when created.
        /// </summary>
        /// <param name="label:">The label of the timer. Name shown while printing.</param>
        class Timer
        {
        public:
            Timer(const string label = "Default Timer");

            void Start();
            void Stop();
            void Reset();
            void PrintPastTime(string addNote = "");

        private:
            string label;
            std::chrono::high_resolution_clock::time_point start;
            std::chrono::high_resolution_clock::time_point end;
            bool isRunning = false;
        };
    }
}

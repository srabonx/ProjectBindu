#pragma once

#include <random>
#include <iostream>
#include <chrono>

namespace BINDU
{
    struct RandomNumber
    {
    public:
        static unsigned int Get(int min = 0, int max = 429496729)
        {
            srand(static_cast <unsigned int> (std::chrono::high_resolution_clock::now().time_since_epoch().count()));
            int seed = rand();
            static std::default_random_engine m_randomEngine(seed);

            return min + (m_randomEngine() % (max + 1 - min));
        }
    }; // Struct
};// Namespace

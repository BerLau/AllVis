#pragma once
#ifndef CORE_RANDOM_H
#define CORE_RANDOM_H

#include <random>
#include "config.h"

namespace Core::Math
{

    template <typename T>
    T random_s(T min, T max)
    {
        static std::mt19937 gen(Config::random_seed);
        // if is real value
        if (std::is_floating_point_v<T>)
        {
            std::uniform_real_distribution<T> dis(min, max);
            return dis(gen);
        }
        // if is integer value
        else if (std::is_integral_v<T>)
        {
            std::uniform_int_distribution<T> dis(min, max);
            return dis(gen);
        }
        else
        {
            throw std::invalid_argument("random_s: invalid type");
        }
    }
    template <typename T>
    T random_s(T max)
    {
        return random_s<T>(0, max);
    }
    template <typename T>
    T random_s(int seed)
    {
        return random_s<T>(0, 1);
    }

    template <typename T>
    T random(T min, T max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        // if is real value
        if constexpr (std::is_floating_point_v<T>)
        {
            std::uniform_real_distribution<T> dis(min, max);
            return dis(gen);
        }
        // if is integer value
        else if constexpr (std::is_integral_v<T>)
        {
            std::uniform_int_distribution<T> dis(min, max);
            return dis(gen);
        }
        else
        {
            throw std::invalid_argument("random: invalid type");
        }
    }

    template <typename T>
    T random(T max)
    {
        return random<T>(0, max);
    }

    template <typename T>
    T random()
    {
        return random<T>(0, 1);
    }

} // namespace  Core::Random

#endif // CORE_RANDOM_H
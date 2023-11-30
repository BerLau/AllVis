#pragma once
#ifndef MATH_BASE_H
#define MATH_BASE_H

#include <stdexcept>
#include <constants.h>

namespace Core
{

    namespace Math
    {
        template <typename T>
        int sign(T x) { return (x > 0) - (x < 0); }

        template <typename T>
        T abs(T x) { return x * sign(x); }

        template <typename T>
        double pow(T x, int n);

        double sqrt(double y);

        template <typename T>
        bool equal(T x, T y);
        template <typename T>
        T clamp(T x, T min, T max)
        {
            // optimized version
            return x < min ? min : (x > max ? max : x);
        }

        /*---------------------------Implementation---------------------------*/
        /*====================================================================*/

        template <typename T>
        double pow(T x, int n)
        {
            double rslt = 1;
            int pos_exp = abs(n);
            while (pos_exp > 0)
            {
                if (pos_exp & 1) // if pos_exp is odd
                {
                    rslt *= x;
                }
                x *= x;
                pos_exp >>= 1;
            }

            return n < 0 ? 1.0 / rslt : rslt;
        };
        template <typename T>
        bool equal(T x, T y)
        {
            // if T is double
            if constexpr (std::is_same_v<T, double>)
            {
                return abs(x - y) < Constants::EPSILON;
            }
            // if T is float
            else if constexpr (std::is_same_v<T, float>)
            {
                return abs(x - y) < Constants::EPSILON_F;
            }
            // if T is arithmetic type
            else if constexpr (std::is_arithmetic_v<T>)
            {
                return x == y;
            }
        };

    }; // namespace math
};
#endif // MATH_ARITHMETICS_H
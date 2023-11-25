#pragma once
#ifndef MATH_BASE_H
#define MATH_BASE_H

#include <stdexcept>

namespace Core
{

    namespace Math
    {
        template <typename T>
        int sign(T x)
        {
            return (x > 0) - (x < 0);
        }

        template <typename T>
        T abs(T x)
        {
            return x * sign(x);
        }

        double sqrt(double y)
        {
            if (y < 0)
                throw std::domain_error("sqrt() argument must be non-negative");
            // Newton-Raphson method
            // let f(x) = x^2 - y
            // f'(x) = 2x
            // tangent line at x_n: y = f'(x_n)(x - x_n) + f(x_n)
            // calculate x_(n+1) where y = 0: 0 = f'(x_n)(x_(n+1) - x_n) + f(x_n)
            // x_(n+1) = x_n - f(x_n) / f'(x_n)
            // x_(n+1) = x_n - (x_n^2 - y) / 2x_n
            // x_(n+1) = (x_n + y / x_n) / 2
            double x = y;
            double e = 1e-15;
            while (abs(x * x - y) > e)
            {
                x = (x + y / x) / 2;
            }
            return x;
        }

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
        }
    }; // namespace math
};
#endif // MATH_ARITHMETICS_H
#include "math/base.h"

double Core::Math::sqrt(double y)
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

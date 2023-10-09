#pragma once
#ifndef MATH_ARITHMETICS_H
#define MATH_ARITHMETICS_H

#include <cmath>

namespace Arithmetics
{
    template <typename T>
    int sign(T x)
    {
        return (x > 0) - (x < 0);
    }

} // namespace math

#endif // MATH_ARITHMETICS_H
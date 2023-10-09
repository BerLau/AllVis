#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

#include <cmath>
#define FLOAT_EPSILON 1e-6
#define EQUAL_F(a, b) (fabsf((a) - (b)) < FLOAT_EPSILON)
#define DOUBLE_EPSILON 1e-12
#define EQUAL_D(a, b) (fabs((a) - (b)) < DOUBLE_EPSILON)

namespace Core
{
    const int FAILURE = 0;
    const int SUCCESS = 1;

}; // namespace GL

#endif
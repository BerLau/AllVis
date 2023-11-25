#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

#include <cmath>
#define FLOAT_EPSILON 1e-6
#define EQUAL_F(a, b) (fabsf((a) - (b)) <= FLOAT_EPSILON)
#define DOUBLE_EPSILON 1e-12
#define EQUAL_D(a, b) (fabs((a) - (b)) <= DOUBLE_EPSILON)

#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

#define RANDOM(max) (rand() % (max)) // [0, max) max has to be greater than 0
#define RANDOM_RANGE(min, max) ((min) + (rand() % ((max) - (min))))

#define RANDOM_F(max) ((float)rand() / (float)(RAND_MAX / (max))) // [0, max) max has to be greater than 0
#define RANDOM_RANGE_F(min, max) ((min) + ((float)rand() / (float)(RAND_MAX / ((max) - (min)))))

namespace Core
{
    const int FAILURE = 0;
    const int SUCCESS = 1;

}; // namespace GL

#endif
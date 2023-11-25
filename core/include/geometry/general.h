#pragma once
#ifndef CORE_GEOMETRY_GENERAL_H
#define CORE_GEOMETRY_GENERAL_H
#include "vector.h"

namespace Core
{
    namespace Geometry
    {
        float radians(float degrees);
        float degrees(float radians);
        Core::Vector degrees(const Core::Vector &radians);
        Core::Vector radians(const Core::Vector &degrees);
        float distance(const Core::Vector &v1, const Core::Vector &v2);
        Core::Vector normalize(const Core::Vector &vector);
    };
    struct EulerAngle
    {
        float pitch;
        float yaw;
        float roll;
    };
};

#endif
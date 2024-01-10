#pragma once
#ifndef CORE_GEOMETRY_GENERAL_H
#define CORE_GEOMETRY_GENERAL_H
#include "vector.h"

namespace Core
{
    struct EulerAngle
    {
        float pitch;
        float yaw;
        float roll;
    };

    struct Line
    {
        Core::Vector origin;
        Core::Vector direction;
    };

    struct LineSegment
    {
        Core::Vector start;
        Core::Vector end;
    };

    namespace Geometry
    {
        float radians(float degrees);
        float degrees(float radians);
        Core::Vector degrees(const Core::Vector &radians);
        Core::Vector radians(const Core::Vector &degrees);
        float distance(const Core::Vector &v1, const Core::Vector &v2);
        Core::Vector normalize(const Core::Vector &vector);
        float dist_point_line(const Core::Vector &p, const Line& line);
        float dist_point_line_segment(const Core::Vector &p, const LineSegment& line_segment);
    };

};

#endif
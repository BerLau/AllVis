#pragma once
#ifndef CORE_GEOMETRY_2D_H
#define CORE_GEOMETRY_2D_H

#include "vector.h"

namespace Core
{
    namespace Geometry
    {
        int point_line_position(const Core::Vector2 &p, const Core::Vector2 &a, const Core::Vector2 b);
        bool point_in_triangle(const Core::Vector2 &p, const Core::Vector2 &a, const Core::Vector2 &b, const Core::Vector2 &c);
    }

}

#endif // GEOMETRY_H
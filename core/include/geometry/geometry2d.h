#pragma once
#ifndef CORE_GEOMETRY_2D_H
#define CORE_GEOMETRY_2D_H

#include "vector.h"

namespace Core::Geometry
{
    int point_line_position(const Core::Vector2 &p, const Core::Vector2 &a, const Core::Vector2 b);
    double distance_point_line(const Core::Vector2 &p, const Core::Vector2 &a, const Core::Vector2 &b);
    double area_triangle(const Core::Vector2 &a, const Core::Vector2 &b, const Core::Vector2 &c);
    bool point_in_triangle(const Core::Vector2 &p, const Core::Vector2 &a, const Core::Vector2 &b, const Core::Vector2 &c);
    bool point_in_convex_polygon(const Core::Vector2 &p, Core::Vector2 *vertices, unsigned int num_vertices);

}; // namespace Core::Geometry

#endif // GEOMETRY_H
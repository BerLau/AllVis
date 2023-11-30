#include "geometry/general.h"
#include "geometry/geometry2d.h"
#include <cmath>
#include "math/base.h"
#include "vector.h"

namespace Core::Geometry
{
    int point_line_position(const Core::Vector2 &p, const Core::Vector2 &a, const Core::Vector2 b)
    {
        //  return the cross product of ab and ap
        return Math::sign((b.x() - a.x()) * (p.y() - a.y()) - (p.x() - a.x()) * (b.y() - a.y()));
    }

    double distance_point_line(const Core::Vector2 &p, const Core::Vector2 &a, const Core::Vector2 &b)
    {
        //  return the cross product of ab and ap
        return std::abs((b.x() - a.x()) * (p.y() - a.y()) - (p.x() - a.x()) * (b.y() - a.y())) / Geometry::distance(a, b);
    }

    double area_triangle(const Core::Vector2 &a, const Core::Vector2 &b, const Core::Vector2 &c)
    {
        //  return the 0.5 * | ab x ac|
        return 0.5 * std::abs((b.x() - a.x()) * (c.y() - a.y()) - (c.x() - a.x()) * (b.y() - a.y()));
    }

    bool point_in_triangle(const Core::Vector2 &p, const Core::Vector2 &a, const Core::Vector2 &b, const Core::Vector2 &c)
    {
        Core::Vector2 pa = a-p;
        Core::Vector2 pb = b-p;
        Core::Vector2 pc = c-p;
        double t1 = Core::Vector2::cross(pa, pb);
        double t2 = Core::Vector2::cross(pb, pc);
        double t3 = Core::Vector2::cross(pc, pa);
        return (t1 * t2 >= 0) && (t2 * t3 >= 0) && (t3 * t1 >= 0);

    }
}; // namespace Core::Geometry

#include "geometry/general.h"
#include <cmath>
#include "quaternion.h"

namespace Core::Geometry
{
    float radians(float degrees)
    {
        return degrees * (float(M_PI) / 180.0f);
    }

    float degrees(float radians)
    {
        return radians * (float(180.0f) / M_PI);
    }

    Core::Vector degrees(const Core::Vector &radians)
    {
        Core::Vector rslt(radians.size());
        for (size_t i = 0; i < radians.size(); ++i)
        {
            rslt[i] = degrees(radians[i]);
        }
        return rslt;
    }

    Core::Vector radians(const Core::Vector &degrees)
    {
        Core::Vector rslt(degrees.size());
        for (size_t i = 0; i < degrees.size(); ++i)
        {
            rslt[i] = radians(degrees[i]);
        }
        return rslt;
    }

    Core::Vector normalize(const Core::Vector &vector)
    {
        float length = std::sqrt(Core::Vector::dot(vector, vector));
        return vector * (1.0f / length);
    }

    float dist_point_line(const Core::Vector &p, const Line &line)
    {
        Core::Vector v = p - line.origin;
        return std::sqrt(Core::Vector::dot(v, v) - std::pow(Core::Vector::dot(v, line.direction), 2));
    }

    float dist_point_line_segment(const Core::Vector &p, const LineSegment &line_segment)
    {
        Core::Vector v = p - line_segment.start;
        Core::Vector w = line_segment.end - line_segment.start;
        float c1 = Core::Vector::dot(v, w);
        if (c1 <= 0)
            return v.length();
        float c2 = Core::Vector::dot(w, w);
        if (c2 <= c1)
            return Vector(p - line_segment.end).length();
        float b = c1 / c2;
        Core::Vector pb = line_segment.start + w * b;
        return Vector(p - pb).length();
    }
    Core::Quaternion normalize(const Core::Quaternion &quaternion)
    {
        float length = Core::Quaternion::dot(quaternion, quaternion);
        return quaternion * (1.0f / length);
    }

    Core::Quaternion angle_axis(float angle_rad, float x, float y, float z)
    {
        float half_angle = angle_rad * 0.5f;
        float sin_half_angle = std::sin(half_angle);
        return Core::Quaternion(cos(half_angle), x * sin_half_angle, y * sin_half_angle, z * sin_half_angle);
    }

    Core::Quaternion angle_axis(float angle_rad, const Core::Vector3 &axis)
    {
        return angle_axis(angle_rad, axis.x(), axis.y(), axis.z());
    }

    float distance(const Core::Vector &v1, const Core::Vector &v2)
    {
        return std::sqrt(Core::Vector::dot(v1 - v2, v1 - v2));
    }
}

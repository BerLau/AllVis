#include "geometry/general.h"
#include <cmath>
#include "global.h"
#include "quaternion.h"

namespace Geometry
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

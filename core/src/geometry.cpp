#include "geometry.h"
#include <cmath>

namespace Geometry
{
    float radians(float degrees)
    {
        return degrees * M_PI / 180;
    }

    float degrees(float radians)
    {
        return radians * 180 / M_PI;
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
        float length = sqrt(Core::Vector::dot(vector, vector));
        return vector * (1.0 / length);
    }

    Core::Quaternion normalize(const Core::Quaternion &quaternion)
    {
        float length = Core::Quaternion::dot(quaternion, quaternion);
        return quaternion * (1.0 / length);
    }

    Core::Vector3 cross(const Core::Vector3 &a, const Core::Vector3 &b)
    {
        return Core::Vector3(a.y() * b.z() - a.z() * b.y(),
                             a.z() * b.x() - a.x() * b.z(),
                             a.x() * b.y() - a.y() * b.x());
    }

    Core::Quaternion angle_axis(float angle_rad, float x, float y, float z)
    {
        float half_angle = angle_rad / 2;
        float sin_half_angle = sin(half_angle);
        return Core::Quaternion(x * sin_half_angle, y * sin_half_angle, z * sin_half_angle, cos(half_angle));
    }

    Core::Quaternion angle_axis(float angle_rad, const Core::Vector3 &axis)
    {
        return angle_axis(angle_rad, axis.x(), axis.y(), axis.z());
    }

    Core::Matrix4 translate(const Core::Matrix4 &matrix, float x, float y, float z)
    {
        Core::Matrix4 rslt = matrix;
        rslt(0, 3) += x;
        rslt(1, 3) += y;
        rslt(2, 3) += z;
        return rslt;
    }

    Core::Matrix4 translate(const Core::Matrix4 &matrix, const Core::Vector3 &translation)
    {
        return translate(matrix, translation.x(), translation.y(), translation.z());
    }

    Core::Matrix4 rotate(const Core::Matrix4 &matrix, float angle_rad, float x, float y, float z)
    {
        Core::Matrix4 rslt = matrix;
        float c = cos(angle_rad);
        float s = sin(angle_rad);
        float t = 1 - c;
        rslt(0, 0) = x * x * t + c;
        rslt(0, 1) = x * y * t - z * s;
        rslt(0, 2) = x * z * t + y * s;
        rslt(1, 0) = y * x * t + z * s;
        rslt(1, 1) = y * y * t + c;
        rslt(1, 2) = y * z * t - x * s;
        rslt(2, 0) = z * x * t - y * s;
        rslt(2, 1) = z * y * t + x * s;
        rslt(2, 2) = z * z * t + c;
        return rslt;
    }

    Core::Matrix4 rotate(const Core::Matrix4 &matrix, float angle_rad, Core::Vector3 axis)
    {
        axis = normalize(axis);
        return rotate(matrix, angle_rad, axis.x(), axis.y(), axis.z());
    }

    Core::Matrix4 scale(const Core::Matrix4 &matrix, float x, float y, float z)
    {
        Core::Matrix4 rslt = matrix;
        rslt(0, 0) *= x;
        rslt(1, 0) *= x;
        rslt(2, 0) *= x;

        rslt(0, 1) *= y;
        rslt(1, 1) *= y;
        rslt(2, 1) *= y;

        rslt(0, 2) *= z;
        rslt(1, 2) *= z;
        rslt(2, 2) *= z;

        return rslt;
    }

    Core::Matrix4 scale(const Core::Matrix4 &matrix, float scalar)
    {
        return scale(matrix, scalar, scalar, scalar);
    }

    Core::Matrix4 scale(const Core::Matrix4 &matrix, const Core::Vector3 &scalar)
    {
        return scale(matrix, scalar.x(), scalar.y(), scalar.z());
    }

    Core::Matrix4 look_at(const Core::Vector3 &eye, const Core::Vector3 &center, const Core::Vector3 &up)
    {
        Core::Vector3 f = normalize(center - eye);
        Core::Vector3 s = normalize(cross(f, up));
        Core::Vector3 u = cross(s, f);

        Core::Matrix4 rslt;
        rslt(0, 0) = s.x();
        rslt(0, 1) = s.y();
        rslt(0, 2) = s.z();
        rslt(0, 3) = -Core::Vector::dot(s, eye);
        rslt(1, 0) = u.x();
        rslt(1, 1) = u.y();
        rslt(1, 2) = u.z();
        rslt(1, 3) = -Core::Vector::dot(u, eye);
        rslt(2, 0) = -f.x();
        rslt(2, 1) = -f.y();
        rslt(2, 2) = -f.z();
        rslt(2, 3) = Core::Vector::dot(f, eye);
        rslt(3, 0) = 0;
        rslt(3, 1) = 0;
        rslt(3, 2) = 0;
        rslt(3, 3) = 1;
        return rslt;
    }
    Core::Matrix4 perspective(float fov_rad, float aspect, float near, float far)
    {
        Core::Matrix4 rslt;
        float f = 1.0 / tan(fov_rad / 2);
        rslt(0, 0) = f / aspect;
        rslt(1, 1) = f;
        rslt(2, 2) = (far + near) / (near - far);
        rslt(2, 3) = (2 * far * near) / (near - far);
        rslt(3, 2) = -1;
        rslt(3, 3) = 0;
        return rslt;
    }

    Core::Matrix4 orthographic(float left, float right, float bottom, float top, float near, float far)
    {
        Core::Matrix4 rslt;
        rslt(0, 0) = 2 / (right - left);
        rslt(1, 1) = 2 / (top - bottom);
        rslt(2, 2) = 2 / (near - far);
        rslt(0, 3) = (left + right) / (left - right);
        rslt(1, 3) = (bottom + top) / (bottom - top);
        rslt(2, 3) = (far + near) / (near - far); // ? check this
        rslt(3, 3) = 1;
        return rslt;
    }

    Core::Matrix4 orthographic(float left, float right, float bottom, float top)
    {
        return orthographic(left, right, bottom, top, -1, 1);
    }

    Core::Quaternion quat_look_at(const Core::Vector3 &direction, const Core::Vector3 &up, bool right_handed)
    {
        Core::Vector3 d = normalize(direction);
        if (right_handed)
        {
            d = -d;
        }
        Core::Vector3 r = normalize(cross(up, d));
        Core::Vector3 u = cross(d, r);
        float w = sqrt(1.0 + r.x() + u.y() + d.z()) / 2.0;
        float w4 = 4.0 * w;
        float x = (u.z() - d.y()) / w4;
        float y = (d.x() - r.z()) / w4;
        float z = (r.y() - u.x()) / w4;
        return Core::Quaternion(w, x, y, z);
    }

    Core::Quaternion quat_look_at(const Core::Vector3 &direction, const Core::Vector3 &up, const Core::Vector3 &right)
    {
        Core::Vector3 _r = Core::Vector3::cross(direction, up);
        if (Core::Vector3::dot(_r, right) < 0)
        {
            return quat_look_at(direction, up, true);
        }
        else
        {
            return quat_look_at(direction, up, false);
        }
    }

    void orthonomalize(Core::Vector3 &up, Core::Vector3 &front, Core::Vector3 &right)
    {
        up = normalize(up);
        front = normalize(front);
        right = normalize(right);
        right = normalize(cross(front, up));
        up = cross(right, front);
    }
}

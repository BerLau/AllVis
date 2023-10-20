#include "geometry.h"
#include <cmath>
#include "global.hpp"

namespace Geometry
{
    float radians(float degrees)
    {
        return degrees * (M_PI / 180.0);
    }

    float degrees(float radians)
    {
        return radians * (180.0 / M_PI);
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
        return sqrt(Core::Vector::dot(v1 - v2, v1 - v2));
    }

    Core::Matrix4 translate(const Core::Matrix4 &matrix, float x, float y, float z) // column major
    {
        Core::Matrix4 rslt = matrix;
        rslt(3, 0) += x;
        rslt(3, 1) += y;
        rslt(3, 2) += z;
        return rslt;
    }

    Core::Matrix4 translate(const Core::Matrix4 &matrix, const Core::Vector3 &translation)
    {
        return translate(matrix, translation.x(), translation.y(), translation.z());
    }

    Core::Matrix4 rotate(const Core::Matrix4 &matrix, float angle_rad, float x, float y, float z)
    {
        Core::Matrix4 rslt = matrix;
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        float t = 1 - c;
        rslt(0, 0) = t * x * x + c;
        rslt(1, 0) = t * x * y - s * z;
        rslt(2, 0) = t * x * z + s * y;
        rslt(0, 1) = t * x * y + s * z;
        rslt(1, 1) = t * y * y + c;
        rslt(2, 1) = t * y * z - s * x;
        rslt(0, 2) = t * x * z - s * y;
        rslt(1, 2) = t * y * z + s * x;
        rslt(2, 2) = t * z * z + c;
        return rslt;
    }

    Core::Matrix4 rotate(const Core::Matrix4 &matrix, float angle_rad, Core::Vector3 axis)
    {
        axis = normalize(axis);
        return rotate(matrix, angle_rad, axis.x(), axis.y(), axis.z());
    }

    Core::Matrix4 scale(const Core::Matrix4 &matrix, float x, float y, float z) // column major
    {
        Core::Matrix4 rslt = matrix;
        rslt(0, 0) *= x;
        rslt(0, 1) *= x;
        rslt(0, 2) *= x;
        rslt(0, 3) *= x;
        rslt(1, 0) *= y;
        rslt(1, 1) *= y;
        rslt(1, 2) *= y;
        rslt(1, 3) *= y;
        rslt(2, 0) *= z;
        rslt(2, 1) *= z;
        rslt(2, 2) *= z;
        rslt(2, 3) *= z;
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
        Core::Vector3 s = normalize(Core::Vector3::cross(f, up));
        Core::Vector3 u = Core::Vector3::cross(s, f);

        Core::Matrix4 rslt;
        rslt(0, 0) = s.x();
        rslt(1, 0) = s.y();
        rslt(2, 0) = s.z();
        rslt(3, 0) = -Core::Vector::dot(s, eye);
        rslt(0, 1) = u.x();
        rslt(1, 1) = u.y();
        rslt(2, 1) = u.z();
        rslt(3, 1) = -Core::Vector::dot(u, eye);
        rslt(0, 2) = -f.x();
        rslt(1, 2) = -f.y();
        rslt(2, 2) = -f.z();
        rslt(3, 2) = Core::Vector::dot(f, eye);
        rslt(0, 3) = 0;
        rslt(1, 3) = 0;
        rslt(2, 3) = 0;
        rslt(3, 3) = 1;

        return rslt;
    }
    Core::Matrix4 perspective(float fov_rad, float aspect, float near, float far)
    {
        Core::Matrix4 rslt = Core::Matrix4::identity();
        float f = 1.0f / std::tan(fov_rad * 0.5f);
        rslt(0, 0) = f / aspect;
        rslt(1, 1) = f;
        rslt(2, 2) = (far + near) / (near - far);
        rslt(2, 3) = -1.0;
        rslt(3, 2) = (2.0f * far * near) / (near - far);
        rslt(3, 3) = 0;
        return rslt;
    }

    Core::Matrix4 orthographic(float left, float right, float bottom, float top, float near, float far)
    {
        Core::Matrix4 rslt = Core::Matrix4::identity();
        rslt(0, 0) = 2.f / (right - left);
        rslt(1, 1) = 2.f / (top - bottom);
        rslt(2, 2) = -2.f / (far - near);
        rslt(3, 0) = -(right + left) / (right - left);
        rslt(3, 1) = -(top + bottom) / (top - bottom);
        rslt(3, 2) = -(far + near) / (far - near);
        return rslt;
    }

    Core::Matrix4 orthographic(float left, float right, float bottom, float top)
    {
        return orthographic(left, right, bottom, top, -1.f, 1.f);
    }

    Core::Matrix4 frustum(float left, float right, float bottom, float top, float near, float far)
    {
        Core::Matrix4 rslt = Core::Matrix4::identity();
        rslt(0, 0) = 2.f * near / (right - left);
        rslt(1, 1) = 2.f * near / (top - bottom);
        rslt(2, 0) = (right + left) / (right - left);
        rslt(2, 1) = (top + bottom) / (top - bottom);
        rslt(2, 2) = -(far + near) / (far - near);
        rslt(2, 3) = -1.f;
        rslt(3, 2) = -(2.f * far * near) / (far - near);
        rslt(3, 3) = 0.f;
        return rslt;
    }

    Core::Quaternion quat_look_at(const Core::Vector3 &direction, const Core::Vector3 &up)
    {
        Core::Matrix3 m = Core::Matrix3::identity();
        Core::Vector3 front = -direction;
        m(2, 0) = front.x();
        m(2, 1) = front.y();
        m(2, 2) = front.z();
        Core::Vector3 right = Core::Vector3::cross(up, front);
        float inv_sqrt = 1.f / std::sqrt(std::fmax(FLOAT_EPSILON, Core::Vector3::dot(right, right)));
        right *= inv_sqrt;
        m(0, 0) = right.x();
        m(0, 1) = right.y();
        m(0, 2) = right.z();
        Core::Vector3 up_ = normalize(Core::Vector3::cross(front, right));
        m(1, 0) = up_.x();
        m(1, 1) = up_.y();
        m(1, 2) = up_.z();
        return Core::Quaternion::from_matrix(m);
    }

    void orthonomalize(Core::Vector3 &up, Core::Vector3 &front, Core::Vector3 &right)
    {
        up = normalize(up);
        front = normalize(front);
        right = normalize(right);
        right = normalize(Core::Vector3::cross(front, up));
        up = Core::Vector3::cross(right, front);
    }
}

#include "quaternion.h"
#include <cmath>
#include <tuple>
#include "geometry.h"
#include "global.hpp"

namespace Core
{
    Quaternion::Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

    Quaternion::Quaternion(const Quaternion &other)
        : w(other.w), x(other.x), y(other.y), z(other.z)
    {
    }

    Quaternion &Quaternion::operator=(const Quaternion &other)
    {
        w = other.w;
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Quaternion::Quaternion(Quaternion &&other)
        : w(other.w), x(other.x), y(other.y), z(other.z)
    {
    }

    Quaternion &Quaternion::operator=(Quaternion &&other)
    {
        w = other.w;
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Quaternion::~Quaternion() {}

    Quaternion Quaternion::operator+(const Quaternion &other) const
    {
        return Quaternion(w + other.w, x + other.x, y + other.y, z + other.z);
    }

    Quaternion Quaternion::operator-(const Quaternion &other) const
    {
        return Quaternion(w - other.w, x - other.x, y - other.y, z - other.z);
    }

    Quaternion Quaternion::operator*(const Quaternion &other) const
    {
        return Quaternion(
            w * other.w - x * other.x - y * other.y - z * other.z,
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w);
    }

    Quaternion Quaternion::operator*(float scalar) const
    {
        return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
    }

    Vector3 Quaternion::operator*(const Vector3 &vector) const
    {
        Quaternion q = *this * Quaternion(0, vector.x(), vector.y(), vector.z()) * conjugate();
        return Vector3(q.x, q.y, q.z);
    }

    Vector4 Quaternion::operator*(const Vector4 &vector) const
    {
        Quaternion q = *this * Quaternion(0, vector.x(), vector.y(), vector.z()) * conjugate();
        return Vector4(q.x, q.y, q.z, vector.w());
    }

    Matrix4 Quaternion::operator*(const Matrix4 &matrix) const
    {
        Matrix4 rslt;
        rslt(0, 0) = w * w + x * x - y * y - z * z;
        rslt(0, 1) = 2.f * x * y - 2.f * w * z;
        rslt(0, 2) = 2.f * x * z + 2.f * w * y;
        rslt(0, 3) = 0.f;
        rslt(1, 0) = 2.f * x * y + 2.f * w * z;
        rslt(1, 1) = w * w - x * x + y * y - z * z;
        rslt(1, 2) = 2.f * y * z - 2.f * w * x;
        rslt(1, 3) = 0.f;
        rslt(2, 0) = 2.f * x * z - 2.f * w * y;
        rslt(2, 1) = 2.f * y * z + 2.f * w * x;
        rslt(2, 2) = w * w - x * x - y * y + z * z;
        rslt(2, 3) = 0.f;
        rslt(3, 0) = 0.f;
        rslt(3, 1) = 0.f;
        rslt(3, 2) = 0.f;
        rslt(3, 3) = 1.f;
        return rslt * matrix;
    }

    bool Quaternion::operator==(const Quaternion &other) const
    {
        return EQUAL_F(w, other.w) && EQUAL_F(x, other.x) && EQUAL_F(y, other.y) && EQUAL_F(z, other.z);
    }

    bool Quaternion::operator!=(const Quaternion &other) const
    {
        return !(*this == other);
    }

    Quaternion Quaternion::conjugate() const
    {
        return Quaternion(w, -x, -y, -z);
    }

    Quaternion Quaternion::inverse() const
    {
        return conjugate() * (1.0f / dot(*this, *this));
    }

    Matrix4 Quaternion::to_matrix4() const
    {
        Matrix4 rslt = Matrix4::identity();
        float xx = x * x;
        float xy = x * y;
        float xz = x * z;
        float xw = x * w;
        float yy = y * y;
        float yz = y * z;
        float yw = y * w;
        float zz = z * z;
        float zw = z * w;

        rslt(0, 0) = 1.f - 2.f * (yy + zz);
        rslt(0, 1) = 2.f * (xy + zw);
        rslt(0, 2) = 2.f * (xz - yw);

        rslt(1, 0) = 2.f * (xy - zw);
        rslt(1, 1) = 1.f - 2.f * (xx + zz);
        rslt(1, 2) = 2.f * (yz + xw);

        rslt(2, 0) = 2.f * (xz + yw);
        rslt(2, 1) = 2.f * (yz - xw);
        rslt(2, 2) = 1.f - 2.f * (xx + yy);
        return rslt;
    }

    Matrix3 Quaternion::to_matrix3() const
    {
        Matrix3 rslt = Matrix3::identity();
        float xx = x * x;
        float xy = x * y;
        float xz = x * z;
        float xw = x * w;
        float yy = y * y;
        float yz = y * z;
        float yw = y * w;
        float zz = z * z;
        float zw = z * w;

        rslt(0, 0) = 1.f - 2.f * (yy + zz);
        rslt(0, 1) = 2.f * (xy + zw);
        rslt(0, 2) = 2.f * (xz - yw);

        rslt(1, 0) = 2.f * (xy - zw);
        rslt(1, 1) = 1.f - 2.f * (xx + zz);
        rslt(1, 2) = 2.f * (yz + xw);

        rslt(2, 0) = 2.f * (xz + yw);
        rslt(2, 1) = 2.f * (yz - xw);
        rslt(2, 2) = 1.f - 2.f * (xx + yy);
        return rslt;
    }

    std::tuple<Vector3, float> Quaternion::to_axis_angle() const
    {
        float angle = 2 * acos(w);
        float s = std::sqrt(1 - w * w);
        if (s < 0.0001)
        {
            return std::make_tuple(Vector3(1.f, 0.f, 0.f), 0);
        }
        else
        {
            return std::make_tuple(Vector3(x / s, y / s, z / s), angle);
        }
    }

    float Quaternion::yaw() const
    {
        return std::asin(CLAMP(2.f * (w * y - x * z), -1, 1));
    }

    float Quaternion::pitch() const
    {
        float y_ = 2.f * (w * x + y * z);
        float x_ = w * w - x * x - y * y + z * z;
        if (y_ == 0.f && x_ == 0.f)
        {
            return 2.f * std::atan2(x, w);
        }
        else
        {
            return std::atan2(y_, x_);
        }
    }

    float Quaternion::roll() const
    {
        return std::atan2(2.f * (w * z + x * y), w * w + x * x - y * y - z * z);
    }

    EulerAngle Quaternion::to_euler_angle() const
    {
        return EulerAngle({Geometry::degrees(pitch()), Geometry::degrees(yaw()), Geometry::degrees(roll())});
    }

    Quaternion Quaternion::from_euler_angle(float pitch, float yaw, float roll)
    {
        float pitch_rad_half = Geometry::radians(pitch) * 0.5;
        float yaw_rad_half = Geometry::radians(yaw) * 0.5;
        float roll_rad_half = Geometry::radians(roll) * 0.5;

        float cy = std::cos(pitch_rad_half);
        float sy = std::sin(pitch_rad_half);
        float cp = std::cos(yaw_rad_half);
        float sp = std::sin(yaw_rad_half);
        float cr = std::cos(roll_rad_half);
        float sr = std::sin(roll_rad_half);

        return Quaternion(
            cy * cp * cr + sy * sp * sr,
            sy * cp * cr - cy * sp * sr,
            sy * cp * sr + cy * sp * cr,
            cy * cp * sr - sy * sp * cr);
    }

    Quaternion Quaternion::from_euler_angle(const EulerAngle &euler_angle)
    {
        return from_euler_angle(euler_angle.pitch, euler_angle.yaw, euler_angle.roll);
    }

    Quaternion Quaternion::from_axis_angle(const Vector3 &axis, float angle_rad)
    {
        float half_angle = angle_rad * 0.5f;
        float s = std::sin(half_angle);
        return Quaternion(std::cos(half_angle), axis.x() * s, axis.y() * s, axis.z() * s);
    }

    Quaternion Quaternion::from_matrix(const Matrix3 &mat)
    {
        float x_sq = mat(0, 0) - mat(1, 1) - mat(2, 2);
        float y_sq = -mat(0, 0) + mat(1, 1) - mat(2, 2);
        float z_sq = -mat(0, 0) - mat(1, 1) + mat(2, 2);
        float w_sq = mat(0, 0) + mat(1, 1) + mat(2, 2);

        int max_index = 0;
        float max_value = w_sq;
        if (x_sq > max_value)
        {
            max_index = 1;
            max_value = x_sq;
        }
        if (y_sq > max_value)
        {
            max_index = 2;
            max_value = y_sq;
        }
        if (z_sq > max_value)
        {
            max_index = 3;
            max_value = z_sq;
        }

        float max_val = std::sqrt(max_value + 1) * 0.5f;
        float mult = 0.25f / max_val;

        switch (max_index)
        {
        case 0:
            return Quaternion(max_val, (mat(1, 2) - mat(2, 1)) * mult, (mat(2, 0) - mat(0, 2)) * mult, (mat(0, 1) - mat(1, 0)) * mult);
        case 1:
            return Quaternion((mat(1, 2) - mat(2, 1)) * mult, max_val, (mat(0, 1) + mat(1, 0)) * mult, (mat(2, 0) + mat(0, 2)) * mult);
        case 2:
            return Quaternion((mat(2, 0) - mat(0, 2)) * mult, (mat(0, 1) + mat(1, 0)) * mult, max_val, (mat(1, 2) + mat(2, 1)) * mult);
        case 3:
            return Quaternion((mat(0, 1) - mat(1, 0)) * mult, (mat(2, 0) + mat(0, 2)) * mult, (mat(1, 2) + mat(2, 1)) * mult, max_val);
        default:
            return Quaternion(1, 0, 0, 0);
        }
    }

    Quaternion Quaternion::from_matrix(const Matrix4 &mat)
    {
        return from_matrix(Matrix3(mat));
    }

    Quaternion Quaternion::from_basis_vector(const Vector3 &front, const Vector3 &up, const Vector3 &right)
    {
        Matrix3 mat;
        mat(0, 0) = right.x();
        mat(0, 1) = right.y();
        mat(0, 2) = right.z();
        mat(1, 0) = up.x();
        mat(1, 1) = up.y();
        mat(1, 2) = up.z();
        mat(2, 0) = front.x();
        mat(2, 1) = front.y();
        mat(2, 2) = front.z();
        return from_matrix(mat);
    }

    float Quaternion::dot(const Quaternion &a, const Quaternion &b)
    {
        return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Quaternion &Quaternion::normalize()
    {
        float length = std::sqrt(dot(*this, *this));
        w /= length;
        x /= length;
        y /= length;
        z /= length;
        return *this;
    }

} // namespace Core

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
        rslt(0, 1) = 2 * x * y - 2 * w * z;
        rslt(0, 2) = 2 * x * z + 2 * w * y;
        rslt(0, 3) = 0;
        rslt(1, 0) = 2 * x * y + 2 * w * z;
        rslt(1, 1) = w * w - x * x + y * y - z * z;
        rslt(1, 2) = 2 * y * z - 2 * w * x;
        rslt(1, 3) = 0;
        rslt(2, 0) = 2 * x * z - 2 * w * y;
        rslt(2, 1) = 2 * y * z + 2 * w * x;
        rslt(2, 2) = w * w - x * x - y * y + z * z;
        rslt(2, 3) = 0;
        rslt(3, 0) = 0;
        rslt(3, 1) = 0;
        rslt(3, 2) = 0;
        rslt(3, 3) = 1;
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

    Matrix4 Quaternion::get_matrix() const
    {
        float xx = x * x;
        float xy = x * y;
        float xz = x * z;
        float xw = x * w;
        float yy = y * y;
        float yz = y * z;
        float yw = y * w;
        float zz = z * z;
        float zw = z * w;

        return Matrix4(
            1 - 2 * (yy + zz), 2 * (xy - zw), 2 * (xz + yw), 0,
            2 * (xy + zw), 1 - 2 * (xx + zz), 2 * (yz - xw), 0,
            2 * (xz - yw), 2 * (yz + xw), 1 - 2 * (xx + yy), 0,
            0, 0, 0, 1);
    }

    std::tuple<Vector3, float> Quaternion::get_axis_angle() const
    {
        float angle = 2 * acos(w);
        float s = sqrt(1 - w * w);
        if (s < 0.001)
        {
            return std::make_tuple(Vector3(1, 0, 0), 0);
        }
        else
        {
            return std::make_tuple(Vector3(x / s, y / s, z / s), angle);
        }
    }

    Vector3 Quaternion::get_euler_angle() const
    {
        float sqw = w * w;
        float sqx = x * x;
        float sqy = y * y;
        float sqz = z * z;
        float unit = sqx + sqy + sqz + sqw;
        float test = x * y + z * w;
        float heading, attitude, bank;
        if (test > 0.499 * unit)
        {
            heading = 2 * atan2(x, w);
            attitude = M_PI / 2;
            bank = 0;
        }
        else if (test < -0.499 * unit)
        {
            heading = -2 * atan2(x, w);
            attitude = -M_PI / 2;
            bank = 0;
        }
        else
        {
            heading = atan2(2 * y * w - 2 * x * z, sqx - sqy - sqz + sqw);
            attitude = asin(2 * test / unit);
            bank = atan2(2 * x * w - 2 * y * z, -sqx + sqy - sqz + sqw);
        }
        return Vector3(heading, attitude, bank);
    }

    Quaternion Quaternion::from_euler_angle(float x, float y, float z)
    {
        float cx = cos(x / 2);
        float cy = cos(y / 2);
        float cz = cos(z / 2);
        float sx = sin(x / 2);
        float sy = sin(y / 2);
        float sz = sin(z / 2);
        return Quaternion(
            sx * cy * cz + cx * sy * sz,
            cx * sy * cz - sx * cy * sz,
            cx * cy * sz + sx * sy * cz,
            cx * cy * cz - sx * sy * sz);
    }

    Quaternion Quaternion::from_euler_angle(const Vector3 &euler_angle)
    {
        return from_euler_angle(euler_angle.x(), euler_angle.y(), euler_angle.z());
    }

    Quaternion Quaternion::from_axis_angle(const Vector3 &axis, float angle)
    {
        float half_angle = angle / 2;
        float s = sin(half_angle);
        return Quaternion(axis.x() * s, axis.y() * s, axis.z() * s, cos(half_angle));
    }

    Quaternion Quaternion::from_matrix(const Matrix4 &mat)
    {
        float trace = mat.trace();
        if (trace > 0)
        {
            float s = sqrt(trace + 1.0f) * 2;
            return Quaternion(
                0.25f * s,
                (mat(2, 1) - mat(1, 2)) / s,
                (mat(0, 2) - mat(2, 0)) / s,
                (mat(1, 0) - mat(0, 1)) / s);
        }
        else if (mat(0, 0) > mat(1, 1) && mat(0, 0) > mat(2, 2))
        {
            float s = sqrt(1.0f + mat(0, 0) - mat(1, 1) - mat(2, 2)) * 2;
            return Quaternion(
                (mat(2, 1) - mat(1, 2)) / s,
                0.25f * s,
                (mat(0, 1) + mat(1, 0)) / s,
                (mat(0, 2) + mat(2, 0)) / s);
        }
        else if (mat(1, 1) > mat(2, 2))
        {
            float s = sqrt(1.0f + mat(1, 1) - mat(0, 0) - mat(2, 2)) * 2;
            return Quaternion(
                (mat(0, 2) - mat(2, 0)) / s,
                (mat(0, 1) + mat(1, 0)) / s,
                0.25f * s,
                (mat(1, 2) + mat(2, 1)) / s);
        }
        else
        {
            float s = sqrt(1.0f + mat(2, 2) - mat(0, 0) - mat(1, 1)) * 2;
            return Quaternion(
                (mat(1, 0) - mat(0, 1)) / s,
                (mat(0, 2) + mat(2, 0)) / s,
                (mat(1, 2) + mat(2, 1)) / s,
                0.25f * s);
        }
    }

    Quaternion Quaternion::from_matrix(const Matrix3 &mat)
    {
        float trace = mat.trace();
        if (trace > 0)
        {
            float s = sqrt(trace + 1.0f) * 2;
            return Quaternion(
                0.25f * s,
                (mat(2, 1) - mat(1, 2)) / s,
                (mat(0, 2) - mat(2, 0)) / s,
                (mat(1, 0) - mat(0, 1)) / s);
        }
        else if (mat(0, 0) > mat(1, 1) && mat(0, 0) > mat(2, 2))
        {
            float s = sqrt(1.0f + mat(0, 0) - mat(1, 1) - mat(2, 2)) * 2;
            return Quaternion(
                (mat(2, 1) - mat(1, 2)) / s,
                0.25f * s,
                (mat(0, 1) + mat(1, 0)) / s,
                (mat(0, 2) + mat(2, 0)) / s);
        }
        else if (mat(1, 1) > mat(2, 2))
        {
            float s = sqrt(1.0f + mat(1, 1) - mat(0, 0) - mat(2, 2)) * 2;
            return Quaternion(
                (mat(0, 2) - mat(2, 0)) / s,
                (mat(0, 1) + mat(1, 0)) / s,
                0.25f * s,
                (mat(1, 2) + mat(2, 1)) / s);
        }
        else
        {
            float s = sqrt(1.0f + mat(2, 2) - mat(0, 0) - mat(1, 1)) * 2;
            return Quaternion(
                (mat(1, 0) - mat(0, 1)) / s,
                (mat(0, 2) + mat(2, 0)) / s,
                (mat(1, 2) + mat(2, 1)) / s,
                0.25f * s);
        }
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
        float length = sqrt(dot(*this, *this));
        w /= length;
        x /= length;
        y /= length;
        z /= length;
        return *this;
    }

} // namespace Core

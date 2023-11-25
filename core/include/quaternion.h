#pragma once
#ifndef QUATERNION_H
#define QUATERNION_H

#include "matrix.h"
#include "vector.h"
#include "geometry/general.h"

namespace Core
{
    class Quaternion
    {
        // attributes
    public:
        float w, x, y, z;

        // constructors and deconstructor
    public:
        Quaternion(float w = 1, float x = 0, float y = 0, float z = 0);
        Quaternion(const Quaternion &other);
        Quaternion &operator=(const Quaternion &other);
        Quaternion(Quaternion &&other);
        Quaternion &operator=(Quaternion &&other);
        ~Quaternion();
        // methods
    public:
        Quaternion operator+(const Quaternion &other) const;
        Quaternion operator-(const Quaternion &other) const;
        Quaternion operator*(const Quaternion &other) const;
        Quaternion operator*(float scalar) const;
        Quaternion &operator+=(const Quaternion &other) { return *this = *this + other; }
        Quaternion &operator-=(const Quaternion &other) { return *this = *this - other; }
        Quaternion &operator*=(const Quaternion &other) { return *this = *this * other; }
        Quaternion &operator*=(float scalar) { return *this = *this * scalar; }

        bool operator==(const Quaternion &other) const;

        bool operator!=(const Quaternion &other) const;

        Vector3 operator*(const Vector3 &vector) const;
        Vector4 operator*(const Vector4 &vector) const;
        Matrix4 operator*(const Matrix4 &matrix) const;

        // quaternion operations
        Quaternion &normalize();
        Quaternion conjugate() const;
        Quaternion inverse() const;

        // conversion
        Matrix4 to_matrix4() const;
        Matrix3 to_matrix3() const;
        std::tuple<Vector3, float> to_axis_angle() const;
        float yaw() const;
        float pitch() const;
        float roll() const;
        EulerAngle to_euler_angle() const;

        // static methods
    public:
        static float dot(const Quaternion &a, const Quaternion &b);
        static Quaternion identity() { return Quaternion(1, 0, 0, 0); }
        static Quaternion from_euler_angle(float pitch, float yaw, float roll);
        static Quaternion from_euler_angle(const EulerAngle &euler_angle);
        static Quaternion from_axis_angle(const Vector3 &axis, float angle_rad);
        static Quaternion from_matrix(const Matrix4 &mat);
        static Quaternion from_matrix(const Matrix3 &mat);
        static Quaternion from_basis_vector(const Vector3 &front, const Vector3 &up, const Vector3 &right);
    };

};

#endif // !QUATERNION_H
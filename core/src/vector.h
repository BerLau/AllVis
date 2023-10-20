#pragma once
#ifndef CORE_VECTOR_H
#define CORE_VECTOR_H

#include "matrix.h"

namespace Core
{

    class Vector : public Matrix
    {
        // members
    private:
    protected:
    public:
        // Rules of five
    public:
        Vector(size_t size = 0);
        Vector(const Vector &other) : Matrix(other) {}
        Vector &operator=(const Vector &other)
        {
            Matrix::operator=(other);
            return *this;
        }

        Vector(Vector &&other) : Matrix(std::move(other)) {}
        Vector &operator=(Vector &&other)
        {
            Matrix::operator=(std::move(other));
            return *this;
        }

        virtual ~Vector() {}

        // construct from matrix
        Vector(const Matrix &other) : Matrix(other) {}
        Vector &operator=(const Matrix &other)
        {
            Matrix::operator=(other);
            return *this;
        }

        Vector(Matrix &&other) : Matrix(other) {}
        Vector &operator=(Matrix &&other)
        {
            Matrix::operator=(other);
            return *this;
        }

        Vector(float *values, size_t size, bool duplicate) : Matrix(values, size, 1, duplicate) {}

        // methods
    private:
    protected:
    public:
        float &operator[](size_t index);
        float operator[](size_t index) const;
        float length2();
        float norm();
        float length() { return norm(); }
        float magnitude() { return norm(); }
        Vector normalize();
        // static methods
    public:
        static Vector ones(size_t size);
        static Vector zeros(size_t size);
        static float dot(const Vector &a, const Vector &b);
    };

    class Vector2 : public Vector
    {
        // members
    private:
    protected:
    public:
        // Rules of five
    public:
        Vector2() : Vector(2) {}
        Vector2(float x, float y) : Vector(2)
        {
            values[0] = x;
            values[1] = y;
        }
        Vector2(const Vector2 &other) : Vector(other) {}
        Vector2 &operator=(const Vector2 &other)
        {
            Vector::operator=(other);
            return *this;
        }

        Vector2(Vector2 &&other) : Vector(std::move(other)) {}
        Vector2 &operator=(Vector2 &&other)
        {
            Vector::operator=(std::move(other));
            return *this;
        }

        ~Vector2() {}
        explicit Vector2(float *values, bool duplicate) : Vector(values, 2, duplicate) {}
        // cast from matrix
        Vector2(const Matrix &other) : Vector(other) {}
        Vector2 &operator=(const Matrix &other)
        {
            Vector::operator=(other);
            return *this;
        }

        Vector2(Matrix &&other) : Vector(other) {}
        Vector2 &operator=(Matrix &&other)
        {
            Vector::operator=(other);
            return *this;
        }
        // methods
    private:
    protected:
    public:
        float &x() { return values[0]; }
        float &y() { return values[1]; }

        float x() const { return values[0]; }
        float y() const { return values[1]; }

        // static methods
    public:
        static Vector2 ones() { return Vector2(1.f, 1.f); }
        static Vector2 zeros() { return Vector2(0.f, 0.f); }
    };

    class Vector3 : public Vector
    {
        // members
    private:
    protected:
    public:
        // Rules of five
    public:
        Vector3() : Vector(3) {}
        Vector3(float x, float y, float z) : Vector(3)
        {
            values[0] = x;
            values[1] = y;
            values[2] = z;
        }
        Vector3(const Vector3 &other) : Vector(other) {}
        Vector3 &operator=(const Vector3 &other)
        {
            Vector::operator=(other);
            return *this;
        }

        Vector3(Vector3 &&other) : Vector(other) {}
        Vector3 &operator=(Vector3 &&other)
        {
            Vector::operator=(other);
            return *this;
        }

        Vector3(const Vector2 &other, float z) : Vector(3)
        {
            values[0] = other.x();
            values[1] = other.y();
            values[2] = z;
        }
        Vector3 &operator=(const Vector2 &other) { return *this = Vector3(other, 0.0f); }

        Vector3(const Vector &other) : Vector(3)
        {
            values[0] = other[0];
            values[1] = other[1];
            values[2] = other[2];
        }

        Vector3 &operator=(const Vector &other) { return *this = Vector3(other); }

        ~Vector3() {}

        Vector3(float *values, bool duplicate) : Vector(values, 3, duplicate) {}

        // cast from matrix
        Vector3(const Matrix &other) : Vector(other) {}
        Vector3 &operator=(const Matrix &other)
        {
            Vector::operator=(other);
            return *this;
        }

        Vector3(Matrix &&other) : Vector(other) {}
        Vector3 &operator=(Matrix &&other)
        {
            Vector::operator=(other);
            return *this;
        }

        // methods
    private:
    protected:
    public:
        float &x() { return values[0]; }
        float &y() { return values[1]; }
        float &z() { return values[2]; }

        float x() const { return values[0]; }
        float y() const { return values[1]; }
        float z() const { return values[2]; }

        Vector3 cross(const Vector3 &other)
        {
            return Vector3(y() * other.z() - z() * other.y(),
                           z() * other.x() - x() * other.z(),
                           x() * other.y() - y() * other.x());
        }

        // static methods
    public:
        static Vector3 ones() { return Vector3(1, 1, 1); }
        static Vector3 zeros() { return Vector3(0, 0, 0); }
        static Vector3 cross(const Vector3 &a, const Vector3 &b);
    };

    class Vector4 : public Vector
    {
        // members
    private:
    protected:
    public:
        // Rules of five
    public:
        Vector4() : Vector(4) {}
        Vector4(float x, float y, float z, float w) : Vector(4)
        {
            values[0] = x;
            values[1] = y;
            values[2] = z;
            values[3] = w;
        }
        Vector4(const Vector4 &other) : Vector(other) {}
        Vector4 &operator=(const Vector4 &other)
        {
            Vector::operator=(other);
            return *this;
        }

        Vector4(Vector4 &&other) : Vector(std::move(other)) {}
        Vector4 &operator=(Vector4 &&other)
        {
            Vector::operator=(std::move(other));
            return *this;
        }

        ~Vector4() {}

        Vector4(float *values, bool duplicate) : Vector(values, 4, duplicate) {}

        // cast from matrix
        Vector4(const Matrix &other) : Vector(other) {}

        Vector4(const Vector3 &other, float w) : Vector(4)
        {
            values[0] = other.x();
            values[1] = other.y();
            values[2] = other.z();
            values[3] = w;
        }
        Vector4 &operator=(const Vector3 &other) { return *this = Vector4(other, 1.0f); }

        Vector4(const Vector2 &other, float z, float w) : Vector(4)
        {
            values[0] = other.x();
            values[1] = other.y();
            values[2] = z;
            values[3] = w;
        }
        Vector4 &operator=(const Vector2 &other) { return *this = Vector4(other, 0.0f, 1.0f); }

        Vector4 &operator=(const Matrix &other)
        {
            Vector::operator=(other);
            return *this;
        }

        Vector4(Matrix &&other) : Vector(other) {}
        Vector4 &operator=(Matrix &&other)
        {
            Vector::operator=(other);
            return *this;
        }

        // methods
    private:
    protected:
    public:
        float &x() { return values[0]; }
        float &y() { return values[1]; }
        float &z() { return values[2]; }
        float &w() { return values[3]; }

        float x() const { return values[0]; }
        float y() const { return values[1]; }
        float z() const { return values[2]; }
        float w() const { return values[3]; }

        // static methods
    public:
        static Vector4 ones() { return Vector4(1, 1, 1, 1); }
        static Vector4 zeros() { return Vector4(0, 0, 0, 0); }
    };

}

#endif // CORE_VECTOR_H
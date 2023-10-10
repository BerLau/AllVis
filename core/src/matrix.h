#pragma once
#ifndef CORE_MATRIX_H
#define CORE_MATRIX_H
#include <iostream>

namespace Core
{
    class Vector;
    class Vector2;
    class Vector3;
    class Vector4;

    class Matrix
    {
        // members
    private:
        bool own_data;

    protected:
        float *values;
        size_t _rows;
        size_t _cols;

    public:
        // Rules of five
    public:
        Matrix(size_t rows = 0, size_t cols = 0);
        Matrix(const Matrix &other);
        Matrix &operator=(const Matrix &other);
        Matrix(Matrix &&other);
        Matrix &operator=(Matrix &&other);
        virtual ~Matrix();

        Matrix(float *values, size_t rows, size_t cols, bool duplicate);

        // construct from vector
        Matrix(const Vector &other);
        Matrix &operator=(const Vector &other);

        Matrix(Vector &&other);
        Matrix &operator=(Vector &&other);

        // methods
    private:
    protected:
    public:
        float &operator()(size_t row, size_t col);
        float operator()(size_t row, size_t col) const;
        float &operator()(size_t index);
        inline size_t index(size_t row, size_t col) const { return row * _cols + col; }

        Matrix operator+(const Matrix &other) const;
        Matrix operator-(const Matrix &other) const;
        Matrix operator*(float scalar) const;
        Matrix operator*(const Matrix &other) const;
        Vector operator*(const Vector &other) const;

        Matrix &operator+=(const Matrix &other) { return *this = *this + other; }
        Matrix &operator-=(const Matrix &other) { return *this = *this - other; }
        Matrix &operator*=(float scalar) { return *this = *this * scalar; }
        Matrix &operator*=(const Matrix &other) { return *this = *this * other; }

        virtual bool operator==(const Matrix &other) const;

        size_t rows() const { return _rows; }
        size_t cols() const { return _cols; }
        size_t size() const { return _rows * _cols; }
        size_t bytes() const { return size() * sizeof(float); }
        std::pair<size_t, size_t> shape() const { return std::make_pair(_rows, _cols); }

        float *data() const { return values; }
        virtual void free_data()
        {
            if (own_data)
            {
                delete[] values;
            }
            values = nullptr;
        }

        void fill(float value);

        void swap_rows(size_t row1, size_t row2);
        void scale_row(size_t row, float scalar);
        void add_row(size_t target, size_t source, float scalar = 1.0f);

        bool is_square() const { return _rows == _cols; }

        Matrix transpose() const;
        Matrix submatrix(size_t row, size_t col) const;
        // static methods
    public:
        static Matrix ones(size_t rows, size_t cols);
        static Matrix zeros(size_t rows, size_t cols);

        // friend functions
    public:
        // friend Matrix operator*(float scalar, const Matrix &other){ return other * scalar; }
        // format output
        friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);
        friend Matrix operator-(const Matrix &matrix);
    };

    class MatrixS : public Matrix
    {
        // members
    private:
    protected:
    public:
        // Rules of five
    public:
        MatrixS(size_t dimension = 0);
        MatrixS(const MatrixS &other);
        MatrixS &operator=(const MatrixS &other);
        MatrixS(MatrixS &&other);
        MatrixS &operator=(MatrixS &&other);
        virtual ~MatrixS() {}

        MatrixS(float *values, size_t dimension, bool duplicate) : Matrix(values, dimension, dimension, duplicate) {}

        // cast from matrix
        MatrixS(const Matrix &other);
        MatrixS(Matrix &&other);
        MatrixS &operator=(const Matrix &other);
        MatrixS &operator=(Matrix &&other);

        // methods
    public:
        size_t dim() const { return _rows; }

        // methods for square matrices
    public:
        float determinant() const;
        MatrixS inverse() const;
        float trace() const;
        // static methods
    public:
        static MatrixS identity(size_t dimension);
    };

    class Matrix4 : public MatrixS
    {
        // members
    private:
    protected:
    public:
        // Rules of five
    public:
        Matrix4();
        Matrix4(const Matrix4 &other);
        Matrix4 &operator=(const Matrix4 &other);
        Matrix4(Matrix4 &&other);
        Matrix4 &operator=(Matrix4 &&other);
        ~Matrix4() {}
        Matrix4(float *values, bool duplicate) : MatrixS(values, 4, duplicate) {}
        Matrix4(float m00, float m01, float m02, float m03,
                float m10, float m11, float m12, float m13,
                float m20, float m21, float m22, float m23,
                float m30, float m31, float m32, float m33);

        // methods
    private:
    protected:
    public:
        // cast from MatrixS
        Matrix4(const Matrix &other) : MatrixS(other) {}
        Matrix4(Matrix &&other) : MatrixS(other) {}
        Matrix4 &operator=(const Matrix &other)
        {
            MatrixS::operator=(other);
            return *this;
        }
        Matrix4 &operator=(Matrix &&other)
        {
            MatrixS::operator=(other);
            return *this;
        }
        // methods for 4x4 matrices
    public:
        void translate(const Vector3 &translation);
        void rotate(float angle_rad, const Vector3 &axis);
        void scale(const Vector3 &scale);

        Matrix4 translate(const Vector3 &translation) const;
        Matrix4 rotate(float angle_rad, const Vector3 &axis) const;
        Matrix4 scale(const Vector3 &scale) const;
        // static methods
    public:
        static Matrix4 identity() { return Matrix4(MatrixS::identity(4)); }
    };

    class Matrix3 : public MatrixS
    {
        // members
    private:
    protected:
    public:
        // Rules of five
    public:
        Matrix3() : MatrixS(3) {}
        Matrix3(const Matrix3 &other) : MatrixS(other) {}
        Matrix3 &operator=(const Matrix3 &other)
        {
            MatrixS::operator=(other);
            return *this;
        }
        Matrix3(Matrix3 &&other) : MatrixS(other) {}
        Matrix3 &operator=(Matrix3 &&other)
        {
            MatrixS::operator=(other);
            return *this;
        }
        ~Matrix3() {}
        Matrix3(float *values, bool duplicate) : MatrixS(values, 3, duplicate) {}
        Matrix3(float m00, float m01, float m02,
                float m10, float m11, float m12,
                float m20, float m21, float m22)
        {
            values = new float[9];
            values[0] = m00;
            values[1] = m01;
            values[2] = m02;
            values[3] = m10;
            values[4] = m11;
            values[5] = m12;
            values[6] = m20;
            values[7] = m21;
            values[8] = m22;
            _rows = 3;
            _cols = 3;
        }

        // methods
    private:
    protected:
    public:
        // cast from MatrixS
        Matrix3(const Matrix &other) : MatrixS(other) {}
        Matrix3(Matrix &&other) : MatrixS(other) {}
        Matrix3 &operator=(const Matrix &other)
        {
            MatrixS::operator=(other);
            return *this;
        }
        Matrix3 &operator=(Matrix &&other)
        {
            MatrixS::operator=(other);
            return *this;
        }
        // methods for 3x3 matrices
    public:
        // static methods
        static Matrix3 identity() { return Matrix3(MatrixS::identity(3)); }
    };
};     // namespace Core
#endif // !CORE_MATRIX_H
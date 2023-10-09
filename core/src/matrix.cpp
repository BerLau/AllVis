#include "matrix.h"
#include <stdexcept>
#include "vector.h"
#include <iostream>
#include "global.hpp"

namespace Core
{
    Matrix::Matrix(size_t rows, size_t cols)
    {
        this->own_data = true;
        this->_rows = rows;
        this->_cols = cols;
        if (rows > 0 && cols > 0)
        {
            values = new float[rows * cols];
            fill(0);
        }
        else
            values = nullptr;
    }

    Matrix::Matrix(const Matrix &other)
    {
        own_data = true;
        _rows = other._rows;
        _cols = other._cols;
        values = new float[size()];
        for (size_t i = 0; i < size(); ++i)
        {
            values[i] = other.values[i];
        }
    }

    Matrix &Matrix::operator=(const Matrix &other)
    {
        if (this != &other)
        {
            free_data();
            own_data = true;
            _rows = other._rows;
            _cols = other._cols;
            values = new float[size()];
            for (size_t i = 0; i < size(); ++i)
            {
                values[i] = other.values[i];
            }
        }
        return *this;
    }

    Matrix::Matrix(Matrix &&other)
    {
        own_data = other.own_data;
        _rows = other._rows;
        _cols = other._cols;
        values = other.values;
        other.values = nullptr;
    }

    Matrix &Matrix::operator=(Matrix &&other)
    {
        if (this != &other)
        {
            free_data();
            own_data = other.own_data;
            _rows = other._rows;
            _cols = other._cols;
            values = other.values;
            other.values = nullptr;
        }
        return *this;
    }

    Matrix::~Matrix()
    {
        free_data();
    }

    Matrix::Matrix(float *values, size_t rows, size_t cols, bool duplicate)
    {
        own_data = duplicate;
        _rows = rows;
        _cols = cols;
        if (duplicate)
        {
            this->values = new float[size()];
            for (size_t i = 0; i < size(); ++i)
            {
                this->values[i] = values[i];
            }
        }
        else
        {
            this->values = values;
        }
    }

    Matrix::Matrix(const Vector &other)
    {
        _rows = other._rows;
        _cols = other._cols;
        values = new float[size()];
        for (size_t i = 0; i < size(); ++i)
        {
            values[i] = other[i];
        }
    }

    Matrix &Matrix::operator=(const Vector &other)
    {
        delete[] values;
        _rows = other._rows;
        _cols = other._cols;
        values = new float[size()];
        for (size_t i = 0; i < size(); ++i)
        {
            values[i] = other[i];
        }
        return *this;
    }

    Matrix::Matrix(Vector &&other)
    {
        _rows = other._rows;
        _cols = other._cols;
        values = other.values;
        other.values = nullptr;
    }

    Matrix &Matrix::operator=(Vector &&other)
    {
        delete[] values;
        _rows = other._rows;
        _cols = other._cols;
        values = other.values;
        other.values = nullptr;
        return *this;
    }

    float &Matrix::operator()(size_t row, size_t col)
    {
        return values[index(row, col)];
    }

    float Matrix::operator()(size_t row, size_t col) const
    {
        return values[index(row, col)];
    }

    float &Matrix::operator()(size_t index)
    {
        return values[index * _cols];
    }
    
    Matrix Matrix::operator+(const Matrix &other) const
    {
        Matrix result(_rows, _cols);
        for (size_t i = 0; i < size(); ++i)
        {
            result.values[i] = values[i] + other.values[i];
        }
        return result;
    }

    Matrix Matrix::operator-(const Matrix &other) const
    {
        Matrix result(_rows, _cols);
        for (size_t i = 0; i < size(); ++i)
        {
            result.values[i] = values[i] - other.values[i];
        }
        return result;
    }

    Matrix Matrix::operator*(float scalar) const
    {
        Matrix result(_rows, _cols);
        for (size_t i = 0; i < size(); ++i)
        {
            result.values[i] = values[i] * scalar;
        }
        return result;
    }

    Matrix Matrix::operator*(const Matrix &other) const
    {
        Matrix result(_rows, other._cols);
        for (size_t i = 0; i < _rows; ++i)
        {
            for (size_t j = 0; j < other._cols; ++j)
            {
                for (size_t k = 0; k < _cols; ++k)
                {
                    result(i, j) += (*this)(i, k) * other(k, j);
                }
            }
        }
        return result;
    }

    Vector Matrix::operator*(const Vector &other) const
    {
        Vector result(_rows);
        for (size_t i = 0; i < _rows; ++i)
        {
            for (size_t j = 0; j < _cols; ++j)
            {
                result[i] += (*this)(i, j) * other[j];
            }
        }
        return result;
    }

    bool Matrix::operator==(const Matrix &other) const
    {
        if (_rows != other._rows || _cols != other._cols)
            return false;
        for (size_t i = 0; i < size(); ++i)
        {
            if (!EQUAL_F(values[i], other.values[i]))
                return false;
        }
        return true;
    }

    void Matrix::fill(float value)
    {
        for (size_t i = 0; i < size(); ++i)
        {
            values[i] = value;
        }
    }

    void Matrix::swap_rows(size_t row1, size_t row2)
    {
        for (size_t i = 0; i < _cols; ++i)
        {
            std::swap((*this)(row1, i), (*this)(row2, i));
        }
    }

    void Matrix::scale_row(size_t row, float scalar)
    {
        for (size_t i = 0; i < _cols; ++i)
        {
            (*this)(row, i) *= scalar;
        }
    }

    void Matrix::add_row(size_t target, size_t source, float scalar)
    {
        for (size_t i = 0; i < _cols; ++i)
        {
            (*this)(target, i) += (*this)(source, i) * scalar;
        }
    }

    Matrix Matrix::ones(size_t rows, size_t cols)
    {
        Matrix result(rows, cols);
        result.fill(1);
        return result;
    }

    Matrix Matrix::zeros(size_t rows, size_t cols)
    {
        Matrix result(rows, cols);
        result.fill(0);
        return result;
    }

    Matrix Matrix::transpose() const
    {
        Matrix result(_cols, _rows);
        for (size_t i = 0; i < _rows; ++i)
        {
            for (size_t j = 0; j < _cols; ++j)
            {
                result(j, i) = (*this)(i, j);
            }
        }
        return result;
    }

    Matrix Matrix::submatrix(size_t row, size_t col) const
    {
        Matrix result(_rows - 1, _cols - 1);
        size_t size = (*this).size();
        size_t index = 0;
        for (size_t i = 0; i < size; ++i)
        {
            if (i / _cols != row && i % _cols != col)
            {
                result.values[index++] = values[i];
            }
        }
        return result;
    }

    MatrixS::MatrixS(size_t dimension) : Matrix(dimension, dimension)
    {
    }

    MatrixS::MatrixS(const MatrixS &other) : Matrix(other)
    {
    }

    MatrixS &MatrixS::operator=(const MatrixS &other)
    {
        Matrix::operator=(other);
        return *this;
    }

    MatrixS::MatrixS(MatrixS &&other) : Matrix(other)
    {
    }

    MatrixS &MatrixS::operator=(MatrixS &&other)
    {
        Matrix::operator=(other);
        return *this;
    }

    MatrixS::MatrixS(const Matrix &other) : Matrix(other)
    {
    }

    MatrixS::MatrixS(Matrix &&other) : Matrix(other)
    {
    }

    MatrixS &MatrixS::operator=(const Matrix &other)
    {
        Matrix::operator=(other);
        return *this;
    }

    MatrixS &MatrixS::operator=(Matrix &&other)
    {
        Matrix::operator=(other);
        return *this;
    }

    MatrixS MatrixS::identity(size_t dimension)
    {
        MatrixS result(dimension);
        result.fill(0);
        for (size_t i = 0; i < dimension; ++i)
        {
            result(i, i) = 1;
        }
        return result;
    }

    float MatrixS::determinant() const
    {
        if (dim() == 1)
            return values[0];
        if (dim() == 2)
            return values[0] * values[3] - values[1] * values[2];
        float result = 0;
        for (size_t i = 0; i < _cols; ++i)
        {
            result += std::pow(-1, i) * values[i] * MatrixS(submatrix(0, i)).determinant();
        }
        return result;
    }

    MatrixS MatrixS::inverse() const
    {
        MatrixS result(dim());
        float det = determinant();
        if (det == 0)
            throw std::runtime_error("Matrix is not invertible");
        for (size_t i = 0; i < _rows; ++i)
        {
            for (size_t j = 0; j < _cols; ++j)
            {
                result(i, j) = std::pow(-1, i + j) * MatrixS(submatrix(i, j)).determinant() / det;
            }
        }
        return result;
    }

    float MatrixS::trace() const
    {
        float result = 0;
        for (size_t i = 0; i < _rows; ++i)
        {
            result += (*this)(i, i);
        }
        return result;
    }

    Matrix4::Matrix4() : MatrixS(4)
    {
    }

    Matrix4::Matrix4(const Matrix4 &other) : MatrixS(4)
    {
        for (size_t i = 0; i < size(); ++i)
        {
            values[i] = other.values[i];
        }
    }

    Matrix4 &Matrix4::operator=(const Matrix4 &other)
    {
        if (this != &other)
        {
            for (size_t i = 0; i < size(); ++i)
            {
                values[i] = other.values[i];
            }
        }
        return *this;
    }

    Matrix4::Matrix4(Matrix4 &&other) : MatrixS(4)
    {
        for (size_t i = 0; i < size(); ++i)
        {
            values[i] = other.values[i];
        }
    }

    Matrix4 &Matrix4::operator=(Matrix4 &&other)
    {
        if (this != &other)
        {
            for (size_t i = 0; i < size(); ++i)
            {
                values[i] = other.values[i];
            }
        }
        return *this;
    }

    Matrix4::Matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) : MatrixS(4)
    {
        (*this)(0, 0) = m00;
        (*this)(0, 1) = m01;
        (*this)(0, 2) = m02;
        (*this)(0, 3) = m03;
        (*this)(1, 0) = m10;
        (*this)(1, 1) = m11;
        (*this)(1, 2) = m12;
        (*this)(1, 3) = m13;
        (*this)(2, 0) = m20;
        (*this)(2, 1) = m21;
        (*this)(2, 2) = m22;
        (*this)(2, 3) = m23;
        (*this)(3, 0) = m30;
        (*this)(3, 1) = m31;
        (*this)(3, 2) = m32;
        (*this)(3, 3) = m33;
    }

    void Matrix4::translate(const Vector3 &translation)
    {
        (*this)(0, 3) += translation.x();
        (*this)(1, 3) += translation.y();
        (*this)(2, 3) += translation.z();
    }

    void Matrix4::scale(const Vector3 &scale)
    {
        (*this)(0, 0) *= scale.x();
        (*this)(1, 1) *= scale.y();
        (*this)(2, 2) *= scale.z();
    }

    void Matrix4::rotate(float angle, const Vector3 &axis)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);
        float t = 1 - c;
        float x = axis.x();
        float y = axis.y();
        float z = axis.z();
        (*this)(0, 0) = t * x * x + c;
        (*this)(0, 1) = t * x * y - s * z;
        (*this)(0, 2) = t * x * z + s * y;
        (*this)(1, 0) = t * x * y + s * z;
        (*this)(1, 1) = t * y * y + c;
        (*this)(1, 2) = t * y * z - s * x;
        (*this)(2, 0) = t * x * z - s * y;
        (*this)(2, 1) = t * y * z + s * x;
        (*this)(2, 2) = t * z * z + c;
    }

    Matrix4 Matrix4::translate(const Vector3 &translation) const
    {
        Matrix4 result(*this);
        result.translate(translation);
        return result;
    }

    Matrix4 Matrix4::scale(const Vector3 &scale) const
    {
        Matrix4 result(*this);
        result.scale(scale);
        return result;
    }

    Matrix4 Matrix4::rotate(float angle, const Vector3 &axis) const
    {
        Matrix4 result(*this);
        result.rotate(angle, axis);
        return result;
    }

    std::ostream &operator<<(std::ostream &os, const Matrix &matrix)
    {
        size_t row = 0;
        for (; row < matrix._rows - 1; ++row)
        {
            size_t col = 0;
            os << "[";
            for (col = 0; col < matrix._cols - 1; ++col)
            {
                os << matrix(row, col) << ", ";
            }
            os << matrix(row, col) << "]" << std::endl;
        }
        os << "[";
        for (size_t col = 0; col < matrix._cols - 1; ++col)
        {
            os << matrix(row, col) << ", ";
        }
        os << matrix(row, matrix._cols - 1) << "]" << std::endl;
        return os;
    }

    Matrix operator-(const Matrix &matrix)
    {
        Matrix result(matrix._rows, matrix._cols);
        for (size_t i = 0; i < matrix.size(); ++i)
        {
            result.values[i] = -matrix.values[i];
        }
        return result;
    }
}
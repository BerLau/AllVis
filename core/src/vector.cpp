#include "vector.h"
#include <stdexcept>

#include <cmath>

namespace Core
{
    float &Vector::operator[](size_t index)
    {
        return values[index];
    }

    float Vector::operator[](size_t index) const
    {
        return values[index];
    }

    Vector::Vector(size_t size) : Matrix(1, size)
    {
    }

    Vector Vector::ones(size_t size)
    {
        Vector rslt(size);
        for (size_t i = 0; i < size; ++i)
        {
            rslt[i] = 1;
        }
        return rslt;
    }

    Vector Vector::zeros(size_t size)
    {
        Vector rslt(size);
        for (size_t i = 0; i < size; ++i)
        {
            rslt[i] = 0;
        }
        return rslt;
    }

    float Vector::dot(const Vector &a, const Vector &b)
    {
        float rslt = 0;
        for (size_t i = 0; i < a.size(); ++i)
        {
            rslt += a[i] * b[i];
        }
        return rslt;
    }

    float Vector::length2()
    {
        size_t size_ = size();
        float rslt = 0;
        for (size_t i = 0; i < size_; ++i)
        {
            rslt += (*this)[i] * (*this)[i];
        }
        return rslt;
    }

    float Vector::norm()
    {
        return std::sqrt(length2());
    }

    Vector Vector::normalize()
    {
        float norm_ = norm();
        size_t size_ = size();
        for (size_t i = 0; i < size_; ++i)
        {
            (*this)[i] /= norm_;
        }
        return *this;
    }

    Vector3 Vector3::cross(const Vector3 &a, const Vector3 &b)
    {
        return Vector3(a.y() * b.z() - a.z() * b.y(),
                       a.z() * b.x() - a.x() * b.z(),
                       a.x() * b.y() - a.y() * b.x());
    }

} // namespace Core
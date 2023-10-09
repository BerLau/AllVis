#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "matrix.h"
#include "vector.h"
#include "quaternion.h"

namespace Geometry
{

    float radians(float degrees);
    float degrees(float radians);
    Core::Vector degrees(const Core::Vector &radians);
    Core::Vector radians(const Core::Vector &degrees);

    Core::Vector normalize(const Core::Vector &vector);
    Core::Quaternion normalize(const Core::Quaternion &quaternion);

    float dot(const Core::Quaternion &a, const Core::Quaternion &b);

    Core::Quaternion angle_axis(float angle_rad, float x, float y, float z);
    Core::Quaternion angle_axis(float angle_rad, const Core::Vector3 &axis);

    Core::Matrix4 translate(const Core::Matrix4 &matrix, float x, float y, float z);

    Core::Matrix4 translate(const Core::Matrix4 &matrix, const Core::Vector3 &translation);

    Core::Matrix4 rotate(const Core::Matrix4 &matrix, float angle_rad, float x, float y, float z);

    Core::Matrix4 rotate(const Core::Matrix4 &matrix, float angle_rad, Core::Vector3 axis);

    Core::Matrix4 scale(const Core::Matrix4 &matrix, float x, float y, float z);

    Core::Matrix4 scale(const Core::Matrix4 &matrix, float scale);

    Core::Matrix4 scale(const Core::Matrix4 &matrix, const Core::Vector3 &scale);

    Core::Matrix4 look_at(const Core::Vector3 &eye, const Core::Vector3 &center, const Core::Vector3 &up);

    Core::Matrix4 perspective(float fov_rad, float aspect, float near, float far);

    Core::Matrix4 orthographic(float left, float right, float bottom, float top, float near, float far);

    Core::Matrix4 orthographic(float left, float right, float bottom, float top);

    Core::Quaternion quat_look_at(const Core::Vector3 &direction, const Core::Vector3 &up, bool right_handed = true);
    Core::Quaternion quat_look_at(const Core::Vector3 &direction, const Core::Vector3 &up, const Core::Vector3 &right);

    // schmidt Orthonormalization
    void orthonomalize(Core::Vector3 &up, Core::Vector3 &front, Core::Vector3 &right);


}

#endif // GEOMETRY_H
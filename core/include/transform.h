#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <memory>
#include "vector.h"
#include "geometry/general.h"
#include "quaternion.h"

namespace Core
{
    class Transform;
    using Transform_S_Ptr = std::shared_ptr<Transform>;
    using Transform_U_Ptr = std::unique_ptr<Transform>;
    using Transform_W_Ptr = Transform_U_Ptr;
    using Transform_Ptr = Transform_U_Ptr;

    const Vector3 WORLD_UP = Vector3(0.0f, 1.0f, 0.0f);
    const Vector3 WORLD_RIGHT = Vector3(-1.0f, 0.0f, 0.0f);
    const Vector3 WORLD_FRONT = Vector3(0.0f, 0.0f, 1.0f);
    class Transform
    {
    public:
        Transform(const Vector3 &pos = Vector3(0.0f, 0.0f, 0.0f), const EulerAngle &euler_angle = {0, 0, 0}, const Vector3 &scale = Vector3(1.0, 1.0, 1.0));
        Transform(const Transform &transform);
        Transform &operator=(const Transform &transform);
        Transform(Transform &&transform);
        Transform &operator=(Transform &&transform);

        Transform(const Vector3 &pos, const Vector3 &front, const Vector3 &up, const Vector3 &scale = Vector3(1.0, 1.0, 1.0));
        ~Transform();

        void set_position(Vector3 position);
        void set_position(float x, float y, float z);

        void set_orientation(Quaternion rotation);
        void set_orientation(const EulerAngle &euler_angle);
        void set_orientation(const Vector3 &front, const Vector3 &up);
        void look_at(const Vector3 &front, const Vector3 &up);
        void look_at(const Vector3 &target);

        void set_front(Vector3 front);
        void set_front(float x, float y, float z);

        void set_up(Vector3 up);
        void set_up(float x, float y, float z);

        void set_right(Vector3 right);
        void set_right(float x, float y, float z);

        void set_scale(Vector3 scale);
        void set_scale(float x, float y, float z);
        void set_scale(float scale);

        void move_forward(float distance);
        void move_right(float distance);
        void move_up(float distance);

        void move_backward(float distance){ move_forward(-distance); }
        void move_left(float distance){ move_right(-distance);}
        void move_down(float distance){ move_up(-distance);}

        void move(Vector3 direction, float distance);

        void move_around_vertically(Vector3 center, float angle_degree);
        void move_around_horizontally(Vector3 center, float angle_degree);

        Vector3 get_position();
        Quaternion get_orientation();
        EulerAngle get_orientation_euler_angle();
        Matrix4 get_orientation_matrix();
        Vector3 get_scale();

        void translate(Vector3 translation);
        void translate(float x, float y, float z);

        void rotate_x(float angle_degree, bool local = true);
        void rotate_y(float angle_degree, bool local = true);
        void rotate_z(float angle_degree, bool local = true);

        // void rotate(float angle_degree, Vector3 axis, bool local = true);

        void angle_axis_rotate(float angle_rad, Vector3 axis, bool local = true);

        void scale(float x, float y, float z);
        void scale(Vector3 scale);
        void scale(float scale);

        Vector3 get_front();
        Vector3 get_right();
        Vector3 get_up();

        Matrix3 get_normal_matrix();
        Matrix4 get_model_matrix();

    private:
        Vector3 m_position;
        Vector3 m_scale;
        Quaternion m_orientation;
    };
}; // namespace Core

#endif
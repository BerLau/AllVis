#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <memory>
#include <core.h>

namespace Core
{
    class Transform;
    using Transform_S_Ptr = std::shared_ptr<Transform>;
    using Transform_U_Ptr = std::unique_ptr<Transform>;
    using Transform_W_Ptr = Transform_U_Ptr;
    using Transform_Ptr = Transform_U_Ptr;

    const Core::Vector3 WORLD_UP = Core::Vector3(0.0f, 1.0f, 0.0f);
    const Core::Vector3 WORLD_RIGHT = Core::Vector3(-1.0f, 0.0f, 0.0f);
    const Core::Vector3 WORLD_FRONT = Core::Vector3(0.0f, 0.0f, 1.0f);
    class Transform
    {
    public:
        Transform(const Core::Vector3 &pos = Core::Vector3(0.0f, 0.0f, 0.0f), const Core::EulerAngle &euler_angle = {0, 0, 0}, const Core::Vector3 &scale = Core::Vector3(1.0, 1.0, 1.0));
        Transform(const Core::Vector3 &pos, const Core::Vector3 &front, const Core::Vector3 &up, const Core::Vector3 &scale = Core::Vector3(1.0, 1.0, 1.0));
        Transform(const Transform &transform);
        Transform &operator=(const Transform &transform);
        Transform(Transform &&transform);
        Transform &operator=(Transform &&transform);
        ~Transform();

        void set_position(Core::Vector3 position);
        void set_position(float x, float y, float z);

        void set_orientation(Core::Quaternion rotation);
        void set_orientation(const Core::EulerAngle& euler_angle);
        void set_orientation(const Core::Vector3 &front, const Core::Vector3 &up, const Core::Vector3 &right);
        void look_at(const Core::Vector3 &front, const Core::Vector3 &up);

        void set_scale(Core::Vector3 scale);
        void set_scale(float x, float y, float z);
        void set_scale(float scale);

        void move_forward(float distance);
        void move_right(float distance);
        void move_up(float distance);
        void move(Core::Vector3 direction, float distance);

        void move_around_vertically(Core::Vector3 center, float angle_degree);
        void move_around_horizontally(Core::Vector3 center, float angle_degree);

        Core::Vector3 get_position();
        Core::Quaternion get_orientation();
        Core::EulerAngle get_orientation_euler_angle();
        Core::Matrix4 get_rotation_matrix();
        Core::Vector3 get_scale();

        void translate(Core::Vector3 translation);
        void translate(float x, float y, float z);

        void rotate_x(float angle_degree, bool local = true);
        void rotate_y(float angle_degree, bool local = true);
        void rotate_z(float angle_degree, bool local = true);

        void angle_axis_rotate(float angle_rad, Core::Vector3 axis);

        void scale(float x, float y, float z);
        void scale(Core::Vector3 scale);
        void scale(float scale);

        Core::Vector3 get_front();
        Core::Vector3 get_right();
        Core::Vector3 get_up();

    private:
        Core::Vector3 m_position;
        Core::Vector3 m_scale;
        Core::Quaternion m_orientation;
    };
}; // namespace Core

#endif
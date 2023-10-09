#include "transform.h"
#include "lmath.h"

namespace Core
{
    Transform::Transform(const Core::Vector3 &pos, float x_degree, float y_degree, float z_degree, const Core::Vector3 &scale)
        : m_position(pos),
          m_scale(scale),
          m_orientation(Quaternion::from_euler_angle(Vector3(Geometry::radians(x_degree),
                                                             Geometry::radians(y_degree),
                                                             Geometry::radians(z_degree))))
    {
    }

    Transform::Transform(const Core::Vector3 &pos, const Core::Vector3 &front, const Core::Vector3 &up, const Core::Vector3 &scale)
        : m_position(pos),
          m_scale(scale)
    {
        Vector3 f = Geometry::normalize(front);
        Vector3 r = Geometry::normalize(Core::Vector3::cross(f, up));
        Vector3 u = Geometry::normalize(Core::Vector3::cross(r, f));
        m_orientation = Geometry::quat_look_at(f, u, r);
    }

    Transform::Transform(const Transform &transform) : m_position(transform.m_position), m_scale(transform.m_scale), m_orientation(transform.m_orientation)
    {
    }

    Transform &Transform::operator=(const Transform &transform)
    {
        m_position = transform.m_position;
        m_scale = transform.m_scale;
        m_orientation = transform.m_orientation;
        return *this;
    }

    Transform::Transform(Transform &&transform) : m_position(transform.m_position), m_scale(transform.m_scale), m_orientation(transform.m_orientation)
    {
    }

    Transform &Transform::operator=(Transform &&transform)
    {
        m_position = transform.m_position;
        m_scale = transform.m_scale;
        m_orientation = transform.m_orientation;
        return *this;
    }

    Transform::~Transform()
    {
    }

    void Transform::set_position(Vector3 position)
    {
        this->m_position = position;
    }

    void Transform::set_position(float x, float y, float z)
    {
        this->m_position = Vector3(x, y, z);
    }

    void Transform::set_orientation(Quaternion rotation)
    {
        this->m_orientation = rotation;
    }

    void Transform::set_orientation(float x_degree, float y_degree, float z_degree)
    {
        this->m_orientation = Quaternion::from_euler_angle(Vector3(Geometry::radians(x_degree), Geometry::radians(y_degree), Geometry::radians(z_degree)));
    }

    void Transform::set_orientation(const Core::Vector3 &front, const Core::Vector3 &up, const Core::Vector3 &right)
    {
        this->m_orientation = Core::Quaternion::from_basis_vector(front, up, right);
    }

    void Transform::look_at(const Core::Vector3 &front, const Core::Vector3 &up)
    {
        this->m_orientation = Geometry::quat_look_at(-front, up, false);
    }

    void Transform::set_scale(Vector3 scale)
    {
        this->m_scale = scale;
    }

    void Transform::set_scale(float x, float y, float z)
    {
        this->m_scale = Vector3(x, y, z);
    }

    void Transform::set_scale(float scale)
    {
        this->m_scale = Vector3(scale, scale, scale);
    }

    void Transform::move_forward(float distance)
    {
        m_position += get_front() * distance;
    }

    void Transform::move_right(float distance)
    {
        m_position += get_right() * distance;
    }

    void Transform::move_up(float distance)
    {
        m_position += get_up() * distance;
    }

    void Transform::move(Vector3 direction, float distance)
    {
        m_position += direction * distance;
    }

    void Transform::move_around_vertically(Vector3 center, float angle_degree)
    {
        float angle_rad = Geometry::radians(-angle_degree);
        Quaternion rot = Geometry::angle_axis(angle_rad, get_up());
        m_position = Vector3(rot * Vector4(m_position - center, 1.0f)) + center;
        m_orientation = (rot * m_orientation).normalize();
    }

    void Transform::move_around_horizontally(Vector3 center, float angle_degree)
    {
        float angle_rad = Geometry::radians(-angle_degree);
        Quaternion rot = Geometry::angle_axis(angle_rad, get_right());
        m_position = Vector3(rot * Vector4(m_position - center, 1.0f)) + center;
        m_orientation = (rot * m_orientation).normalize();
    }

    Vector3 Transform::get_position()
    {
        return m_position;
    }

    Quaternion Transform::get_orientation()
    {
        return m_orientation;
    }

    Vector3 Transform::get_orientation_xyz()
    {
        Vector3 euler = m_orientation.get_euler_angle();
        return Vector3(Geometry::degrees(euler.x()), Geometry::degrees(euler.y()), Geometry::degrees(euler.z()));
    }

    Matrix4 Transform::get_rotation_matrix()
    {
        return m_orientation.get_matrix();
    }

    Vector3 Transform::get_scale()
    {
        return m_scale;
    }

    void Transform::translate(Vector3 translation)
    {
        m_position += translation;
    }

    void Transform::translate(float x, float y, float z)
    {
        m_position += Vector3(x, y, z);
    }

    void Transform::rotate_x(float angle_degree, bool local)
    {
        float offset = Geometry::radians(angle_degree);
        Vector3 axis = local ? get_right() : WORLD_RIGHT;
        Quaternion rot = Geometry::angle_axis(offset, axis);
        m_orientation = (rot * m_orientation).normalize();
    }

    void Transform::rotate_y(float angle_degree, bool local)
    {
        float offset = Geometry::radians(angle_degree);
        Vector3 axis = local ? get_up() : WORLD_UP;
        Quaternion rot = Geometry::angle_axis(offset, axis);
        m_orientation = (rot * m_orientation).normalize();
    }

    void Transform::rotate_z(float angle_degree, bool local)
    {
        float offset = Geometry::radians(angle_degree);
        Vector3 axis = local ? get_front() : WORLD_UP;
        Quaternion rot = Geometry::angle_axis(offset, axis);
        m_orientation = (rot * m_orientation).normalize();
    }

    void Transform::angle_axis_rotate(float angle_rad, Vector3 axis)
    {
        Quaternion offset = Geometry::angle_axis(angle_rad, axis.normalize());
        m_orientation = (offset * m_orientation).normalize();
    }

    void Transform::scale(float x, float y, float z)
    {
        m_scale *= Vector3(x, y, z);
    }

    void Transform::scale(Vector3 scale)
    {
        this->m_scale *= scale;
    }

    void Transform::scale(float scale)
    {
        this->m_scale *= Vector3(scale, scale, scale);
    }

    Vector3 Transform::get_front()
    {
        // return glm::normalize(glm::rotate(m_orientation, WORLD_UP));
        return m_orientation * WORLD_FRONT;
    }

    Vector3 Transform::get_right()
    {
        // return glm::normalize(glm::rotate(m_orientation, WORLD_RIGHT));
        return m_orientation * WORLD_RIGHT;
    }

    Vector3 Transform::get_up()
    {
        // return glm::normalize(glm::rotate(m_orientation, WORLD_UP));
        return m_orientation * WORLD_UP;
    }
}; // namespace Core
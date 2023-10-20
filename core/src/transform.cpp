#include "transform.h"

namespace Core
{
    Transform::Transform(const Core::Vector3 &pos, const EulerAngle &euler_angle, const Core::Vector3 &scale)
        : m_position(pos),
          m_scale(scale),
          m_orientation(Quaternion::from_euler_angle(euler_angle.yaw, euler_angle.pitch, euler_angle.roll))
    {
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

    Transform::Transform(const Core::Vector3 &pos, const Core::Vector3 &front, const Core::Vector3 &up, const Core::Vector3 &scale)
        : m_position(pos),
          m_scale(scale)
    {
        set_orientation(front, up);
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

    void Transform::set_orientation(const Core::EulerAngle &euler_angle)
    {
        this->m_orientation = Quaternion::from_euler_angle(euler_angle);
    }

    void Transform::set_orientation(const Core::Vector3 &front, const Core::Vector3 &up)
    {
        // normalize and orthogonalize
        Vector3 front_ = Geometry::normalize(front);
        Vector3 up_ = Geometry::normalize(up);
        Vector3 right = Vector3::cross(front_, up_);
        up_ = Vector3::cross(right, front_);
        this->m_orientation = Quaternion::from_basis_vector(front_, up_, right);
    }

    void Transform::look_at(const Core::Vector3 &front, const Core::Vector3 &up)
    {
        this->m_orientation = Geometry::quat_look_at(front, up);
    }

    void Transform::look_at(const Core::Vector3 &target)
    {
        this->m_orientation = Geometry::quat_look_at(target - m_position, get_up());
    }

    void Transform::set_front(Core::Vector3 front)
    {
        Vector3 front_ = Geometry::normalize(front);
        Vector3 up_ = Geometry::normalize(get_up());
        Vector3 right = Vector3::cross(front_, up_);
        up_ = Vector3::cross(right, front_);
        this->m_orientation = Quaternion::from_basis_vector(front_, up_, right);
    }

    void Transform::set_front(float x, float y, float z)
    {
        set_front(Vector3(x, y, z));
    }

    void Transform::set_up(Core::Vector3 up)
    {
        Vector3 up_ = Geometry::normalize(up);
        Vector3 front_ = Geometry::normalize(get_front());
        Vector3 right = Vector3::cross(front_, up_);
        front_ = Vector3::cross(up_, right);
        this->m_orientation = Quaternion::from_basis_vector(front_, up_, right);
    }

    void Transform::set_up(float x, float y, float z)
    {
        set_up(Vector3(x, y, z));
    }

    void Transform::set_right(Core::Vector3 right)
    {
        Vector3 right_ = Geometry::normalize(right);
        Vector3 front_ = Geometry::normalize(get_front());
        Vector3 up_ = Vector3::cross(right_, front_);
        front_ = Vector3::cross(up_, right_);
        this->m_orientation = Quaternion::from_basis_vector(front_, up_, right_);
    }

    void Transform::set_right(float x, float y, float z)
    {
        set_right(Vector3(x, y, z));
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

    EulerAngle Transform::get_orientation_euler_angle()
    {
        return m_orientation.to_euler_angle();
    }

    Matrix4 Transform::get_orientation_matrix()
    {
        return m_orientation.to_matrix4();
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
        float angle_rad = Geometry::radians(angle_degree);
        Quaternion offset = Geometry::angle_axis(angle_rad, get_right());
        if (local)
        {
            m_orientation = (offset * m_orientation).normalize();
        }
        else
        {
            m_orientation = (m_orientation * offset).normalize();
        }
    }

    void Transform::rotate_y(float angle_degree, bool local)
    {
        float angle_rad = Geometry::radians(angle_degree);
        Quaternion offset = Geometry::angle_axis(angle_rad, get_up());
        if (local)
        {
            m_orientation = (offset * m_orientation).normalize();
        }
        else
        {
            m_orientation = (m_orientation * offset).normalize();
        }
    }

    void Transform::rotate_z(float angle_degree, bool local)
    {
        float angle_rad = Geometry::radians(angle_degree);
        Quaternion offset = Geometry::angle_axis(angle_rad, get_front());
        if (local)
        {
            m_orientation = (offset * m_orientation).normalize();
        }
        else
        {
            m_orientation = (m_orientation * offset).normalize();
        }
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
        return Geometry::normalize(m_orientation * WORLD_FRONT);
    }

    Vector3 Transform::get_right()
    {
        return Geometry::normalize(m_orientation * WORLD_RIGHT);
    }

    Vector3 Transform::get_up()
    {
        return Geometry::normalize(m_orientation * WORLD_UP);
    }

    Core::Matrix3 Transform::get_normal_matrix()
    {
        return m_orientation.to_matrix4().inverse().transpose();
    }

    Core::Matrix4 Transform::get_model_matrix()
    {
        Core::Matrix4 model = Core::Matrix4::identity();
        model = model * m_orientation.to_matrix4();
        model = Geometry::scale(model, m_scale);
        model = Geometry::translate(model, m_position);
        return model;
    }
}; // namespace Core
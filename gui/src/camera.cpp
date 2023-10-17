#include "camera.h"
#include "config.h"

namespace Rendering
{
    Camera::Camera()
        : transform(Core::Transform_Ptr(new Core::Transform())),
          properties(Properties())
    {
        init();
    }

    Camera::Camera(const Core::Vector3 &position)
        : transform(Core::Transform_Ptr(new Core::Transform(position))),
          properties(Properties())
    {
    }

    Camera::~Camera()
    {
    }

    void Camera::init()
    {
    }

    void Camera::move(CameraMovement direction, float distance)
    {
        switch (direction)
        {
        case FORWARD:
            move_forward(distance);
            break;
        case BACKWARD:
            move_backward(distance);
            break;
        case LEFT:
            move_left(distance);
            break;
        case RIGHT:
            move_right(distance);
            break;
        case ELEVATE:
            move_up(distance);
            break;
        case DEPRESS:
            move_down(distance);
            break;
        default:
            break;
        }
    }

    void Camera::move(Core::Vector3 direction, float distance)
    {
        transform->translate(direction * distance);
    }

    Core::Matrix4 Camera::get_view_matrix()
    {
        Core::Vector3 pos = transform->get_position();
        Core::Vector3 front = transform->get_front();
        Core::Vector3 up = transform->get_up();
        Core::Vector3 center = pos + front * properties.focus_distance;
        return Geometry::look_at(pos, center, up);
    }

    Core::Vector3 Camera::get_position() const
    {
        return transform->get_position();
    }

    void Camera::focus_on(Core::Vector3 target, Core::Vector3 up)
    {
        Core::Vector3 front = Geometry::normalize(transform->get_position() - target);
        up = Geometry::normalize(up);
        transform->look_at(front, up);
    }

    void Camera::move_forward(float distance)
    {
        transform->translate(transform->get_front() * distance);
    }

    void Camera::move_backward(float distance)
    {
        transform->translate(-transform->get_front() * distance);
    }

    void Camera::move_left(float distance)
    {
        transform->translate(-transform->get_right() * distance);
    }

    void Camera::move_right(float distance)
    {
        transform->translate(transform->get_right() * distance);
    }

    void Camera::move_up(float distance)
    {
        transform->translate(transform->get_up() * distance);
    }

    void Camera::move_down(float distance)
    {
        transform->translate(-transform->get_up() * distance);
    }
} // namespace Scene
// Path: camera.h

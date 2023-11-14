#include "camera.h"
#include "config.h"

namespace Rendering
{
    Camera::Camera()
        : Configurable("Camera"),
          transform(Core::Transform_Ptr(new Core::Transform())),
          properties(Properties())
    {
        init();
    }

    Camera::Camera(const Core::Vector3 &position)
        : Configurable("Camera"),
          transform(Core::Transform_Ptr(new Core::Transform(position))),
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
            transform->move_forward(distance);
            break;
        case BACKWARD:
            transform->move_backward(distance);
            break;
        case LEFT:
            transform->move_left(distance);
            break;
        case RIGHT:
            transform->move_right(distance);
            break;
        case ELEVATE:
            transform->move_up(distance);
            break;
        case DEPRESS:
            transform->move_down(distance);
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
        Core::Vector3 front = Geometry::normalize(target - transform->get_position());
        up = Geometry::normalize(up);
        transform->look_at(-front, up);
        properties.focus_distance = Geometry::distance(target, transform->get_position());
    }

} // namespace Scene
// Path: camera.h

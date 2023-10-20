#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "core.h"

namespace Rendering
{
    const float default_fov = 45.0f;
    const float default_focus_distance = 1.0f;
    const float default_near = 0.1f;
    const float default_far = 100.0f;

    class Camera;

    using Camera_S_Ptr = std::shared_ptr<Camera>;
    using Camera_U_Ptr = std::unique_ptr<Camera>;
    using Camera_W_Ptr = std::weak_ptr<Camera>;
    using Camera_Ptr = Camera_U_Ptr;

    enum CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        ELEVATE,
        DEPRESS
    };

    class Camera : public Core::Configurable
    {
    public:
        struct Properties
        {
            float fov = default_fov;
            float near = default_near;
            float far = default_far;
            float focus_distance = default_focus_distance;
        };

        // attributes
    public:
        Core::Transform_Ptr transform;
        Properties properties;

        // constructors and destructor
    public:
        Camera();
        Camera(const Core::Vector3 &position);
        Camera(const Camera &camera) : transform(Core::Transform_Ptr(new Core::Transform(*camera.transform))), properties(Properties()) { init(); }
        Camera &operator=(const Camera &camera)
        {
            transform = Core::Transform_Ptr(new Core::Transform(*camera.transform));
            properties = camera.properties;
            return *this;
        }
        Camera(Camera &&camera) : transform(std::move(camera.transform)), properties(camera.properties) { init(); }
        Camera &operator=(Camera &&camera)
        {
            transform = std::move(camera.transform);
            properties = camera.properties;
            return *this;
        }
        ~Camera();

        // methods
    public:
        void move(CameraMovement direction, float distance);
        void move(Core::Vector3 direction, float distance);
        Core::Matrix4 get_view_matrix();
        Core::Vector3 get_position() const;
        Core::Vector3 get_up() const { return transform->get_up(); }
        Core::Vector3 get_front() const { return transform->get_front(); }
        Core::Vector3 get_right() const { return transform->get_right(); }
        Core::Vector3 get_focus() const { return transform->get_position() + transform->get_front() * properties.focus_distance; }
        void focus_on(Core::Vector3 position, Core::Vector3 up = Core::Vector3(0.0f, 1.0f, 0.0f));

        void init();
        void move_forward(float distance);
        void move_backward(float distance);
        void move_left(float distance);
        void move_right(float distance);
        void move_up(float distance);
        void move_down(float distance);
    };
} // namespace Scene
#endif // CAMERA_H
#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "core.h"

namespace Rendering
{
    const float default_pitch = 0.0f;
    const float default_yaw = -90.0f;
    const float default_roll = 0.0f;
    const float default_fov = 60.0f;
    const float default_mouse_sensitivity_unit = 0.001f;
    const float default_movement_speed_unit = 0.001f;
    const float default_focus_distance = 2.0f;

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

    class Camera
    {
        // attributes
    public:
        float fov;
        Core::Transform_Ptr transform;
        float focus_distance;

        // constructors and destructor
    public:
        Camera();
        Camera(const Core::Vector3 &position);
        Camera(const Camera &camera) : fov(camera.fov), transform(Core::Transform_Ptr(new Core::Transform(*camera.transform))), focus_distance(camera.focus_distance) { init(); }
        Camera &operator=(const Camera &camera)
        {
            fov = camera.fov;
            transform = Core::Transform_Ptr(new Core::Transform(*camera.transform));
            focus_distance = camera.focus_distance;
            return *this;
        }
        Camera(Camera &&camera) : fov(camera.fov), transform(std::move(camera.transform)), focus_distance(camera.focus_distance) { init(); }
        Camera &operator=(Camera &&camera)
        {
            fov = camera.fov;
            transform = std::move(camera.transform);
            focus_distance = camera.focus_distance;
            return *this;
        }
        ~Camera();

        // methods
    public:
        void move(CameraMovement direction, float distance);
        void move(Core::Vector3 direction, float distance);
        Core::Matrix4 get_view_matrix();
        void focus_on(Core::Vector3 position, Core::Vector3 up = Core::Vector3(0.0f, 1.0f, 0.0f));

    private:
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
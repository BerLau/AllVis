#pragma once
#ifndef RENDERING_SCENE_H
#define RENDERING_SCENE_H

#include <glad/glad.h>
#include <memory>
#include <vector>

#include "models.h"
#include "camera.h"

namespace Rendering
{
    class Scene;
    using Scene_U_Ptr = std::unique_ptr<Scene>;
    using Scene_S_Ptr = std::shared_ptr<Scene>;
    using Scene_W_Ptr = std::weak_ptr<Scene>;
    using Scene_Ptr = Scene_U_Ptr;

    class Scene
    {
        // attributes
    public:
        float width;
        float height;
        float aspect;
        // constructors and deconstructor
    public:
        Scene(float width, float height) : width(width), height(height), aspect(width / height) {}
        virtual ~Scene() {}
        // methods
    public:
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void destroy() = 0;
        virtual void render() = 0;

        void resize(float width, float height)
        {
            this->width = width;
            this->height = height;
            this->aspect = width / height;
        }
    };

    class OGL_Scene : public Scene
    {
        // attributes
    public:
        float near;
        float far;
        std::vector<OGL_Model_Ptr> models;
        // constructors and deconstructor
    public:
        OGL_Scene(float width, float height, float near = 0.1f, float far = 100.f) : Scene(height, width) {}
        virtual ~OGL_Scene() {}
        // methods
    public:
        virtual void init() override = 0;
        virtual void update() override = 0;
        virtual void destroy() override = 0;
        virtual void render() override = 0;
    };

    struct Rendering_Style
    {
        float exposure = 1.0f;
        float gamma = 2.2f;
    };

    class OGL_Scene_3D : public OGL_Scene
    {
        // attributes
    public:
        Rendering::Camera_Ptr camera_pos;
        Rendering::Camera_Ptr camera_front;
        Rendering::Camera_Ptr camera_up;
        Rendering::Camera_Ptr camera_right;

        Rendering_Style style;
        std::vector<OGL_Model_Ptr> models;
        // constructors and deconstructor
    public:
        OGL_Scene_3D(float width, float height, float near = 0.1f, float far = 100.f) : OGL_Scene(width, height, near, far) {}
        virtual ~OGL_Scene_3D() {}
        // methods
    public:
        virtual void init() override = 0;
        virtual void update() override = 0;
        virtual void destroy() override = 0;
        virtual void render() override = 0;
    };

} // namespace scene

#endif // !RENDERING_SCENE_H
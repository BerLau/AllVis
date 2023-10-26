#pragma once
#ifndef RENDERING_SCENE_H
#define RENDERING_SCENE_H

#include <glad/glad.h>
#include <memory>
#include <vector>

#include "models.h"
#include "camera.h"
#include "light.h"
#include "fbo.h"

namespace Rendering
{
    class Scene;
    using Scene_U_Ptr = std::unique_ptr<Scene>;
    using Scene_S_Ptr = std::shared_ptr<Scene>;
    using Scene_W_Ptr = std::weak_ptr<Scene>;
    using Scene_Ptr = Scene_U_Ptr;

    class Scene : public Core::Configurable
    {
        // structures
    public:
    public:
        float width = 0.f;
        float height = 0.f;
        float aspect = 0.f;
        Configurable* focused_object = nullptr;
        // constructors and deconstructor
    public:
        Scene(float width, float height)
            : width(width),
              height(height),
              aspect(height == 0.f ? 0.f : width / height)
        {
            this->resize(width, height);
        }
        virtual ~Scene() {}
        // methods
    public:
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void destroy() = 0;
        virtual void render() = 0;

        virtual void resize(float width, float height)
        {
            this->width = width;
            this->height = height;
            this->aspect = height == 0.f ? 0 : width / height;
        }
    };

    class OGL_Scene;
    using OGL_Scene_U_Ptr = std::unique_ptr<OGL_Scene>;
    using OGL_Scene_S_Ptr = std::shared_ptr<OGL_Scene>;
    using OGL_Scene_W_Ptr = std::weak_ptr<OGL_Scene>;
    using OGL_Scene_Ptr = OGL_Scene_U_Ptr;

    class OGL_Scene : public Scene
    {
        // structures
    public:
        struct OGL_Model_Item
        {
            OGL_Model_Ptr value = nullptr;
            bool is_active = true;
        };
        // attributes
    public:
        float near = 0.1f;
        float far = 100.0f;
        float fov = 45.0f;
        Core::Vector4 bg_color = Core::Vector4(1.f, 1.f, 1.f, 1.0f);
        GLuint fbo = 0, fb_tex = 0, rbo = 0;
        FBO_Ptr fbo_ptr = nullptr;
        Rendering::Shader_Program *shader = nullptr;
        std::vector<OGL_Model_Item> models;
        // constructors and deconstructor
    public:
        // cast properties to OGL_Scene::Properties
        OGL_Scene(float width, float height)
            : Scene(height, width)
        {
            init();
        }
        virtual ~OGL_Scene()
        {
            destroy();
        }
        // methods
    public:
        virtual void init()
        {
            create_framebuffer();
            fbo_ptr = FBO_Ptr(new FBO(this->width, this->height));
            fbo_ptr->bind();
            auto color_attachment = Texture_Ptr(new Texture());
            color_attachment->bind();
            color_attachment->resize(this->width, this->height);
            // color_attachment->set_sampler(Sampler_Manager::instance().get_sampler("default"));
            fbo_ptr->attach_texture(color_attachment);
            auto depth_attachment = Render_Buffer_Ptr(new Render_Buffer(this->width, this->height));
            fbo_ptr->attach_render_buffer(std::move(depth_attachment));
            fbo_ptr->unbind();
            color_attachment->unbind();
            depth_attachment->unbind();
            fbo_ptr->check_status();
        }
        virtual void update() override = 0;
        virtual void destroy()
        {
            unbind_framebuffer();
            release_framebuffer();
        }
        virtual void render() override = 0;
        virtual void resize(float width, float height) override
        {
            if (EQUAL_F(this->width, width) && EQUAL_F(this->height, height))
            {
                return;
            }
            else
            {
                Scene::resize(width, height);
                this->resize_framebuffer();
                fbo_ptr->resize(width, height);
            }
        }
        void create_framebuffer();
        void resize_framebuffer();
        void bind_framebuffer();
        void unbind_framebuffer();
        void release_framebuffer();
    };

    class OGL_Scene_3D;
    using OGL_Scene_3D_U_Ptr = std::unique_ptr<OGL_Scene_3D>;
    using OGL_Scene_3D_S_Ptr = std::shared_ptr<OGL_Scene_3D>;
    using OGL_Scene_3D_W_Ptr = std::weak_ptr<OGL_Scene_3D>;
    using OGL_Scene_3D_Ptr = OGL_Scene_3D_U_Ptr;

    class OGL_Scene_3D : public OGL_Scene
    {
    public: // structures
        struct Camera_Item
        {
            Camera_Ptr value = nullptr;
            bool is_active = true;
        };
        struct Light_Item
        {
            Light_Ptr value = nullptr;
            bool is_active = true;
        };
        // attributes
    public:
        float gamma=2.2f;
        float exposure = 1.0f;
        // constructors and deconstructor
        Rendering::Texture_Ptr sample_texture;

        std::vector<Camera_Item> cameras;
        int active_camera_index;
        // Rendering::OGL_Model_U_Ptr cube_model;
        std::vector<Light_Item> lights;
        // constructors and deconstructor
    public:
        OGL_Scene_3D(float width, float height);
        virtual ~OGL_Scene_3D();

        // methods
    public:
        virtual void init();
        virtual void update();
        virtual void destroy();
        virtual void render();

        void set_shader(Rendering::Shader_Program *shader)
        {
            this->shader = shader;
        }
    };

} // namespace scene

#endif // !RENDERING_SCENE_H
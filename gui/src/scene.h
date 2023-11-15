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
        Configurable *focused_object = nullptr;
        // constructors and deconstructor
    public:
        Scene(float width, float height)
            : Configurable("Scene"),
              width(width),
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
        // GLuint fbo = 0, fb_tex = 0, rbo = 0;
        FBO_Ptr final_fbo = nullptr;
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
        virtual void init();

        virtual void update() override = 0;
        virtual void destroy() {}
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
                final_fbo->resize(width, height);
            }
        }
        virtual Texture *get_output_texture()
        {
            return final_fbo->get_color_attachment();
        }
        virtual void init_final_fbo();
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
        float gamma = 2.2f;
        float exposure = 1.0f;
        std::string skybox_path;
        std::vector<Camera_Item> cameras;
        int active_camera_index;
        std::vector<Light_Item> lights;
        FBO_Ptr pbr_fbo = nullptr;
        FBO_Ptr cubemap_fbo = nullptr;

        FBO_Ptr irradiance_fbo = nullptr;
        FBO_Ptr prefilter_fbo = nullptr;
        FBO_Ptr brdf_fbo = nullptr;
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
        virtual void resize(float width, float height) override
        {
            if (EQUAL_F(this->width, width) && EQUAL_F(this->height, height))
            {
                return;
            }
            else
            {
                OGL_Scene::resize(width, height);
                pbr_fbo->resize(width, height);
            }
        }
        virtual Texture *get_output_texture()
        {
            // return brdf_fbo->get_color_attachment();
            return final_fbo->get_color_attachment();
        }
        void equi_to_cubemap(const Core::Matrix4 &projection = Geometry::perspective(Geometry::radians(90.0f), 1.0f, 0.1f, 10.0f));
        void precompute_envrionment();
        void compute_env_irradiance(Texture *env_cubemap);
        void compute_env_prefilter(Texture *env_cubemap);
        void compute_brdf_lut();
        void update_environment();

    protected:
        void render_skybox(const Core::Matrix4 &view, const Core::Matrix4 &projection);
        void render_lights(const Core::Matrix4 &view, const Core::Matrix4 &projection);
        void render_pbr(const Core::Matrix4 &view, const Core::Matrix4 &projection);
        void tone_mapping(Texture *texture);

    private:
        void init_pbr_fbo();
        void init_cubemap_fbo();
        void init_irradiance_fbo();
        void init_prefilter_fbo();
        void init_brdf_fbo();
    };

    static const Core::Matrix4 cube_projection = Geometry::perspective(Geometry::radians(90.0f), 1.0f, 0.1f, 10.0f);
    static const Core::Matrix4 cube_views[6] = {
        Geometry::look_at(Core::Vector3(0.0f, 0.0f, 0.0f), Core::Vector3(1.0f, 0.0f, 0.0f), Core::Vector3(0.0f, -1.0f, 0.0f)),
        Geometry::look_at(Core::Vector3(0.0f, 0.0f, 0.0f), Core::Vector3(-1.0f, 0.0f, 0.0f), Core::Vector3(0.0f, -1.0f, 0.0f)),
        Geometry::look_at(Core::Vector3(0.0f, 0.0f, 0.0f), Core::Vector3(0.0f, 1.0f, 0.0f), Core::Vector3(0.0f, 0.0f, 1.0f)),
        Geometry::look_at(Core::Vector3(0.0f, 0.0f, 0.0f), Core::Vector3(0.0f, -1.0f, 0.0f), Core::Vector3(0.0f, 0.0f, -1.0f)),
        Geometry::look_at(Core::Vector3(0.0f, 0.0f, 0.0f), Core::Vector3(0.0f, 0.0f, 1.0f), Core::Vector3(0.0f, -1.0f, 0.0f)),
        Geometry::look_at(Core::Vector3(0.0f, 0.0f, 0.0f), Core::Vector3(0.0f, 0.0f, -1.0f), Core::Vector3(0.0f, -1.0f, 0.0f))};

} // namespace scene

#endif // !RENDERING_SCENE_H
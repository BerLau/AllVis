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

    class Scene : public Core::Configurable
    {
        // structures

    public:
        class Properties
        {
        public:
            float width = 0.f;
            float height = 0.f;
            float aspect = 0.f;
            virtual bool is_accessible() { return true; }
        };
        using Properties_Ptr = std::unique_ptr<Properties>;
        // attributes
    public:
        Properties_Ptr properties;
        // constructors and deconstructor
    public:
        Scene(float width, float height, Properties *properties) : properties(properties)
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
            properties->width = width;
            properties->height = height;
            properties->aspect = width / height;
        }
        virtual Properties *get_properties() { return properties.get(); }
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

        class Properties : public Scene::Properties
        {
        public:
            float near = 0.1f;
            float far = 100.f;
            float fov = 45.f;
            float bg_color[4] = {0.f, 0.f, 0.f, 1.f};
        };
        // attributes
    public:
        GLuint fbo, fb_tex, rbo;
        std::vector<OGL_Model_Item> models;
        Rendering::Shader_Program *shader;
        // constructors and deconstructor
    public:
        // cast properties to OGL_Scene::Properties
        OGL_Scene(float width, float height, Properties *properties)
            : fb_tex(0), fbo(0), rbo(0), shader(nullptr),
              Scene(height, width, properties)
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
            if (EQUAL_F(width, properties->width) && EQUAL_F(height, properties->height))
            {
                return;
            }
            else
            {
                Scene::resize(width, height);
                this->resize_framebuffer();
            }
        }

        Properties *get_properties()
        {
            return dynamic_cast<Properties *>(properties.get());
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
        class Properties : public OGL_Scene::Properties
        {
        public:
            float gamma = 2.2f;
            float exposure = 1.f;
        };
        // attributes
    public:
        // constructors and deconstructor
        Rendering::Texture_Ptr sample_texture;

        std::vector<Camera_Item> cameras;
        // Rendering::OGL_Model_U_Ptr cube_model;
        std::vector<Light_Item> lights;
        // constructors and deconstructor
    public:
        OGL_Scene_3D(float width, float height, Properties *properties);
        virtual ~OGL_Scene_3D();

        // methods
    public:
        virtual void init();
        virtual void update();
        virtual void destroy();
        virtual void render();

        virtual Properties *get_properties()
        {
            return dynamic_cast<Properties *>(properties.get());
        }

        void set_shader(Rendering::Shader_Program *shader)
        {
            this->shader = shader;
        }
    };

} // namespace scene

#endif // !RENDERING_SCENE_H
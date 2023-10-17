#include "scene.h"
namespace Rendering
{
    void OGL_Scene::create_framebuffer()
    {
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

        glGenTextures(1, &this->fb_tex);
        glBindTexture(GL_TEXTURE_2D, this->fb_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, properties->width, properties->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->fb_tex, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }

        // add depth buffer

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, properties->width, properties->height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OGL_Scene::resize_framebuffer()
    {
        glBindTexture(GL_TEXTURE_2D, this->fb_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, properties->width, properties->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_RENDERBUFFER, this->rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, properties->width, properties->height);
        glBindBuffer(GL_RENDERBUFFER, 0);
    }

    void OGL_Scene::bind_framebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    }

    void OGL_Scene::unbind_framebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OGL_Scene::release_framebuffer()
    {
        if (this->fbo)
        {
            glDeleteFramebuffers(1, &this->fbo);
        }
        if (this->fb_tex)
        {
            glDeleteTextures(1, &this->fb_tex);
        }
        if (this->rbo)
        {
            glDeleteRenderbuffers(1, &this->rbo);
        }
    }

    OGL_Scene_3D::OGL_Scene_3D(float width, float height, Properties *properties)
        : OGL_Scene(width, height, properties),
          lights(),
          cameras(),
          active_camera_index(-1)
    {
        init();
    }

    OGL_Scene_3D::~OGL_Scene_3D()
    {
        destroy();
    }

    void OGL_Scene_3D::init()
    {
        auto cube_model = Rendering::OGL_Model_U_Ptr(new Rendering::Cube_Model());
        cube_model->name = "cube 1";

        auto cube_mode_2 = Rendering::OGL_Model_U_Ptr(new Rendering::Cube_Model());
        cube_mode_2->name = "cube 2";
        cube_mode_2->transform->set_position(Core::Vector3(-1.f, 0.0f, 0.0f));
        models.push_back({std::move(cube_model), true});
        models.push_back({std::move(cube_mode_2), true});
        // set light
        auto light = Rendering::Light_Ptr(new Rendering::Light());
        light->name = "light 1";
        light->set_position(Core::Vector3(2.0f, 3.0f, 3.0f));
        // light->set_direction(Core::Vector3(-1.0f, -1.0f, -1.0f));
        light->set_color(Core::Vector3(0.75f, 0.75f, 0.75f));
        light->set_type(Rendering::Light::POINT_LIGHT);
        lights.push_back({std::move(light), true});

        auto light_2 = Rendering::Light_Ptr(new Rendering::Light());
        light_2->name = "light 2";
        light_2->set_position(Core::Vector3(-2.0f, 3.0f, 3.0f));
        // light_2->set_direction(Core::Vector3(-1.0f, -1.0f, -1.0f));
        light_2->set_color(Core::Vector3(0.75f, 0.75f, 0.75f));
        light_2->set_type(Rendering::Light::POINT_LIGHT);
        lights.push_back({std::move(light_2), true});

        auto camera = Rendering::Camera_Ptr(new Rendering::Camera(Core::Vector3(0.0f, 2.0f, 4.0f)));
        camera->name = "camera 1";
        camera->focus_on(Core::Vector3(0.f, 0.0f, 0.0f), Core::Vector3(0.0f, 1.0f, 0.0f));
        cameras.push_back({std::move(camera), true});
        active_camera_index = 0;

        auto camera_2 = Rendering::Camera_Ptr(new Rendering::Camera(Core::Vector3(3.0f, 0.0f, 0.0f)));
        camera_2->focus_on(Core::Vector3(0.f, 0.0f, 0.0f), Core::Vector3(0.0f, 1.0f, 0.0f));
        camera_2->name = "camera 2";
        cameras.push_back({std::move(camera_2), false});
    }
    void OGL_Scene_3D::update()
    {
        // TODO: update scene
    }
    void OGL_Scene_3D::destroy()
    {
        // TODO: destroy scene
    }
    void OGL_Scene_3D::render()
    {
        using namespace Core;
        bind_framebuffer();
        auto props = get_properties();
        // temporarily set the light properties
        glClearColor(props->bg_color[0], props->bg_color[1], props->bg_color[2], props->bg_color[3]);
        glViewport(0, 0, props->width, props->height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Core::Matrix4 projection = Geometry::perspective(Geometry::radians(props->fov), props->width / props->height, props->near, props->far);
        Core::Matrix4 view = Core::Matrix4::identity();
        Core::Vector3 view_position = Core::Vector3(0.0f, 0.0f, 0.0f);
        if (active_camera_index >= 0)
        {
            auto &active_camera = cameras[active_camera_index].value;
            view = Geometry::look_at(active_camera->get_position(), active_camera->get_focus(), active_camera->get_up());
            view_position = active_camera->get_position();
        }

        shader->activate();
        shader->set_mat4("u_view", view.data());
        shader->set_mat4("u_projection", projection.data());
        shader->set_vec3("u_view_position", view_position.data());
        int active_light_num = 0;
        for (auto &light : lights)
        {
            if (light.is_active)
            {
                shader->set_light("u_lights[" + std::to_string(active_light_num++) + "]", *light.value);
            }
        }
        shader->set_int("u_light_num", active_light_num);
        for (auto &model : models)
        {
            if (model.is_active)
            {
                model.value->bind_shader(shader);
                model.value->draw();
                model.value->unbind_shader();
            }
        }
        unbind_framebuffer();
    }
};

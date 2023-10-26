#include "scene.h"
namespace Rendering
{
    void OGL_Scene::create_framebuffer()
    {
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

        glGenTextures(1, &this->fb_tex);
        glBindTexture(GL_TEXTURE_2D, this->fb_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
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
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->width, this->height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OGL_Scene::resize_framebuffer()
    {
        glBindTexture(GL_TEXTURE_2D, this->fb_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_RENDERBUFFER, this->rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->width, this->height);
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

    OGL_Scene_3D::OGL_Scene_3D(float width, float height)
        : OGL_Scene(width, height),
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
        auto cube_model = Rendering::OGL_Model_Ptr(new Rendering::OGL_Model(
            "Cube 1",
            Rendering::OGL_Mesh::cube_mesh(1.0f, 1.0f, 1.0f)));
        cube_model->name = "cube 1";
        cube_model->material->color = Core::Vector3(RANDOM_RANGE_F(0.2, 1.0), RANDOM_RANGE_F(0.2, 1.0), RANDOM_RANGE_F(0.2, 1.0));
        cube_model->material->metallic = RANDOM_RANGE_F(0.2, 1.0);
        cube_model->material->roughness = RANDOM_RANGE_F(0.2, 1.0);
        cube_model->material->ao = RANDOM_RANGE_F(0.2, 1.0);

        auto cube_model_2 = Rendering::OGL_Model_Ptr(new Rendering::OGL_Model(
            "Cube 2",
            Rendering::OGL_Mesh::sphere_mesh()));
        cube_model_2->transform->set_position(Core::Vector3(-1.f, 0.0f, 0.0f));
        cube_model_2->material->color = Core::Vector3(RANDOM_RANGE_F(0.2, 1.0), RANDOM_RANGE_F(0.2, 1.0), RANDOM_RANGE_F(0.2, 1.0));
        cube_model_2->material->metallic = RANDOM_RANGE_F(0.2, 1.0);
        cube_model_2->material->roughness = RANDOM_RANGE_F(0.2, 1.0);
        cube_model_2->material->ao = RANDOM_RANGE_F(0.2, 1.0);

        models.push_back({std::move(cube_model), true});
        models.push_back({std::move(cube_model_2), true});
        // set light
        auto light = Rendering::Light_Ptr(new Rendering::Light());
        light->name = "light 1";
        light->set_position(Core::Vector3(1.0f, 1.0f, 2.0f));
        light->spot_on(Core::Vector3(0.0f, 0.0f, 0.0f));
        float r = RANDOM_RANGE_F(0.2, 1.0);
        light->color = Core::Vector3(r, r, r);
        light->intensity = RANDOM_RANGE_F(0.2, 1.0);
        light->type = Rendering::Light::POINT_LIGHT;
        lights.push_back({std::move(light), true});

        auto light_2 = Rendering::Light_Ptr(new Rendering::Light());
        light_2->name = "light 2";
        light_2->set_position(Core::Vector3(-1.0f, 1.0f, 1.0f));
        light_2->spot_on(Core::Vector3(0.0f, 0.0f, 0.0f));
        r = RANDOM_RANGE_F(0.2, 1.0);
        light_2->color = Core::Vector3(r, r, r);
        light_2->intensity = RANDOM_RANGE_F(0.2, 1.0);
        light_2->type = Rendering::Light::POINT_LIGHT;
        lights.push_back({std::move(light_2), true});

        auto camera = Rendering::Camera_Ptr(new Rendering::Camera(Core::Vector3(0.0f, 0.0f, 4.0f)));
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
        // bind_framebuffer();
        fbo_ptr->bind();
        // temporarily set the light properties
        glClearColor(this->bg_color[0], this->bg_color[1], this->bg_color[2], this->bg_color[3]);
        glViewport(0, 0, this->width, this->height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Core::Matrix4 projection = Geometry::perspective(Geometry::radians(this->fov), this->aspect, this->near, this->far);
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
                light.value->write_to_shader("u_lights", active_light_num++, shader);
            }
        }
        shader->set_int("u_light_num", active_light_num);
        for (auto &model : models)
        {
            if (model.is_active)
            {
                model.value->draw(shader);
            }
        }
        auto light_shader = Rendering::shader_program_factory.find_shader_program("light_shader");
        if (light_shader)
        {
            light_shader->activate();
            light_shader->set_mat4("u_view", view.data());
            light_shader->set_mat4("u_projection", projection.data());
            light_shader->set_vec3("u_view_position", view_position.data());
            for (auto &light : lights)
            {
                if (light.is_active)
                {
                    light.value->visualize(light_shader);
                }
            }
            light_shader->deactivate();
        }
        // unbind_framebuffer();
        fbo_ptr->unbind();
    }
};

#include "scene.h"
namespace Rendering
{
    void OGL_Scene::init()
    {
        init_final_fbo();
    }

    void OGL_Scene::init_final_fbo()
    {
        final_fbo = FBO_Ptr(new FBO(this->width, this->height));
        final_fbo->bind();
        auto color_attachment = Texture_Ptr(new Texture());
        color_attachment->bind();
        color_attachment->resize(this->width, this->height);
        final_fbo->attach_texture(color_attachment);
        color_attachment->unbind();
        final_fbo->set_draw_buffers();
        final_fbo->check_status();
        final_fbo->unbind();
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
        init_pbr_fbo();
        init_cubemap_fbo();

        auto cube_model = Rendering::OGL_Model_Ptr(new Rendering::OGL_Model(
            "Cube 1",
            Rendering::OGL_Mesh::cube_mesh(1.0f, 1.0f, 1.0f)));
        cube_model->material->color = Core::Vector3(RANDOM_RANGE_F(0.2, 1.0), RANDOM_RANGE_F(0.2, 1.0), RANDOM_RANGE_F(0.2, 1.0));
        cube_model->material->metallic = RANDOM_RANGE_F(0.2, 1.0);
        cube_model->material->roughness = RANDOM_RANGE_F(0.2, 1.0);
        cube_model->material->ao = RANDOM_RANGE_F(0.1, 0.5);

        auto sphere_model = Rendering::OGL_Model_Ptr(new Rendering::OGL_Model(
            "Sphere 1",
            Rendering::OGL_Mesh::sphere_mesh()));
        sphere_model->transform->set_position(Core::Vector3(-1.f, 0.0f, 0.0f));
        sphere_model->material->color = Core::Vector3(RANDOM_RANGE_F(0.2, 1.0), RANDOM_RANGE_F(0.2, 1.0), RANDOM_RANGE_F(0.2, 1.0));
        sphere_model->material->metallic = RANDOM_RANGE_F(0.2, 1.0);
        sphere_model->material->roughness = RANDOM_RANGE_F(0.2, 1.0);
        sphere_model->material->ao = RANDOM_RANGE_F(0.1, 0.5);

        models.push_back({std::move(cube_model), true});
        models.push_back({std::move(sphere_model), true});
        // set lights
        // set directional light
        auto light = Rendering::Light_Ptr(new Rendering::Light());
        light->name = "light 0";
        light->set_position(Core::Vector3(2.0f, 2.0f, 2.0f));
        light->spot_on(Core::Vector3(0.0f, 0.0f, 0.0f));
        light->color = Core::Vector3(1.0f, 1.0f, 1.0f);
        light->intensity = 1.0f;
        light->type = Rendering::Light::PARALLEL_LIGHT;
        lights.push_back({std::move(light), true});

        // set point lights
        for (int i = 1; i <= 4; i++)
        {
            auto light = Rendering::Light_Ptr(new Rendering::Light());
            light->name = "light " + std::to_string(i);
            light->set_position(Core::Vector3(RANDOM_RANGE_F(-2.5, 2.5), RANDOM_RANGE_F(-2.5, 2.5), RANDOM_RANGE_F(-2.5, 2.5)));
            light->spot_on(Core::Vector3(0.0f, 0.0f, 0.0f));
            float r = RANDOM_RANGE_F(0.2, 1.0);
            light->color = Core::Vector3(r, r, r);
            light->intensity = RANDOM_RANGE_F(0.2, 1.0);
            light->type = Rendering::Light::POINT_LIGHT;
            lights.push_back({std::move(light), true});
        }

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
        Matrix4 projection = Geometry::perspective(Geometry::radians(this->fov), this->aspect, this->near, this->far);
        Matrix4 view = Core::Matrix4::identity();
        if (active_camera_index >= 0)
        {
            auto &active_camera = cameras[active_camera_index].value;
            view = Geometry::look_at(active_camera->get_position(), active_camera->get_focus(), active_camera->get_up());
        }

        pbr_fbo->bind();
        pbr_fbo->clear(this->bg_color);
        render_pbr(view, projection);
        render_lights(view, projection);
        render_skybox(view, projection);
        pbr_fbo->unbind();

        final_fbo->bind();
        final_fbo->clear(this->bg_color);
        tone_mapping(pbr_fbo->get_color_attachment(0));
        final_fbo->unbind();
    }
    void OGL_Scene_3D::render_skybox(const Core::Matrix4 &view, const Core::Matrix4 &projection)
    {
        auto skybox_texture = cubemap_fbo->get_color_attachment(0);
        if (!skybox_texture)
        {
            return;
        }
        auto skybox_shader = Rendering::shader_program_factory.find_shader_program("skybox_shader");
        skybox_shader->activate();
        skybox_shader->set_mat4("u_view", view.data());
        skybox_shader->set_mat4("u_projection", projection.data());
        glActiveTexture(GL_TEXTURE0);
        skybox_texture->bind();
        skybox_shader->set_int("u_skybox", 0);
        Rendering::OGL_Mesh::instanced_cube_mesh()->render(skybox_shader);
        skybox_texture->unbind();
        skybox_shader->deactivate();
    }

    void OGL_Scene_3D::render_lights(const Core::Matrix4 &view, const Core::Matrix4 &projection)
    {
        auto light_shader = Rendering::shader_program_factory.find_shader_program("light_shader");
        if (light_shader)
        {
            light_shader->activate();
            light_shader->set_mat4("u_view", view.data());
            light_shader->set_mat4("u_projection", projection.data());
            for (auto &light : lights)
            {
                if (light.is_active)
                {
                    light.value->visualize(light_shader);
                }
            }
            light_shader->deactivate();
        }
    }

    void OGL_Scene_3D::render_pbr(const Core::Matrix4 &view, const Core::Matrix4 &projection)
    {
        auto pbr_shader = Rendering::shader_program_factory.find_shader_program("pbr_shader");
        pbr_shader->activate();
        pbr_shader->set_mat4("u_view", view.data());
        pbr_shader->set_mat4("u_projection", projection.data());
        pbr_shader->set_bool("environment_mapping", false);
        int active_light_num = 0;
        for (auto &light : lights)
        {
            if (light.is_active)
            {
                light.value->write_to_shader("u_lights", active_light_num++, pbr_shader);
            }
        }
        pbr_shader->set_int("u_light_num", active_light_num);
        for (auto &model : models)
        {
            if (model.is_active)
            {
                model.value->draw(pbr_shader);
            }
        }
        pbr_shader->deactivate();
    }

    void OGL_Scene_3D::tone_mapping(Texture *texture)
    {
        auto tone_mapping_shader = Rendering::shader_program_factory.find_shader_program("tone_mapping_shader");
        tone_mapping_shader->activate();
        tone_mapping_shader->set_float("u_gamma", this->gamma);
        tone_mapping_shader->set_float("u_exposure", this->exposure);
        glActiveTexture(GL_TEXTURE0);
        texture->bind();
        tone_mapping_shader->set_int("u_image", 0);
        Rendering::OGL_Mesh::instanced_quad_mesh()->render(tone_mapping_shader);
        texture->unbind();
        tone_mapping_shader->deactivate();
    }

    void OGL_Scene_3D::init_pbr_fbo()
    {
        // set pbr fbo
        Texture::Format float_2d_format(GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT);
        pbr_fbo = FBO_Ptr(new FBO(this->width, this->height));
        pbr_fbo->bind();

        auto color_attachment = Texture_Ptr(new Texture(float_2d_format));
        color_attachment->bind();
        color_attachment->resize(this->width, this->height);
        pbr_fbo->attach_texture(color_attachment);
        color_attachment->unbind();

        auto color_attachment_2 = Texture_Ptr(new Texture(float_2d_format));
        color_attachment_2->bind();
        color_attachment_2->resize(this->width, this->height);
        pbr_fbo->attach_texture(color_attachment_2);
        color_attachment_2->unbind();

        auto depth_attachment = Render_Buffer_Ptr(new Render_Buffer(this->width, this->height));
        pbr_fbo->attach_render_buffer(std::move(depth_attachment));
        depth_attachment->unbind();
        pbr_fbo->set_draw_buffers();
        pbr_fbo->check_status();

        pbr_fbo->unbind();
    }

    void OGL_Scene_3D::init_cubemap_fbo()
    {
        cubemap_fbo = FBO_Ptr(new FBO(1024, 1024));
        cubemap_fbo->bind();

        auto cubemap_format = Texture::Format(GL_TEXTURE_CUBE_MAP, GL_RGB16F, GL_RGB, GL_FLOAT);
        auto attachment = Texture_Ptr(new Texture(cubemap_format));
        attachment->bind();
        attachment->resize(1024, 1024);
        cubemap_fbo->attach_texture(attachment);
        attachment->unbind();

        auto depth_attachment_2 = Render_Buffer_Ptr(new Render_Buffer(1024, 1024));
        cubemap_fbo->attach_render_buffer(std::move(depth_attachment_2));
        depth_attachment_2->unbind();

        cubemap_fbo->set_draw_buffers();
        cubemap_fbo->check_status();
        cubemap_fbo->unbind();
    }

    void OGL_Scene_3D::equi_to_cubemap(const Core::Matrix4 &projection)
    {
        auto equi_texture = Texture_Manager::instance().get_texture(this->skybox_path);
        if (!equi_texture)
        {
            return;
        }
        static const Core::Matrix4 views[6] = {
            Geometry::look_at(Core::Vector3(0.0f, 0.0f, 0.0f), Core::Vector3(1.0f, 0.0f, 0.0f), Core::Vector3(0.0f, -1.0f, 0.0f)),
            Geometry::look_at(Core::Vector3(0.0f, 0.0f, 0.0f), Core::Vector3(-1.0f, 0.0f, 0.0f), Core::Vector3(0.0f, -1.0f, 0.0f)),
            Geometry::look_at(Core::Vector3(0.0f, 0.0f, 0.0f), Core::Vector3(0.0f, 1.0f, 0.0f), Core::Vector3(0.0f, 0.0f, 1.0f)),
            Geometry::look_at(Core::Vector3(0.0f, 0.0f, 0.0f), Core::Vector3(0.0f, -1.0f, 0.0f), Core::Vector3(0.0f, 0.0f, -1.0f)),
            Geometry::look_at(Core::Vector3(0.0f, 0.0f, 0.0f), Core::Vector3(0.0f, 0.0f, 1.0f), Core::Vector3(0.0f, -1.0f, 0.0f)),
            Geometry::look_at(Core::Vector3(0.0f, 0.0f, 0.0f), Core::Vector3(0.0f, 0.0f, -1.0f), Core::Vector3(0.0f, -1.0f, 0.0f))};

        auto equi_to_cube_shader = Rendering::shader_program_factory.find_shader_program("equi_to_cube_shader");
        equi_to_cube_shader->activate();
        equi_to_cube_shader->set_mat4("u_projection", projection.data());
        cubemap_fbo->bind();
        glViewport(0, 0, 1024, 1024);
        glActiveTexture(GL_TEXTURE0);
        equi_texture->bind();
          if(equi_texture->format.is_hdr)
        {
            equi_to_cube_shader->set_bool("u_is_hdr", true);
        }
        else
        {
            equi_to_cube_shader->set_bool("u_is_hdr", false);
        }
        equi_to_cube_shader->set_int("u_equirectangular_map", 0);
        // set gamma correction
        equi_to_cube_shader->set_float("u_gamma", this->gamma);
        // set exposure
        equi_to_cube_shader->set_float("u_exposure", this->exposure);
        auto skybox_texture = cubemap_fbo->get_color_attachment(0);
        for (int i = 0; i < 6; i++)
        {
            equi_to_cube_shader->set_mat4("u_view", views[i].data());
            // set the render target to be the cube face
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, skybox_texture->texture_id, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Rendering::OGL_Mesh::instanced_cube_mesh()->render(equi_to_cube_shader);
        }
        equi_texture->unbind();
        equi_to_cube_shader->deactivate();
        cubemap_fbo->unbind();
    }

    void OGL_Scene_3D::update_gamma_exposure()
    {
        equi_to_cubemap();
    }
};

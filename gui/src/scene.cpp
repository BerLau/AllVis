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
        init_irradiance_fbo();
        init_prefilter_fbo();
        init_brdf_fbo();

        // add 5x5x5 cubes
        const int row = 3; 
        // for (int i = 0; i < row; ++i)
        // {
        //     float z = -float(row) + i * float(row)/2.f;
        //     for (int j = 0; j < 3; ++j)
        //     {
        //         float x = -float(row) + j * float(row)/2.f;
        //         for (int k = 0; k < row; ++k)
        //         {
        //             float y = -float(row) + k * float(row)/2.f;
        //             Core::Vector3 pos = Core::Vector3(x, y, z);
        //             auto cube_model = Rendering::OGL_Model_Ptr(new Rendering::OGL_Model(
        //                 "Cube 1",
        //                 Rendering::OGL_Mesh::cube_mesh(1.0f, 1.0f, 1.0f)));
        //             cube_model->material->color = Core::Vector3(RANDOM_RANGE_F(0.2, 1.0), RANDOM_RANGE_F(0.2, 1.0), RANDOM_RANGE_F(0.2, 1.0));
        //             cube_model->transform->scale(0.5);
        //             cube_model->transform->set_position(pos);
        //             cube_model->material->metallic = RANDOM_RANGE_F(0.2, 1.0);
        //             cube_model->material->roughness = RANDOM_RANGE_F(0.2, 1.0);
        //             cube_model->material->ao = RANDOM_RANGE_F(0.1, 0.5);
        //             models.push_back({std::move(cube_model), true});
        //         }
        //     }
        // }

        // float off_set = float(row)/4.f;
        float off_set = 0.f;
        // add 5x5x5 spheres
        for (int i = 0; i < row; ++i)
        {
            float z = -float(row) + i * float(row)/2.f + off_set;
            for (int j = 0; j < row; ++j)
            {
                float x = -float(row) + j * float(row)/2.f+ off_set;
                for (int k = 0; k < row; ++k)
                {
                    float y = -float(row) + k * float(row)/2.f + off_set;
                    Core::Vector3 pos = Core::Vector3(x, y, z);
                    auto sphere_model = Rendering::OGL_Model_Ptr(new Rendering::OGL_Model(
                        "Sphere 1",
                        Rendering::OGL_Mesh::sphere_mesh(1.0, 32, 32)));
                    sphere_model->transform->set_position(pos);
                    sphere_model->material->color = Core::Vector3(RANDOM_RANGE_F(0.2, 1.0), RANDOM_RANGE_F(0.2, 1.0), RANDOM_RANGE_F(0.2, 1.0));
                    sphere_model->material->metallic = RANDOM_RANGE_F(0.2, 1.0);
                    sphere_model->material->roughness = RANDOM_RANGE_F(0.2, 1.0);
                    sphere_model->material->ao = RANDOM_RANGE_F(0.1, 0.5);
                    sphere_model->transform->scale(0.5);
                    models.push_back({std::move(sphere_model), true});
                }
            }
        }

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
        for (int i = 1; i <= 32; i++)
        {
            auto light = Rendering::Light_Ptr(new Rendering::Light());
            light->name = "light " + std::to_string(i);
            light->set_position(Core::Vector3(RANDOM_RANGE_F(-2.5, 2.5), RANDOM_RANGE_F(-2.5, 2.5), RANDOM_RANGE_F(-2.5, 2.5)));
            light->spot_on(Core::Vector3(0.0f, 0.0f, 0.0f));
            Core::Vector3 color = Core::Vector3(RANDOM_RANGE_F(0.2, 1.0), RANDOM_RANGE_F(0.2, 1.0), RANDOM_RANGE_F(0.2, 1.0));
            light->color = color;
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

        // set skybox
        skybox_path = "textures/TCom_JapanParkingGarageB_4K_hdri_sphere_tone.jpg";
        auto sky_texture = load_texture(skybox_path);
        Texture_Manager::instance().add_texture(skybox_path, sky_texture);
        update_environment();
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
        // auto skybox_texture =  irradiance_fbo->get_color_attachment(0);
        // auto skybox_texture = prefilter_fbo->get_color_attachment(0);
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

        // set environment map
        pbr_shader->set_bool("environment_mapping", true);

        auto irradiance_map = irradiance_fbo->get_color_attachment(0);
        if (irradiance_map)
        {
            glActiveTexture(GL_TEXTURE0);
            irradiance_map->bind();
            pbr_shader->set_int("u_irradiance_map", 0);
        }
        else
        {
            pbr_shader->set_bool("environment_mapping", false);
        }

        // set prefilter map
        auto prefilter_map = prefilter_fbo->get_color_attachment(0);
        if (prefilter_map)
        {
            glActiveTexture(GL_TEXTURE1);
            prefilter_map->bind();
            pbr_shader->set_int("u_prefilter_map", 1);
        }
        else
        {
            pbr_shader->set_bool("environment_mapping", false);
        }

        // set brdf lut
        auto brdf_lut = brdf_fbo->get_color_attachment(0);
        if (brdf_lut)
        {
            glActiveTexture(GL_TEXTURE2);
            brdf_lut->bind();
            pbr_shader->set_int("u_brdf_lut", 2);
        }
        else
        {
            pbr_shader->set_bool("environment_mapping", false);
        }

        pbr_shader->set_mat4("u_view", view.data());
        pbr_shader->set_mat4("u_projection", projection.data());

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
                model.value->material->bind(3);
                model.value->material->write_to_shader("u_material", pbr_shader,3);
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
        pbr_fbo->set_draw_buffers();
        pbr_fbo->check_status();

        pbr_fbo->unbind();
    }

    void OGL_Scene_3D::init_cubemap_fbo()
    {
        cubemap_fbo = FBO_Ptr(new FBO(1024, 1024));
        cubemap_fbo->bind();

        auto cubemap_params = Texture::TexParams::linear_mipmap_clamp_edge();

        auto cubemap_format = Texture::Format(GL_TEXTURE_CUBE_MAP, GL_RGB16F, GL_RGB, GL_FLOAT);
        auto attachment = Texture_Ptr(new Texture(cubemap_format, cubemap_params));
        attachment->bind();
        attachment->resize(1024, 1024);
        cubemap_fbo->attach_texture(attachment);
        attachment->unbind();

        auto depth_attachment_2 = Render_Buffer_Ptr(new Render_Buffer(1024, 1024));
        cubemap_fbo->attach_render_buffer(std::move(depth_attachment_2));

        cubemap_fbo->set_draw_buffers();
        cubemap_fbo->check_status();
        cubemap_fbo->unbind();
    }

    void OGL_Scene_3D::init_irradiance_fbo()
    {
        irradiance_fbo = FBO_Ptr(new FBO(32, 32));
        irradiance_fbo->bind();

        auto cubemap_format = Texture::Format(GL_TEXTURE_CUBE_MAP, GL_RGB16F, GL_RGB, GL_FLOAT);
        auto attachment = Texture_Ptr(new Texture(cubemap_format));
        attachment->bind();
        attachment->resize(32, 32);
        irradiance_fbo->attach_texture(attachment);
        attachment->unbind();

        auto depth_attachment_2 = Render_Buffer_Ptr(new Render_Buffer(32, 32));
        irradiance_fbo->attach_render_buffer(std::move(depth_attachment_2));
        depth_attachment_2->unbind();

        irradiance_fbo->set_draw_buffers();
        irradiance_fbo->check_status();
        irradiance_fbo->unbind();
    }

    void OGL_Scene_3D::init_prefilter_fbo()
    {
        prefilter_fbo = FBO_Ptr(new FBO(128, 128));
        prefilter_fbo->bind();

        auto cubemap_format = Texture::Format(GL_TEXTURE_CUBE_MAP, GL_RGB16F, GL_RGB, GL_FLOAT);
        auto attachment = Texture_Ptr(new Texture(cubemap_format));
        attachment->bind();
        attachment->resize(128, 128);
        prefilter_fbo->attach_texture(attachment);
        attachment->unbind();

        auto depth_attachment_2 = Render_Buffer_Ptr(new Render_Buffer(128, 128));
        prefilter_fbo->attach_render_buffer(std::move(depth_attachment_2));

        prefilter_fbo->set_draw_buffers();
        prefilter_fbo->check_status();
        prefilter_fbo->unbind();
    }

    void OGL_Scene_3D::init_brdf_fbo()
    {
        brdf_fbo = FBO_Ptr(new FBO(512, 512));
        brdf_fbo->bind();

        auto brdf_format = Texture::Format(GL_TEXTURE_2D, GL_RG16F, GL_RG, GL_FLOAT);
        auto attachment = Texture_Ptr(new Texture(brdf_format));
        attachment->bind();
        attachment->resize(512, 512);
        brdf_fbo->attach_texture(attachment);
        attachment->unbind();

        auto depth_attachment_2 = Render_Buffer_Ptr(new Render_Buffer(512, 512));
        brdf_fbo->attach_render_buffer(std::move(depth_attachment_2));

        brdf_fbo->set_draw_buffers();
        brdf_fbo->check_status();
        brdf_fbo->unbind();
    }

    void OGL_Scene_3D::equi_to_cubemap(const Core::Matrix4 &projection)
    {
        auto equi_texture = Texture_Manager::instance().get_texture(this->skybox_path);
        if (!equi_texture)
        {
            return;
        }
        auto equi_to_cube_shader = Rendering::shader_program_factory.find_shader_program("equi_to_cube_shader");
        equi_to_cube_shader->activate();
        equi_to_cube_shader->set_mat4("u_projection", projection.data());
        cubemap_fbo->bind();
        glViewport(0, 0, 1024, 1024);
        glActiveTexture(GL_TEXTURE0);
        equi_texture->bind();
        if (equi_texture->format.is_hdr)
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
            equi_to_cube_shader->set_mat4("u_view", cube_views[i].data());
            // set the render target to be the cube face
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, skybox_texture->texture_id, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Rendering::OGL_Mesh::instanced_cube_mesh()->render(equi_to_cube_shader);
        }
        equi_texture->unbind();
        equi_to_cube_shader->deactivate();
        skybox_texture->generate_mipmap();
        cubemap_fbo->unbind();
    }

    void OGL_Scene_3D::precompute_envrionment()
    {
        Texture *env_cubemap = cubemap_fbo->get_color_attachment(0);
        if (!env_cubemap)
        {
            return;
        }
        compute_env_irradiance(env_cubemap);
        compute_env_prefilter(irradiance_fbo->get_color_attachment(0));
        compute_brdf_lut();
    }

    void OGL_Scene_3D::compute_env_irradiance(Texture *env_map)
    {
        if (!env_map)
        {
            return;
        }

        irradiance_fbo->bind();
        irradiance_fbo->clear(this->bg_color);
        auto irradiance_shader = Rendering::shader_program_factory.find_shader_program("env_irradiance_shader");
        irradiance_shader->activate();
        glActiveTexture(GL_TEXTURE0);
        env_map->bind();
        irradiance_shader->set_int("u_environment_map", 0);
        irradiance_shader->set_mat4("u_projection", cube_projection.data());
        auto irradiance_map = irradiance_fbo->get_color_attachment(0);
        for (int i = 0; i < 6; i++)
        {
            irradiance_shader->set_mat4("u_view", cube_views[i].data());
            // set the render target to be the cube face
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance_map->texture_id, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Rendering::OGL_Mesh::instanced_cube_mesh()->render(irradiance_shader);
        }
        env_map->unbind();
        irradiance_shader->deactivate();
        irradiance_map->generate_mipmap();
        irradiance_fbo->unbind();
    }

    void OGL_Scene_3D::compute_env_prefilter(Texture *env_irradiance_map)
    {
        if (!env_irradiance_map)
        {
            return;
        }

        prefilter_fbo->bind();
        prefilter_fbo->clear(this->bg_color);
        auto prefilter_shader = Rendering::shader_program_factory.find_shader_program("env_prefilter_shader");
        prefilter_shader->activate();
        glActiveTexture(GL_TEXTURE0);
        env_irradiance_map->bind();
        prefilter_shader->set_int("u_environment_map", 0);
        prefilter_shader->set_mat4("u_projection", cube_projection.data());
        const int max_mip_levels = 5;

        for (int mip = 0; mip < max_mip_levels; ++mip)
        {
            // resize framebuffer according to mip-level size.
            unsigned int mip_width = 128 * std::pow(0.5, mip);
            unsigned int mip_height = 128 * std::pow(0.5, mip);
            prefilter_fbo->resize(mip_width, mip_height);
            float roughness = (float)mip / (float)(max_mip_levels - 1);
            prefilter_shader->set_float("u_roughness", roughness);
            auto prefilter_map = prefilter_fbo->get_color_attachment(0);
            for (int i = 0; i < 6; i++)
            {
                prefilter_shader->set_mat4("u_view", cube_views[i].data());
                // set the render target to be the cube face
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilter_map->texture_id, mip);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Rendering::OGL_Mesh::instanced_cube_mesh()->render(prefilter_shader);
            }
        }
        env_irradiance_map->unbind();
        prefilter_shader->deactivate();
        prefilter_fbo->unbind();
    }

    void OGL_Scene_3D::compute_brdf_lut()
    {
        brdf_fbo->bind();
        brdf_fbo->clear(this->bg_color);
        auto brdf_shader = Rendering::shader_program_factory.find_shader_program("env_brdf_shader");
        brdf_shader->activate();
        Rendering::OGL_Mesh::instanced_quad_mesh()->render(brdf_shader);
        brdf_shader->deactivate();
        brdf_fbo->unbind();
    }

    void OGL_Scene_3D::update_environment()
    {
        equi_to_cubemap();
        precompute_envrionment();
    }
};

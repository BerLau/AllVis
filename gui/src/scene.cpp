#include "scene.h"
namespace Rendering
{
    void OGL_Scene::init()
    {
        fbo_ptr = FBO_Ptr(new FBO(this->width, this->height));
        fbo_ptr->bind();

        auto color_attachment = Texture_Ptr(new Texture());
        color_attachment->bind();
        color_attachment->resize(this->width, this->height);
        fbo_ptr->attach_texture(color_attachment);
        color_attachment->unbind();

        auto color_attachment_2 = Texture_Ptr(new Texture());
        color_attachment_2->bind();
        color_attachment_2->resize(this->width, this->height);
        fbo_ptr->attach_texture(color_attachment_2);
        color_attachment_2->unbind();

        auto depth_attachment = Render_Buffer_Ptr(new Render_Buffer(this->width, this->height));
        fbo_ptr->attach_render_buffer(std::move(depth_attachment));
        depth_attachment->unbind();
        fbo_ptr->set_draw_buffers();
        fbo_ptr->check_status();
        
        fbo_ptr->unbind();
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
        // set lights
        for (int i = 0; i < 16; i++)
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
        fbo_ptr->unbind();
    }
};

#include "application.h"
#include <stdexcept>
#include <stdio.h>
#include <memory>
#include <chrono>

#include "ui_log.h"

namespace GUI
{
    Application::Application()
    {
    }

    Application::~Application()
    {
        this->destroy();
    }

    void Application::init()
    {
        this->settings.load_from_file(settings.path);
        this->load_layout(this->settings.ini_file);
        Rendering::shader_program_factory.add_shader_from_file("./shaders/blinn-phong.vert", GL_VERTEX_SHADER, "blinn-phong_vertex");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/blinn-phong.frag", GL_FRAGMENT_SHADER, "blinn-phong_fragment");
        Rendering::shader_program_factory.add_shader_program("blinn-phong_shader", "blinn-phong_vertex", "blinn-phong_fragment");

        Rendering::shader_program_factory.add_shader_from_file("./shaders/text.vert", GL_VERTEX_SHADER, "text_vertex_shader");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/text.frag", GL_FRAGMENT_SHADER, "text_fragment_shader");
        Rendering::shader_program_factory.add_shader_program("text_shader", "text_vertex_shader", "text_fragment_shader");

        Rendering::shader_program_factory.add_shader_from_file("./shaders/basic_tex.vert", GL_VERTEX_SHADER, "basic_tex_vertex");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/basic_tex.frag", GL_FRAGMENT_SHADER, "basic_tex_fragment");
        Rendering::shader_program_factory.add_shader_program("basic_tex_shader", "basic_tex_vertex", "basic_tex_fragment");

        Rendering::shader_program_factory.add_shader_from_file("./shaders/pbr.vert", GL_VERTEX_SHADER, "pbr_vertex");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/pbr.frag", GL_FRAGMENT_SHADER, "pbr_fragment");
        Rendering::shader_program_factory.add_shader_program("pbr_shader", "pbr_vertex", "pbr_fragment");

        // Rendering::shader_program_factory.add_shader_from_file("./shaders/pbr.vert", GL_VERTEX_SHADER, "pbr_vertex");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/pbr_ibl.frag", GL_FRAGMENT_SHADER, "pbr_ibl_fragment");
        Rendering::shader_program_factory.add_shader_program("pbr_ibl_shader", "pbr_vertex", "pbr_ibl_fragment");

        Rendering::shader_program_factory.add_shader_from_file("./shaders/vis_light.vert", GL_VERTEX_SHADER, "light_vertex");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/vis_light.frag", GL_FRAGMENT_SHADER, "light_fragment");
        Rendering::shader_program_factory.add_shader_program("light_shader", "light_vertex", "light_fragment");

        Rendering::shader_program_factory.add_shader_from_file("./shaders/skybox.vert", GL_VERTEX_SHADER, "skybox_vertex");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/skybox.frag", GL_FRAGMENT_SHADER, "skybox_fragment");
        Rendering::shader_program_factory.add_shader_program("skybox_shader", "skybox_vertex", "skybox_fragment");

        // add tone mapping shader
        Rendering::shader_program_factory.add_shader_from_file("./shaders/tone_mapping.vert", GL_VERTEX_SHADER, "tone_mapping_vertex");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/tone_mapping.frag", GL_FRAGMENT_SHADER, "tone_mapping_fragment");
        Rendering::shader_program_factory.add_shader_program("tone_mapping_shader", "tone_mapping_vertex", "tone_mapping_fragment");

        // add equi to cube shader
        Rendering::shader_program_factory.add_shader_from_file("./shaders/equi_to_cube.vert", GL_VERTEX_SHADER, "equi_to_cube_vertex");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/equi_to_cube.frag", GL_FRAGMENT_SHADER, "equi_to_cube_fragment");
        Rendering::shader_program_factory.add_shader_program("equi_to_cube_shader", "equi_to_cube_vertex", "equi_to_cube_fragment");

        // add irradiance_shader
        Rendering::shader_program_factory.add_shader_from_file("./shaders/env_irradiance.vert", GL_VERTEX_SHADER, "env_irradiance_vertex");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/env_irradiance.frag", GL_FRAGMENT_SHADER, "env_irradiance_fragment");
        Rendering::shader_program_factory.add_shader_program("env_irradiance_shader", "env_irradiance_vertex", "env_irradiance_fragment");

        // add prefilter_shader
        Rendering::shader_program_factory.add_shader_from_file("./shaders/env_prefilter.vert", GL_VERTEX_SHADER, "env_prefilter_vertex");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/env_prefilter.frag", GL_FRAGMENT_SHADER, "env_prefilter_fragment");
        Rendering::shader_program_factory.add_shader_program("env_prefilter_shader", "env_prefilter_vertex", "env_prefilter_fragment");

        // add brdf_shader
        Rendering::shader_program_factory.add_shader_from_file("./shaders/env_brdf.vert", GL_VERTEX_SHADER, "env_brdf_vertex");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/env_brdf.frag", GL_FRAGMENT_SHADER, "env_brdf_fragment");
        Rendering::shader_program_factory.add_shader_program("env_brdf_shader", "env_brdf_vertex", "env_brdf_fragment");

        auto w = std::unique_ptr<Sample_OGL_Widget>(new Sample_OGL_Widget("OpenGL Window", 0, 0, 800, 600, true));
        this->ogl_widget_test = std::move(w);
        this->log_widget = std::unique_ptr<Log_Widget>(new Log_Widget("Log", 0, 0, 800, 600, true));
        this->settings_widget = std::unique_ptr<UI_Settings_Widget>(new UI_Settings_Widget("Settings", 0, 0, 800, 600, true));

        // this->text_widget = std::unique_ptr<Text_Widget>(new Text_Widget("Text", 0, 0, 800, 600, true));
        // dynamic_cast<Text_Widget *>(this->text_widget.get())->text = "Hello World";
        // text_widget->background_color[0] = 1.0;
        // text_widget->background_color[1] = 1.0;
        // text_widget->background_color[2] = 1.0;
        settings_widget->bind_settings(&this->settings);

        this->scene_widget = std::unique_ptr<Scene_Widget>(new Scene_Widget("Scene", 0, 0, 800, 600, true));
        this->scene_widget->bind_scene(ogl_widget_test->scene.get());

        this->properties_Widget = std::unique_ptr<Properties_Widget>(new Properties_Widget("Properties", 0, 0, 800, 600, true));
    }

    void Application::run(bool maximized)
    {

        // ImGui::ShowDemoWindow();
        settings_widget->show();
        if (settings.show_OpenGL_window)
        {
            ogl_widget_test->show();
            scene_widget->show();
            if (settings.show_Properties_window)
            {
                properties_Widget->bind_object(scene_widget->selected_object);
                properties_Widget->show();
            }
        }

        if (settings.show_Log_window)
        {
            log_widget->show();
        }
    }

    void Application::destroy()
    {
    }

    void Application::load_layout(const std::string &filename)
    {
        ImGui::LoadIniSettingsFromDisk(filename.c_str());
    }

} // namespace GUI

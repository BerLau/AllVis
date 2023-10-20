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
        Rendering::shader_program_factory.add_shader_from_file("./shaders/basic.vert", GL_VERTEX_SHADER, "basic_vertex");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/basic.frag", GL_FRAGMENT_SHADER, "basic_fragment");
        Rendering::shader_program_factory.add_shader_program("basic_shader", "basic_vertex", "basic_fragment");

        Rendering::shader_program_factory.add_shader_from_file("./shaders/text.vert", GL_VERTEX_SHADER, "text_vertex_shader");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/text.frag", GL_FRAGMENT_SHADER, "text_fragment_shader");
        Rendering::shader_program_factory.add_shader_program("text_shader", "text_vertex_shader", "text_fragment_shader");

        auto w = std::unique_ptr<Sample_OGL_Widget>(new Sample_OGL_Widget("OpenGL Window", 0, 0, 800, 600, true));
        w->scene->set_shader(Rendering::shader_program_factory.find_shader_program("basic_shader"));
        Rendering::Texture *tex = Rendering::load_texture("./textures/box.jpeg");
        // Rendering::sampler_manager.add_sampler("default", new Rendering::Sampler());
        Rendering::Sampler_Manager::instance().add_sampler("basic_sampler", new Rendering::Sampler());
        tex->set_sampler(Rendering::Sampler_Manager::instance().get_sampler("basic_sampler"));
        // check if the texture is loaded
        if (tex == nullptr)
        {
            throw std::runtime_error("Failed to load texture");
        }
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

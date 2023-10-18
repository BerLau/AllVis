#include "imgui_window.h"
#include <stdexcept>
#include <stdio.h>
#include <memory>
#include <chrono>

#include "ui_log.h"

namespace GUI
{

    void IMG_Window::callback(KEY_EVENT event)
    {
        Log::get().info("Key Pressed: " + std::string(event.name));
    }

    void IMG_Window::callback(MOUSE_EVENT event)
    {
        Log::get().info("Mouse Event: " + std::to_string(event.x_pos) + ", " + std::to_string(event.y_pos));
    }

    void IMG_Window::callback(RESIZE_EVENT event)
    {
        this->width = event.width;
        this->height = event.height;
    }

    void IMG_Window::callback(REPOSITION_EVENT event)
    {
        this->x_pos = event.x_pos;
        this->y_pos = event.y_pos;
    }

    IMG_Window::IMG_Window(int width, int height, int x, int y_pos)
    {
        this->width = width;
        this->height = height;
        this->x_pos = x;
        this->y_pos = y_pos;
        this->register_resize_watcher(this);
        this->register_reposition_watcher(this);
    }

    IMG_Window::~IMG_Window()
    {
        this->destroy();
    }

    void IMG_Window::init()
    {
        this->settings.load_from_file();
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

    void IMG_Window::show(bool maximized)
    {

        // ImGui::ShowDemoWindow();
        // get mouse position
        auto mouse_pos = ImGui::GetMousePos();
        ImGui::Begin("Information");
        ImGui::Text("Window Size: (%.1f, %.1f)", this->width, this->height);
        ImGui::Text("Window Position: (%.1f, %.1f)", this->x_pos, this->y_pos);
        ImGui::Text("Mouse Position: (%.1f, %.1f)", mouse_pos.x, mouse_pos.y);
        ImGui::End();
        settings_widget->show();
        if (settings.show_OpenGL_window)
        {
            auto scene = ogl_widget_test->scene.get();
            ogl_widget_test->show();
            scene_widget->bind_scene(scene);
            scene_widget->show();
            if (settings.show_Properties_window)
            {
                properties_Widget->bind_object(scene_widget->selected_object);
                properties_Widget->show();
            }
        }

        if (settings.show_Text_window)
        {
            static float timer_gap = 10;
            static int random_cap = 100;
            ImGui::Begin("Timer Settings");
            ImGui::SliderFloat("Timer Gap", &timer_gap, 0.1, 10, "%.1f", 0.1);
            ImGui::SliderInt("Random Cap", &random_cap, 10, 1000000);
            ImGui::End();
            // get the current time
            static auto prev_time = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::now();
            // get the time difference
            auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(time - prev_time);
            // if the time difference is greater than the timer gap
            if (time_diff.count() > timer_gap * 1000)
            {
                // reset the timer
                prev_time = time;
                // generate a random number
                int random_number = rand() % random_cap;

                // convert current time to string in the format of "hh:mm:ss"
                // std::time_t curr_time = std::chrono::system_clock::to_time_t(time);

                // auto time_info = localtime(&curr_time);

                // char buffer[80];
                // strftime(buffer, 80, "%H:%M:%S", time_info);

                // update the text
                // dynamic_cast<Text_Widget *>(this->text_widget.get())->text = std::to_string(random_number);
            }

            // text_widget->show();
        }

        if (settings.show_Log_window)
        {
            log_widget->show();
        }
    }

    void IMG_Window::destroy()
    {
    }

    void IMG_Window::resize_event(float width_, float height_)
    {
        // notify all resize watchers
        if (!EQUAL_F(width_, this->width) || !EQUAL_F(height_, this->height))
        {
            // notify all resize watchers
            for (auto watcher : this->resize_watchers)
            {
                watcher->callback({width_, height_});
            }
        }
    }

    void IMG_Window::reposition_event(float x_pos, float y_pos)
    {
        // notify all reposition watchers
        if (x_pos != this->x_pos || y_pos != this->y_pos)
        {
            // notify all resize watchers
            for (auto watcher : this->reposition_watchers)
            {
                watcher->callback({x_pos, y_pos});
            }
        }
    }

    void IMG_Window::register_keyboard_watcher(KeyBoard_Watcher *watcher)
    {
        this->keyboard_watchers.insert(watcher);
    }

    void IMG_Window::register_mouse_watcher(Mouse_Watcher *watcher)
    {
        this->mouse_watchers.insert(watcher);
    }

    void IMG_Window::register_resize_watcher(Resize_Watcher *watcher)
    {

        this->resize_watchers.insert(watcher);
    }

    void IMG_Window::register_reposition_watcher(Reposition_Watcher *watcher)
    {
        this->reposition_watchers.insert(watcher);
    }

    void IMG_Window::load_layout(const std::string &filename)
    {
        ImGui::LoadIniSettingsFromDisk(filename.c_str());
    }

} // namespace GUI

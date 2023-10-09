#include "imgui_window.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <stdexcept>
#include <stdio.h>
#include <memory>
#include <chrono>

#include "ui_log.h"

namespace GUI
{
    Window::Window(int width, int height, int x, int y)
    {
        this->width = width;
        this->height = height;
        this->x = x;
        this->y = y;
        this->handle = NULL;
        this->init();
    }

    Window::~Window()
    {
        this->destroy();
    }

    void Window::init()
    {
        this->init_glfw();
        this->init_glad();
        this->init_opengl();
        this->init_imgui();

        Rendering::shader_program_factory.add_shader_from_file("./shaders/basic.vert", GL_VERTEX_SHADER, "basic_vertex");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/basic.frag", GL_FRAGMENT_SHADER, "basic_fragment");
        Rendering::shader_program_factory.add_shader_program("basic_shader", "basic_vertex", "basic_fragment");

        Rendering::shader_program_factory.add_shader_from_file("./shaders/text.vert", GL_VERTEX_SHADER, "text_vertex_shader");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/text.frag", GL_FRAGMENT_SHADER, "text_fragment_shader");
        Rendering::shader_program_factory.add_shader_program("text_shader", "text_vertex_shader", "text_fragment_shader");

        Rendering::shader_program_factory.add_shader_from_file("./shaders/test_texture.vert", GL_VERTEX_SHADER, "test_texture_vertex");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/test_texture.frag", GL_FRAGMENT_SHADER, "test_texture_fragment");
        Rendering::shader_program_factory.add_shader_program("test_texture_shader", "test_texture_vertex", "test_texture_fragment");


        auto w = std::unique_ptr<Sample_OGL_Widget>(new Sample_OGL_Widget("OpenGL Window", 0, 0, 800, 600, true));
        w->background_color[0] = 0.0f;
        w->background_color[1] = 0.3f;
        w->background_color[2] = 0.7f;
        w->set_shader(Rendering::shader_program_factory.find_shader_program("basic_shader"));
        Rendering::Texture* tex = Rendering::load_texture("./textures/box.jpeg");
        // Rendering::sampler_manager.add_sampler("default", new Rendering::Sampler());
        Rendering::Sampler_Manager::instance().add_sampler("basic_shader", new Rendering::Sampler());
        tex->set_sampler(Rendering::Sampler_Manager::instance().get_sampler("default"));
        // check if the texture is loaded
        if(tex == nullptr)
        {
            throw std::runtime_error("Failed to load texture");
        }
        w->sample_texture = Rendering::Texture_Ptr(tex);
        this->ogl_widget_test = std::move(w);
        this->log_widget = std::unique_ptr<Log_Widget>(new Log_Widget("Log", 0, 0, 800, 600, true));
        this->settings_widget = std::unique_ptr<UI_Settings_Widget>(new UI_Settings_Widget("Settings", 0, 0, 800, 600, true));

        this->text_widget = std::unique_ptr<Text_Widget>(new Text_Widget("Text", 0, 0, 800, 600, true));
        dynamic_cast<Text_Widget *>(this->text_widget.get())->text = "Hello World";
        text_widget->background_color[0] = 1.0;
        text_widget->background_color[1] = 1.0;
        text_widget->background_color[2] = 1.0;
        dynamic_cast<UI_Settings_Widget *>(this->settings_widget.get())->bind_settings(&this->settings);
    }

    void Window::show(bool maximized)
    {
        if (maximized)
        {
            glfwMaximizeWindow(this->handle);
        }

        while (!glfwWindowShouldClose(this->handle))
        {
            glfwPollEvents();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            auto viewport = ImGui::GetMainViewport();
            ImGui::DockSpaceOverViewport(viewport);
            settings_widget->show();
            if (settings.show_OpenGL_window)
            {
                ogl_widget_test->show();
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
                    dynamic_cast<Text_Widget *>(this->text_widget.get())->text = std::to_string(random_number);
                }

                text_widget->show();
            }
            if (settings.show_Log_window)
            {
                log_widget->show();
            }

            ImGui::Render();

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow *backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

            glfwSwapBuffers(this->handle);
        }
    }

    void Window::destroy()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(this->handle);
        glfwTerminate();
    }

    void Window::init_imgui()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        // enable docking
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(this->handle, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
        // load layout
        this->settings.load_from_file();
        this->load_layout(this->settings.ini_file);
    }

    void Window::init_glfw()
    {
        glfwSetErrorCallback(Window::glfw_error_callback);
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        this->handle = glfwCreateWindow(this->width, this->height, "GUI", NULL, NULL);
        if (!this->handle)
        {
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwSetWindowPos(this->handle, this->x, this->y);
        glfwSetWindowUserPointer(this->handle, this);
        glfwSetKeyCallback(this->handle, Window::glfw_key_callback);
        glfwSetWindowSizeCallback(this->handle, Window::glfw_resize_callback);
        glfwSetWindowPosCallback(this->handle, Window::glfw_window_pos_callback);
        glfwMakeContextCurrent(this->handle);
        glfwSwapInterval(1);

        // enable multisampling
        glfwWindowHint(GLFW_SAMPLES, 4);
    }

    void Window::init_glad()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }
    }

    void Window::init_opengl()
    {
        // enable depth testing
        glEnable(GL_DEPTH_TEST);
        // enable multisampling
        glEnable(GL_MULTISAMPLE);
    }

    void Window::load_layout(const std::string &filename)
    {
        ImGui::LoadIniSettingsFromDisk(filename.c_str());
    }

    void Window::glfw_error_callback(int error, const char *description)
    {
        throw std::runtime_error(description);
    }

    void Window::glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void Window::glfw_resize_callback(GLFWwindow *window, int width, int height)
    {
        Window *self = (Window *)glfwGetWindowUserPointer(window);
        self->width = width;
        self->height = height;
    }

    void Window::glfw_window_pos_callback(GLFWwindow *window, int xpos, int ypos)
    {
        Window *self = (Window *)glfwGetWindowUserPointer(window);
        self->x = xpos;
        self->y = ypos;
    }

} // namespace GUI

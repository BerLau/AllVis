#include "imgui_widget.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "ui_log.h"
#include <lmath.h>

namespace GUI
{
    IMG_Widget::IMG_Widget(const std::string &name, float x, float y, float width, float height, bool active)
    {
        this->name = name;
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->active = active;
        init();
    }

    IMG_Widget::~IMG_Widget()
    {
        destroy();
    }

    void IMG_Widget::update()
    {
        // get size
        ImVec2 rect = ImGui::GetContentRegionAvail();
        if (!EQUAL_F(rect.x, this->width) || !EQUAL_F(rect.y, this->height))
        {
            this->resize(rect.x, rect.y);
        }
    }

    OGL_Widget::OGL_Widget(const std::string &name, float x, float y, float width, float height, bool active)
        : IMG_Widget(name, x, y, width, height, active)
    {
        init();
    }

    OGL_Widget::~OGL_Widget()
    {
        destroy();
    }

    void OGL_Widget::resize(float width, float height)
    {
        this->width = width;
        this->height = height;
        this->resize_framebuffer();
    }

    void OGL_Widget::create_framebuffer()
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
            Log::get().error("FRAMEBUFFER:: Framebuffer is not complete!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OGL_Widget::bind_framebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    }

    void OGL_Widget::unbind_framebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OGL_Widget::resize_framebuffer()
    {
        glBindTexture(GL_TEXTURE_2D, this->fb_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OGL_Widget::release_framebuffer()
    {
        glDeleteFramebuffers(1, &this->fbo);
        glDeleteTextures(1, &this->fb_tex);
    }

    void OGL_Widget::show_framebuffer()
    {
        GLuint textureID = this->fb_tex;
        ImGui::Image((void *)(intptr_t)textureID, ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));
    }

    void OGL_Widget::init()
    {
        this->create_framebuffer();
    }

    void OGL_Widget::destroy()
    {
        this->release_framebuffer();
    }

    Sample_OGL_Widget::Sample_OGL_Widget(const std::string &name, float x, float y, float width, float height, bool active) : OGL_Widget(name, x, y, width, height, active), shader(nullptr), cube_model(nullptr)
    {
        camera = Rendering::Camera_Ptr(new Rendering::Camera(Core::Vector3(0.0f, 2.0f, 4.0f)));
        camera->focus_on(Core::Vector3(0.f, 0.0f, 0.0f), Core::Vector3(0.0f, 1.0f, 0.0f));
        init();
    }

    Sample_OGL_Widget::~Sample_OGL_Widget()
    {
        destroy();
    }

    void Sample_OGL_Widget::init()
    {
        cube_model = Rendering::OGL_Model_U_Ptr(new Rendering::Cube_Model());
        cube_model->transform->set_position(Core::Vector3(0.0f, 0.0f, 0.0f));
    }

    void Sample_OGL_Widget::destroy()
    {
    }

    void Sample_OGL_Widget::show()
    {
        ImGui::Begin(name.c_str());
        {
            update();
            this->bind_framebuffer();
            this->render();
            this->unbind_framebuffer();
            this->show_framebuffer();
            ImGui::End();
        }
    }

    void Sample_OGL_Widget::set_shader(Rendering::Shader_Program *shader)
    {
        this->shader = shader;
    }

    void Sample_OGL_Widget::render()
    {
        using namespace Core;
        // temporarily set the light properties
        glViewport(0, 0, width, height);
        glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Core::Matrix4 projection = Geometry::perspective(45.0f, width / height, 0.1f, 100.0f);
        Core::Matrix4 view = Geometry::look_at(Vector3(0.0f, 2.0f, 4.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
        std::cout << view << std::endl;
        view = camera->get_view_matrix();
        std::cout << view << std::endl;
        shader->activate();
        shader->set_mat4("u_view", view.data());
        shader->set_mat4("u_projection", projection.data());
        shader->set_vec3("u_color", color);
        shader->set_vec3("u_light_position", Core::Vector3(2.0f, 3.0f, 3.0f).data());
        shader->set_vec3("u_light_color", Core::Vector3(1.0f, 1.0f, 1.0f).data());
        shader->set_vec3("u_view_position", Vector3(0.0f, 2.0f, 4.0f).data());

        cube_model->bind_shader(shader);
        cube_model->draw();
        cube_model->unbind_shader();
    }

    Log_Widget::Log_Widget(const std::string &name, float x, float y, float width, float height, bool active) : IMG_Widget(name, x, y, width, height, active)
    {
        init();
    }

    Log_Widget::~Log_Widget()
    {
        destroy();
    }

    void Log_Widget::show()
    {
        ImGui::Begin(name.c_str());
        {
            update();
            ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            for (auto msg : Log::get().messages)
            {
                ImGui::TextWrapped("%s", msg.c_str());
            }
            ImGui::EndChild();
            ImGui::End();
        }
    }

    UI_Settings_Widget::UI_Settings_Widget(const std::string &name, float x, float y, float width, float height, bool active) : IMG_Widget(name, x, y, width, height, active)
    {
        init();
    }

    UI_Settings_Widget::~UI_Settings_Widget()
    {
    }

    void UI_Settings::save_to_file()
    {
        // open file
        Log::get().info(std::string("Saving GUI settings from file: ") + path);
        std::fstream file(path, std::ios::out);
        if (!file.is_open())
        {
            return;
        }
        // write data
        // write the validation code
        file << "GUI_SETTINGS" << std::endl;
        // write the data
        file << "ini_file " << ini_file << std::endl;
        file << "show_OpenGL_window " << show_OpenGL_window << std::endl;
        file << "show_Log_window " << show_Log_window << std::endl;
        file << "show_Text_window " << show_Text_window << std::endl;
        file << "show_Transform_window " << show_Transform_window << std::endl;
        file << "clear_color " << clear_color[0] << " " << clear_color[1] << " " << clear_color[2] << " " << clear_color[3] << std::endl;
        // close file
        file.close();
    }

    void UI_Settings::load_from_file()
    {
        // open file
        Log::get().info(std::string("Loading GUI settings from file: ") + path);
        std::fstream file(path, std::ios::in);
        if (!file.is_open())
        {
            return;
        }
        // read data
        std::string line;
        std::string key;
        // read the validation code
        std::getline(file, line);
        if (line != "GUI_SETTINGS")
        {
            return;
        }
        // read the data
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            ss >> key;
            if (key == "ini_file")
            {
                ss >> ini_file;
            }
            else if (key == "show_OpenGL_window")
            {
                ss >> show_OpenGL_window;
            }
            else if (key == "show_Log_window")
            {
                ss >> show_Log_window;
            }
            else if (key == "show_Text_window")
            {
                ss >> show_Text_window;
            }
            else if (key == "show_Transform_window")
            {
                ss >> show_Transform_window;
            }
            else if (key == "clear_color")
            {
                ss >> clear_color[0] >> clear_color[1] >> clear_color[2] >> clear_color[3];
            }
        }
    }

    void UI_Settings_Widget::show()
    {
        ImGui::Begin(name.c_str());
        {
            if (this->settings)
            {
                ImGui::Checkbox("Show OpenGL Window", &this->settings->show_OpenGL_window);
                ImGui::Checkbox("Show Log Window", &this->settings->show_Log_window);
                ImGui::Checkbox("Show Text Window", &this->settings->show_Text_window);
                ImGui::Checkbox("Show Transform Window", &this->settings->show_Transform_window);
                ImGui::ColorEdit3("Clear Color", (float *)&this->settings->clear_color);
                if (ImGui::Button("Save Layout"))
                {
                    // Open Popup Modal to get filename
                    ImGui::OpenPopup("Save Layout");
                }
                if (ImGui::BeginPopupModal("Save Layout", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    static char filename[256] = "";
                    ImGui::InputText("Filename", filename, 256);
                    if (ImGui::Button("Save"))
                    {
                        this->save_layout(filename);
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel"))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::End();
        }
    }

    void UI_Settings_Widget::bind_settings(UI_Settings *settings)
    {
        this->settings = settings;
    }

    void UI_Settings_Widget::save_layout(const std::string &filename)
    {
        // save imgui layout to file for loading directly in future.
        auto ini = ImGui::SaveIniSettingsToMemory();

        std::ofstream file(settings->ini_file, std::ios::out | std::ios::trunc);
        file.write(ini, strlen(ini));
        file.close();
        this->settings->path = filename + std::string(".layout");
        this->settings->ini_file = filename + std::string(".ini");
        this->settings->save_to_file();
    }

    Text_Widget::Text_Widget(const std::string &name, float x, float y, float width, float height, bool active)
        : OGL_Widget(name, x, y, width, height, active),
          text_render(Rendering::Text_Render("./fonts/NotoSansSC-VariableFont_wght.ttf"))

    {
        init();
        create_framebuffer();
    }

    Text_Widget::~Text_Widget()
    {
        destroy();
    }

    void Text_Widget::init()
    {
    }

    void Text_Widget::destroy()
    {
        glDeleteFramebuffers(1, &this->fbo);
        glDeleteTextures(1, &this->fb_tex);
    }

    void Text_Widget::show()
    {
        ImGui::Begin(name.c_str());
        {
            update();
            this->bind_framebuffer();
            glViewport(0, 0, width, height);
            glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
            this->render();
            this->unbind_framebuffer();
            this->show_framebuffer();
            ImGui::End();
        }
    }

    void Text_Widget::render()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // center align
        float scale = 1.0f;
        float text_width = this->get_text_width(this->text, scale);
        float text_height = this->get_text_height(this->text, scale);
        float x = (this->width - text_width) / 2;
        float y = (this->height - text_height) / 2;

        text_render.shader->activate();
        text_render.shader->set_vec3("u_color", color);
        text_render.shader->set_mat4("u_projection", Geometry::orthographic(0.0f, this->width, 0.0f, this->height, -1.0f, 1.0f).data());

        text_render.render_text(this->text, x, y, scale, Core::Vector3(0.0f, 0.0f, 0.0f));
    }

    float Text_Widget::get_text_width(const std::string &text, float scale)
    {
        float width = 0.f;
        for (auto c = text.begin(); c != text.end(); c++)
        {
            auto ch = text_render.characters[*c];
            width += (ch.advance >> 6) * scale;
        }
        return width;
    }

    float Text_Widget::get_text_height(const std::string &text, float scale)
    {
        float height = 0.f;
        for (auto c = text.begin(); c != text.end(); c++)
        {
            auto ch = text_render.characters[*c];
            height = std::max(height, ch.size.y() * scale);
        }
        return height;
    }

    void Transform_Widget::show()
    {

        ImGui::Begin(name.c_str(), &active);
        if (transform != nullptr)
        {
            auto pos = transform->get_position();
            ImGui::Text("Position");
            ImGui::DragFloat3("##position", pos.data(), 0.1f);
            transform->set_position(pos);
            Core::EulerAngle euler_angle = transform->get_orientation_euler_angle();
            ImGui::Text("Rotation(pitch,yaw,roll)");
            ImGui::DragFloat3("##rotation", (float *)&euler_angle, 0.1f, -180, 180);
            transform->set_orientation(euler_angle);
            auto scale = transform->get_scale();
            static bool lock_proportion = false;
            if (lock_proportion)
            {
                ImGui::Text("Scale");
                float x = scale.x();
                ImGui::DragFloat("##scale", &scale.x(), 0.01f, 0.01f, 100.0f);
                float ratio = scale.x() / x;
                scale.y() *= ratio;
                scale.z() *= ratio;
            }
            else
            {
                ImGui::Text("Scale");
                ImGui::DragFloat3("##scale", scale.data(), 0.01f, 0.01f, 100.0f);
            }
            ImGui::SameLine();
            ImGui::Checkbox("Lock Proportion", &lock_proportion);
            transform->set_scale(scale);
        }

        ImGui::End();
    }

}; // namespace GUI
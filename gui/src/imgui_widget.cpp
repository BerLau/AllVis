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
        ImVec2 rect = ImGui::GetContentRegionAvail();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        this->resize(rect.x, rect.y, pos.x, pos.y);
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
    }

    void OGL_Widget::show_framebuffer(GLuint tex, float x, float y, float width, float height)
    {
        ImGui::Image((void *)tex, ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));
    }

    void OGL_Widget::init()
    {
    }

    void OGL_Widget::destroy()
    {
    }

    Sample_OGL_Widget::Sample_OGL_Widget(const std::string &name, float x, float y, float width, float height, bool active) : OGL_Widget(name, x, y, width, height, active)
    {
        Rendering::OGL_Scene_3D::Properties *properties = new Rendering::OGL_Scene_3D::Properties();
        scene = Rendering::OGL_Scene_3D_Ptr(new Rendering::OGL_Scene_3D(width, height, properties));
        init();
    }

    Sample_OGL_Widget::~Sample_OGL_Widget()
    {
        destroy();
    }

    void Sample_OGL_Widget::init()
    {
    }

    void Sample_OGL_Widget::destroy()
    {
    }

    void Sample_OGL_Widget::show()
    {
        ImGui::Begin(name.c_str());
        {
            update();
            scene->resize(width, height);
            this->render();
            this->show_framebuffer(scene->fb_tex, x, y, width, height);
            ImGui::End();
        }
    }

    void Sample_OGL_Widget::render()
    {
        scene->render();
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
        file << "show_Properties_window " << show_Properties_window << std::endl;
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
            else if (key == "show_Properties_window")
            {
                ss >> show_Properties_window;
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
                ImGui::Checkbox("Show Properties Window", &this->settings->show_Properties_window);
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

    // Text_Widget::Text_Widget(const std::string &name, float x, float y, float width, float height, bool active)
    //     : OGL_Widget(name, x, y, width, height, active),
    //       text_render(Rendering::Text_Render("./fonts/NotoSansSC-VariableFont_wght.ttf"))

    // {
    //     init();
    //     create_framebuffer();
    // }

    // Text_Widget::~Text_Widget()
    // {
    //     destroy();
    // }

    // void Text_Widget::init()
    // {
    // }

    // void Text_Widget::destroy()
    // {
    //     glDeleteFramebuffers(1, &this->fbo);
    //     glDeleteTextures(1, &this->fb_tex);
    // }

    // void Text_Widget::show()
    // {
    //     ImGui::Begin(name.c_str());
    //     {
    //         update();
    //         this->bind_framebuffer();
    //         glViewport(0, 0, width, height);
    //         glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
    //         this->render();
    //         this->unbind_framebuffer();
    //         this->show_framebuffer();
    //         ImGui::End();
    //     }
    // }

    // void Text_Widget::render()
    // {
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     glEnable(GL_BLEND);
    //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //     // center align
    //     float scale = 1.0f;
    //     float text_width = this->get_text_width(this->text, scale);
    //     float text_height = this->get_text_height(this->text, scale);
    //     float x = (this->width - text_width) / 2;
    //     float y = (this->height - text_height) / 2;

    //     text_render.shader->activate();
    //     text_render.shader->set_vec3("u_color", color);
    //     text_render.shader->set_mat4("u_projection", Geometry::orthographic(0.0f, this->width, 0.0f, this->height, -1.0f, 1.0f).data());

    //     text_render.render_text(this->text, x, y, scale, Core::Vector3(0.0f, 0.0f, 0.0f));
    // }

    // float Text_Widget::get_text_width(const std::string &text, float scale)
    // {
    //     float width = 0.f;
    //     for (auto c = text.begin(); c != text.end(); c++)
    //     {
    //         auto ch = text_render.characters[*c];
    //         width += (ch.advance >> 6) * scale;
    //     }
    //     return width;
    // }

    // float Text_Widget::get_text_height(const std::string &text, float scale)
    // {
    //     float height = 0.f;
    //     for (auto c = text.begin(); c != text.end(); c++)
    //     {
    //         auto ch = text_render.characters[*c];
    //         height = std::max(height, ch.size.y() * scale);
    //     }
    //     return height;
    // }

    void Properties_Widget::show()
    {
        ImGui::Begin(name.c_str());
        {
            update();
            if (selected_object == nullptr)
            {
                ImGui::End();
                return;
            }
            // show scene configuration if it can be casted to scene
            auto scene = dynamic_cast<Rendering::Scene *>(this->selected_object);
            if (scene)
            {
                show_scene_property(scene);
            }

            // show model configuration if it can be casted to model
            auto model = dynamic_cast<Rendering::Model *>(this->selected_object);
            if (model)
            {
                show_model_property(model);
            }

            // show camera configuration if it can be casted to camera
            auto camera = dynamic_cast<Rendering::Camera *>(this->selected_object);
            if (camera)
            {
                show_camera_property(camera);
            }

            // show light configuration if it can be casted to light
            auto light = dynamic_cast<Rendering::Light *>(this->selected_object);
            if (light)
            {
                show_light_property(light);
            }
            ImGui::End();
        }
    }

    void Properties_Widget::show_model_property(Rendering::Model *model)
    {
        if (model != nullptr)
        {
            ImGui::Text("Model");
            show_transform_property(model->transform.get());
        }

        auto ogl_model = dynamic_cast<Rendering::OGL_Model *>(model);
        if (ogl_model)
        {
            // show material properties
            show_material_property(ogl_model->material.get());
        }
    }

    void Properties_Widget::show_light_property(Rendering::Light *light)
    {
        if (light != nullptr)
        {
            auto &light_prop = light->properties;
            ImGui::Text("Light");
            ImGui::Separator();
            ImGui::Text("Type");
            ImGui::SameLine();
            ImGui::Combo("##type", (int *)&light_prop.type, "Parallel\0Point\0Spot\0");
            ImGui::Text("Color");
            ImGui::ColorEdit3("##color", light_prop.color.data());
            ImGui::Text("Intensity");
            ImGui::SliderFloat("##intensity", &light_prop.intensity, 0.0f, 1.0f, "%.3f");
            show_transform_property(light->transform.get());
        }
    }

    void Properties_Widget::show_camera_property(Rendering::Camera *camera)
    {
        if (camera != nullptr)
        {
            ImGui::Text("Camera");
            ImGui::Separator();
            show_transform_property(camera->transform.get());
            auto &camera_props = camera->properties;
            ImGui::Text("FOV");
            ImGui::DragFloat("##fov", &camera_props.fov, 0.1f, 0.1f, 180.0f);
            ImGui::Text("Near");
            ImGui::DragFloat("##near", &camera_props.near, 0.1f, 0.1f, 100.0f);
            ImGui::Text("Far");
            ImGui::DragFloat("##far", &camera_props.far, 0.1f, 0.1f, 100.0f);
            ImGui::Text("Focus Distance");
            ImGui::DragFloat("##focus_distance", &camera_props.focus_distance, 0.1f, 0.1f, 100.0f);
        }
    }

    void Properties_Widget::show_scene_property(Rendering::Scene *scene)
    {
        if (scene != nullptr)
        {

            auto props = scene->get_properties();
            ImGui::Text("Scene");
            ImGui::Separator();
            ImGui::Text("Rect");
            ImGui::Text("width: %.1f", props->width);
            ImGui::SameLine();
            ImGui::Text("height: %.1f", props->height);
            auto ogl_scene = dynamic_cast<Rendering::OGL_Scene *>(scene);
            if (ogl_scene)
            {
                // show the ogl_scene properties
                auto props = ogl_scene->get_properties();
                ImGui::Text("OGL_Scene");
                ImGui::Separator();
                ImGui::Text("background color");
                ImGui::ColorEdit4("##bg_color", props->bg_color);
                ImGui::Text("fov");
                ImGui::DragFloat("##fov", &props->fov, 0.1f, 0.1f, 180.0f);
                ImGui::Text("near");
                ImGui::DragFloat("##near", &props->near, 0.1f, 0.1f, 100.0f);
                ImGui::Text("far");
                ImGui::DragFloat("##far", &props->far, 0.1f, 0.1f, 100.0f);
            }

            auto ogl_3d = dynamic_cast<Rendering::OGL_Scene_3D *>(scene);
            if (ogl_3d)
            {
                // show the ogl_scene properties
                auto props = ogl_3d->get_properties();
                ImGui::Text("OGL_Scene_3D");
                ImGui::Separator();
                ImGui::Text("gamma");
                ImGui::DragFloat("##gamma", &props->gamma, 0.1f, 0.01f, 10.0f);
                ImGui::Text("exposure");
                ImGui::DragFloat("##exposure", &props->exposure, 0.1f, 0.01f, 10.0f);
            }
        }
    }

    void Properties_Widget::show_material_property(Rendering::Material *material)
    {
        if (material != nullptr)
        {
            ImGui::Text("MAterial");
            ImGui::Separator();
            ImGui::Text("Albedo");
            ImGui::ColorEdit3("##albedo", material->albedo.data());
            ImGui::Text("Metallic");
            ImGui::SliderFloat("##metallic", &material->metallic, 0.0f, 1.0f, "%.3f");
            ImGui::Text("Roughness");
            ImGui::SliderFloat("##roughness", &material->roughness, 0.001f, 1.0f, "%.3f");
            ImGui::Text("AO");
            ImGui::SliderFloat("##ao", &material->ao, 0.001f, 1.0f, "%.3f");
            ImGui::Text("Emissive");
            ImGui::ColorEdit3("##emissive", material->emissive.data());
        }
    }

    void Properties_Widget::show_transform_property(Core::Transform *transform)
    {
        if (transform != nullptr)
        {
            // show Title
            ImGui::Text("Transform");
            // Display a separator
            ImGui::Separator();
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
            ImGui::Checkbox("Lock Proportion", &lock_proportion);
            transform->set_scale(scale);
        }
    }

    void Scene_Widget::show()
    {

        ImGui::Begin(name.c_str());
        {
            update();
            // list all the configurable objects in the scene
            if (scene == nullptr)
            {
                ImGui::End();
                selected_object = nullptr;
                return;
            }
            // list all configurable objects in the scene and get the selected object
            // if none was selected then select the scene
            auto ogl_scene = dynamic_cast<Rendering::OGL_Scene *>(scene);
            if (ImGui::TreeNode("Scene"))
            {
                if (ogl_scene)
                {
                    if (ImGui::TreeNode("Models"))
                    {
                        for (int i = 0; i < ogl_scene->models.size(); i++)
                        {
                            auto &model = ogl_scene->models[i];
                            const std::string &label = model.value->name + "##model" + std::to_string(i);
                            ImGui::Checkbox(("##" + label).c_str(), &model.is_active);
                            ImGui::SameLine();
                            if (ImGui::Selectable(label.c_str(), selected_object == model.value.get()))
                            {
                                selected_object = model.value.get();
                            }
                        }
                        ImGui::TreePop();
                    }
                }
                auto ogl_scene_3d = dynamic_cast<Rendering::OGL_Scene_3D *>(scene);
                if (ogl_scene_3d)
                {

                    if (ImGui::TreeNode("Lights"))
                    {
                        for (int i = 0; i < ogl_scene_3d->lights.size(); i++)
                        {
                            auto &light = ogl_scene_3d->lights[i];
                            const std::string &label = light.value->name + "##light" + std::to_string(i);
                            ImGui::Checkbox(("##" + label).c_str(), &light.is_active);
                            ImGui::SameLine();
                            if (ImGui::Selectable(label.c_str(), selected_object == light.value.get()))
                            {
                                selected_object = light.value.get();
                            }
                        }
                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Cameras"))
                    {
                        for (int i = 0; i < ogl_scene_3d->cameras.size(); i++)
                        {
                            auto &camera = ogl_scene_3d->cameras[i];
                            const std::string &label = camera.value->name + "##camera" + std::to_string(i);
                            ImGui::RadioButton(("##" + label).c_str(), &(ogl_scene_3d->active_camera_index), i);
                            ImGui::SameLine();
                            if (ImGui::Selectable(label.c_str(), selected_object == camera.value.get()))
                            {
                                selected_object = camera.value.get();
                            }
                        }
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
            else
            {
                selected_object = scene;
            }
        }
        ImGui::End();
    }

}; // namespace GUI
#pragma once
#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H

#include <imgui.h>
#include <glad/glad.h>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include "ui_log.h"
#include <map>
#include "shader.h"
#include "text_render.h"
#include "scene.h"
#include "light.h"
#include "transform.h"

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

namespace GUI
{
    class IMG_Widget;
    using IMGWidget_U_Ptr = std::unique_ptr<IMG_Widget>;
    using IMGWidget_S_Ptr = std::shared_ptr<IMG_Widget>;
    using IMGWidget_W_Ptr = std::weak_ptr<IMG_Widget>;
    using IMGWidget_Ptr = IMGWidget_U_Ptr;

    class IMG_Widget : public Core::Configurable
    {
        // structures
    public:
        // attributes
    public:
        float width = 0.f;
        float height = 0.f;
        float pos_x = 0.f;
        float pos_y = 0.f;
        bool active = true;
        bool focused = false;
        // constructors and deconstructor
    public:
        IMG_Widget(const std::string &name = "IMG_Widget", float x = 0, float y = 0, float width = 0, float height = 0, bool active = true);
        virtual ~IMG_Widget();
        // methods
    public:
        virtual void show() = 0;
        virtual void update();
        virtual void init(){};
        virtual void destroy(){};
        virtual void resize(float width, float height)
        {
            this->width = width;
            this->height = height;
        }
        virtual void reposition(float x, float y)
        {
            this->pos_x = x;
            this->pos_y = y;
        }

        Core::Vector2 win_to_ndc(float x, float y);
    };

    class OGL_Widget;
    using OGL_Widget_U_Ptr = std::unique_ptr<OGL_Widget>;
    using OGL_Widget_S_Ptr = std::shared_ptr<OGL_Widget>;
    using OGL_Widget_W_Ptr = std::weak_ptr<OGL_Widget>;
    using OGL_Widget_Ptr = OGL_Widget_U_Ptr;
    class OGL_Widget : public IMG_Widget
    {
        // attributes
    public:
        // constructors and deconstructor
    public:
        OGL_Widget(const std::string &name = "OGL_Widget", float x = 0, float y = 0, float width = 800, float height = 600, bool active = true);
        virtual ~OGL_Widget();
        // methods
    public:
        virtual void init();
        virtual void destroy();
        virtual void show() = 0;
        Core::Vector2 ndc_to_clip(float x, float y);
        void show_framebuffer(GLuint tex, float x, float y, float width, float height);
    };

    class Sample_OGL_Widget;
    using Sample_OGL_Widget_U_Ptr = std::unique_ptr<Sample_OGL_Widget>;
    using Sample_OGL_Widget_S_Ptr = std::shared_ptr<Sample_OGL_Widget>;
    using Sample_OGL_Widget_W_Ptr = std::weak_ptr<Sample_OGL_Widget>;
    using Sample_OGL_Widget_Ptr = Sample_OGL_Widget_U_Ptr;
    class Sample_OGL_Widget : public OGL_Widget
    {
        // inherit from OGL_Widget
    public:
        virtual void show();
        virtual void update() override;
        virtual void init();
        virtual void destroy();
        virtual void resize(float width, float height) override;
        // attributes
    public:
        Rendering::OGL_Scene_3D_Ptr scene;

    public:
        Sample_OGL_Widget(const std::string &name = "OpenGL IMG_Widget", float x = 0, float y = 0, float width = 0, float height = 0, bool active = true);
        ~Sample_OGL_Widget();
        // methods
    public:
        void render();
        void process_input();
        void process_mouse_movement();
    };

    class Log_Widget;
    using Log_Widget_U_Ptr = std::unique_ptr<Log_Widget>;
    using Log_Widget_S_Ptr = std::shared_ptr<Log_Widget>;
    using Log_Widget_W_Ptr = std::weak_ptr<Log_Widget>;
    using Log_Widget_Ptr = Log_Widget_U_Ptr;
    class Log_Widget : public IMG_Widget
    {
        // attributes
    public:
        // constructors and deconstructor
    public:
        Log_Widget(const std::string &name = "Log_Widget", float x = 0, float y = 0, float width = 0, float height = 0, bool active = true);
        ~Log_Widget();
        // methods
    public:
        // void init();
        // void destroy();
        void show();
        // void update();
    };

    class Properties_Widget;
    using Properties_Widget_U_Ptr = std::unique_ptr<Properties_Widget>;
    using Properties_Widget_S_Ptr = std::shared_ptr<Properties_Widget>;
    using Properties_Widget_W_Ptr = std::weak_ptr<Properties_Widget>;
    using Properties_Widget_Ptr = Properties_Widget_U_Ptr;
    class Properties_Widget : public IMG_Widget
    {
        // attributes
    public:
        Core::Configurable *selected_object;
        // constructors and deconstructor
    public:
        Properties_Widget(const std::string &name = "Properties_Widget", float x = 0, float y = 0, float width = 0, float height = 0, bool active = true)
            : IMG_Widget(name, x, y, width, height, active),
              selected_object(nullptr){};
        ~Properties_Widget(){};
        // methods
    public:
        void bind_object(Core::Configurable *object) { selected_object = object; }
        void show();
        // void update();
        void show_model_property(Rendering::Model *model);
        void show_ogl_model_property(Rendering::OGL_Model *model);
        void show_light_property(Rendering::Light *light);
        void show_camera_property(Rendering::Camera *camera);
        void show_scene_property(Rendering::Scene *scene);

    protected:
        void show_material_property(Rendering::Material_PBR *material);
        void show_transform_property(Core::Transform *transform);
    };

    struct UI_Settings
    {
        // attributes
        std::string path = "default.layout";
        std::string ini_file = "default.ini";
        bool show_OpenGL_window = false;
        bool show_Log_window = false;
        bool show_Properties_window = false;

        void save_to_file(const std::string &directory, const std::string &filename);

        void load_from_file(const std::string &path);
    };

    class UI_Settings_Widget;
    using UI_Settings_Widget_U_Ptr = std::unique_ptr<UI_Settings_Widget>;
    using UI_Settings_Widget_S_Ptr = std::shared_ptr<UI_Settings_Widget>;
    using UI_Settings_Widget_W_Ptr = std::weak_ptr<UI_Settings_Widget>;
    using UI_Settings_Widget_Ptr = UI_Settings_Widget_U_Ptr;
    class UI_Settings_Widget : public IMG_Widget
    {
        // attributes
    public:
        UI_Settings *settings;
        // constructors and deconstructor
    public:
        UI_Settings_Widget(const std::string &name = "UI_Settings_Widget", float x = 0, float y = 0, float width = 0, float height = 0, bool active = true);
        ~UI_Settings_Widget();
        // methods
    public:
        void show();
        void bind_settings(UI_Settings *settings);

    private:
        void save_layout(const std::string &directory, const std::string &filename);
    };

    class Scene_Widget;
    using Scene_Widget_U_Ptr = std::unique_ptr<Scene_Widget>;
    using Scene_Widget_S_Ptr = std::shared_ptr<Scene_Widget>;
    using Scene_Widget_W_Ptr = std::weak_ptr<Scene_Widget>;
    using Scene_Widget_Ptr = Scene_Widget_U_Ptr;
    class Scene_Widget : public IMG_Widget
    {
        // inherit from OGL_Widget
    public:
        void show() override;
        // void update() override;
        void init() override;
        // void destroy() override;
        // void resize(float width, float height) override;
        // attributes
    public:
        Rendering::Scene *scene;
        Core::Configurable *selected_object;
        // constructors and deconstructor
    public:
        Scene_Widget(const std::string &name = "Scene_Widget", float x = 0, float y = 0, float width = 0, float height = 0, bool active = true)
            : IMG_Widget(name, x, y, width, height, active),
              scene(nullptr),
              selected_object(nullptr){};
        ~Scene_Widget(){};
        // methods
    public:
        void bind_scene(Rendering::Scene *scene) { this->scene = scene; }
    };

    // class Cubemap_Dialog : public IMG_Widget
    // {
    //     // inherit from OGL_Widget
    // public:
    //     void show() override
    //     {
    //         // open modal dialog
    //         ImGui::OpenPopup(name.c_str());
    //         if (ImGui::BeginPopupModal(name.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
    //         {
    //             // Positive X
    //             ImGui::Text("Positive X");
    //             ImGui::SameLine();
    //             ImGui::TextWrapped("%s", paths[0].c_str());
    //             load_map_button("Positive X", paths[0]);
    //             // Negative X
    //             ImGui::Text("Negative X");
    //             ImGui::SameLine();
    //             ImGui::TextWrapped("%s", paths[1].c_str());
    //             load_map_button("Negative X", paths[1]);
    //             // Positive Y
    //             ImGui::Text("Positive Y");
    //             ImGui::SameLine();
    //             ImGui::TextWrapped("%s", paths[2].c_str());
    //             load_map_button("Positive Y", paths[2]);
    //             // Negative Y
    //             ImGui::Text("Negative Y");
    //             ImGui::SameLine();
    //             ImGui::TextWrapped("%s", paths[3].c_str());
    //             load_map_button("Negative Y", paths[3]);
    //             // Positive Z
    //             ImGui::Text("Positive Z");
    //             ImGui::SameLine();
    //             ImGui::TextWrapped("%s", paths[4].c_str());
    //             load_map_button("Positive Z", paths[4]);
    //             // Negative Z
    //             ImGui::Text("Negative Z");
    //             ImGui::SameLine();
    //             ImGui::TextWrapped("%s", paths[5].c_str());
    //             load_map_button("Negative Z", paths[5]);
    //             // button ok
    //             if (ImGui::Button("OK"))
    //             {

    //                 ImGui::CloseCurrentPopup();
    //             }
    //             // button cancel
    //             if (ImGui::Button("Cancel"))
    //             {
    //                 ImGui::CloseCurrentPopup();
    //             }
    //             ImGui::EndPopup();
    //         }
    //     }
    //     // void update() override;
    //     void init() override {}
    //     // attributes
    // public:
    //     std::string paths[6];
    //     unsigned int state = 0;
    //     // constructors and deconstructor
    // public:
    //     Cubemap_Dialog(const std::string &name = "Cubemap_Dialog", float x = 0, float y = 0, float width = 0, float height = 0, bool active = true)
    //         : IMG_Widget(name, x, y, width, height, active){};
    //     ~Cubemap_Dialog(){};
    //     std::string get_path() const;
    // };

    void load_map_button(const std::string &name, std::string &path);
    void update_material(Rendering::Material_PBR *material);
};
#endif
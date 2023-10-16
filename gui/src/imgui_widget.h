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
#include <core.h>
#include "ui_log.h"
#include <map>
#include "shader.h"
#include "text_render.h"

#include "scene.h"

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

namespace GUI
{
    class IMG_Widget;
    using IMGWidget_U_Ptr = std::unique_ptr<IMG_Widget>;
    using IMGWidget_S_Ptr = std::shared_ptr<IMG_Widget>;
    using IMGWidget_W_Ptr = std::weak_ptr<IMG_Widget>;
    using IMGWidget_Ptr = IMGWidget_U_Ptr;

    class IMG_Widget
    {
        // attributes
    public:
        float x;
        float y;
        float width;
        float height;
        bool active;
        std::string name;
        float background_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};

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
    };

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
        virtual void resize(float width, float height);
        void show_framebuffer(GLuint tex, float x, float y, float width, float height);
    };

    class Sample_OGL_Widget : public OGL_Widget
    {
        // inherit from OGL_Widget
    public:
        virtual void show();
        // virtual void update();
        virtual void init();
        virtual void destroy();
        // attributes
    public:
        Rendering::OGL_Scene_3D_Ptr scene;
    public:
        Sample_OGL_Widget(const std::string &name = "OpenGL IMG_Widget", float x = 0, float y = 0, float width = 0, float height = 0, bool active = true);
        ~Sample_OGL_Widget();
        // methods
    public:
        void render();
    };

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

    class Property_Widget : public IMG_Widget
    {
        // attributes
    public:
        // constructors and deconstructor
    public:
        Property_Widget(const std::string &name = "Property_Widget", float x = 0, float y = 0, float width = 0, float height = 0, bool active = true) : IMG_Widget(name, x, y, width, height, active){};
        ~Property_Widget(){};
        // methods
    public:
        void show(){};
        // void update();
        void show_model_property(Rendering::Model *model);
        void show_ogl_model_property(Rendering::OGL_Model *model);
        void show_light_property(Rendering::Light *light);
        void show_camera_property(Rendering::Camera *camera);

    protected:
        void show_material_property(Rendering::Material *material);
        void show_transform_property(Core::Transform *transform);
    };

    struct UI_Settings
    {
        // attributes
        std::string path = "default.layout";
        std::string ini_file = "default.ini";
        bool show_OpenGL_window = false;
        bool show_Log_window = false;
        bool show_Text_window = true;
        bool show_Properties_window = false;
        float clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};

        void save_to_file();

        void load_from_file();
    };

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
        void save_layout(const std::string &filename);
    };

    // class Text_Widget : public OGL_Widget
    // {
    //     // attributes
    // public:
    //     std::string text;
    //     float color[3] = {0.3f, 0.0f, 0.7f};
    //     Rendering::Text_Render text_render;
    //     // constructors and deconstructor
    // public:
    //     Text_Widget(const std::string &name = "Text_Widget", float x = 0, float y = 0, float width = 0, float height = 0, bool active = true);
    //     ~Text_Widget();
    //     // methods
    // public:
    //     void init();
    //     void destroy();
    //     void show();
    //     void render();

    // private:
    //     float get_text_width(const std::string &text, float scale);
    //     float get_text_height(const std::string &text, float scale);
    // };

};
#endif
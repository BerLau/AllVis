#pragma once
#ifndef GUI_APPLICATION_H
#define GUI_APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include "imgui_widget.h"
#include "events.h"

#include <set>

namespace GUI
{
    class Application
    {
        // structures

        // attributes
    public:
        UI_Settings settings;
        Sample_OGL_Widget_Ptr ogl_widget_test;
        Log_Widget_Ptr log_widget;
        UI_Settings_Widget_Ptr settings_widget;
        // IMGWidget_Ptr text_widget;
        Properties_Widget_Ptr properties_Widget;
        Scene_Widget_Ptr scene_widget;

        // constructors and deconstructor
    public:
        Application();
        ~Application();

        // methods
    public:
        void init();
        void run(bool maximized = false);
        void destroy();
        void load_layout(const std::string &filename);
    };
}
#endif
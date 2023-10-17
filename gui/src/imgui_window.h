#pragma once
#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include "imgui_widget.h"

namespace GUI
{
    class Window
    {
        // attributes
    public:
        int width;
        int height;
        int x;
        int y;
        Sample_OGL_Widget_Ptr ogl_widget_test;
        Log_Widget_Ptr log_widget;
        UI_Settings_Widget_Ptr settings_widget;
        // IMGWidget_Ptr text_widget;
        Properties_Widget_Ptr properties_Widget;
        Scene_Widget_Ptr scene_widget;

    private:
        GLFWwindow *handle;
        UI_Settings settings;

        // constructors and deconstructor
    public:
        Window(int width=800, int height=600, int x=0, int y=0);
        ~Window();


        // methods
    public:
        void init();
        void show(bool maximized=false);
        void destroy();

    private:
        void init_imgui();
        void init_glfw();
        void init_glad();
        void init_opengl();

        void load_layout(const std::string &filename);

        // callbacks
    private:
        static void glfw_error_callback(int error, const char *description);
        static void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void glfw_resize_callback(GLFWwindow *window, int width, int height);
        static void glfw_window_pos_callback(GLFWwindow *window, int xpos, int ypos);
    };
}
#endif
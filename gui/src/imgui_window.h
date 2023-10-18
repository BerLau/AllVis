#pragma once
#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include "imgui_widget.h"
#include "events.h"

#include <set>

namespace GUI
{
    class IMG_Window;
    using IMG_Window_U_Ptr = std::unique_ptr<IMG_Window>;
    using IMG_Window_S_Ptr = std::shared_ptr<IMG_Window>;
    using IMG_Window_W_Ptr = std::weak_ptr<IMG_Window>;
    using IMG_Window_Ptr = IMG_Window_U_Ptr;
    class IMG_Window : public KeyBoard_Watcher, public Mouse_Watcher, public Resize_Watcher, public Reposition_Watcher
    {
        // structures
        // interface
    public:
        void callback(KEY_EVENT event) override;
        void callback(MOUSE_EVENT event) override;
        void callback(RESIZE_EVENT event) override;
        void callback(REPOSITION_EVENT event) override;
        // attributes
    public:
        float width;
        float height;
        float x_pos;
        float y_pos;
        UI_Settings settings;
        Sample_OGL_Widget_Ptr ogl_widget_test;
        Log_Widget_Ptr log_widget;
        UI_Settings_Widget_Ptr settings_widget;
        // IMGWidget_Ptr text_widget;
        Properties_Widget_Ptr properties_Widget;
        Scene_Widget_Ptr scene_widget;

        std::set<KeyBoard_Watcher *> keyboard_watchers;
        std::set<Mouse_Watcher *> mouse_watchers;
        std::set<Resize_Watcher *> resize_watchers;
        std::set<Reposition_Watcher *> reposition_watchers;


        // constructors and deconstructor
    public:
        IMG_Window(int width = 800, int height = 600, int x_pos = 0, int y_pos = 0);
        ~IMG_Window();

        // methods
    public:
        void init();
        void show(bool maximized = false);
        void destroy();

        void resize_event(float width, float height);
        void reposition_event(float x_pos, float y_pos);

        void register_keyboard_watcher(KeyBoard_Watcher *watcher);
        void register_mouse_watcher(Mouse_Watcher *watcher);
        void register_resize_watcher(Resize_Watcher *watcher);
        void register_reposition_watcher(Reposition_Watcher *watcher);

        void load_layout(const std::string &filename);
    };
}
#endif
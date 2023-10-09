#pragma once
#ifndef GUI_SCENE_WIDGET_H
#define GUI_SCENE_WIDGET_H
#include "imgui_widget.h"

namespace GUI
{
    class Scene_Widget : public IMG_Widget
    {
        // interface
    public:
        void show();
        void update();
        void init();
        void destroy();

        // attributes
        // public:

        // constructors and deconstructor
    public:
        Scene_Widget(const std::string &name = "Scene_Widget", float x = 0, float y = 0, float width = 0, float height = 0, bool active = true);
        ~Scene_Widget();

        // methods
    private:
        void render();

    public:
        
    };

}

#endif // GUI_SCENE_RENDER_H
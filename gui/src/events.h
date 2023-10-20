#pragma once
#ifndef EVENTS_H
#define EVENTS_H
#include <GLFW/glfw3.h>

namespace GUI
{

    class KeyBoard_Watcher
    {
        // structures
    public:
        struct KEY_EVENT
        {
            int value = 0;
            const char *name = "";
            int scancode = 0;
            int action = 0;
            int mods = 0;
        };

        // attributes
    public:
        // constructors and deconstructor
    public:
        KeyBoard_Watcher(){};
        virtual ~KeyBoard_Watcher(){};
        // methods
    public:
        virtual void callback(KEY_EVENT KEY_EVENT) = 0;
    };

    class Mouse_Click_Watcher
    {
        // structures
    public:
        struct MOUSE_CLICK_EVENT
        {
            float x_pos;
            float y_pos;
            int button;
            int action;
            int mods;
        };

        // attributes
    public:
        // constructors and deconstructor
    public:
        Mouse_Click_Watcher(){};
        virtual ~Mouse_Click_Watcher(){};
        // methods
    public:
        virtual void callback(MOUSE_CLICK_EVENT event) = 0;
    };

    class Mouse_Move_Watcher
    {
        // structures
    public:
        struct MOUSE_MOVE_EVENT
        {
            float x_pos;
            float y_pos;
        };

        // attributes
    public:
        // constructors and deconstructor
    public:
        Mouse_Move_Watcher(){};
        virtual ~Mouse_Move_Watcher(){};
        // methods
    public:
        virtual void callback(MOUSE_MOVE_EVENT event) = 0;
    };

    class Scroll_Watcher
    {
        // structures
    public:
        struct SCROLL_EVENT
        {
            float x_offset;
            float y_offset;
        };

        // attributes
    public:
        // constructors and deconstructor
    public:
        Scroll_Watcher(){};
        virtual ~Scroll_Watcher(){};
        // methods
    public:
        virtual void callback(SCROLL_EVENT event) = 0;
    };

    class Resize_Watcher
    {
        // structures
    public:
        struct RESIZE_EVENT
        {
            float width;
            float height;
        };

        // attributes
    public:
        // constructors and deconstructor
    public:
        Resize_Watcher(){};
        virtual ~Resize_Watcher(){};
        // methods
    public:
        virtual void callback(RESIZE_EVENT event) = 0;
    };

    class Reposition_Watcher
    {
        // structures
    public:
        struct REPOSITION_EVENT
        {
            float x_pos;
            float y_pos;
        };

        // attributes
    public:
        // constructors and deconstructor
    public:
        Reposition_Watcher(){};
        virtual ~Reposition_Watcher(){};
        // methods
    public:
        virtual void callback(REPOSITION_EVENT event) = 0;
    };

    class Event_Dispatcher
    {
        // structures
    public:

        // attributes
    public:
        // constructors and deconstructor
    public:
        Event_Dispatcher(){};
        virtual ~Event_Dispatcher(){};
        // methods
    public:
        virtual void capture_events(GLFWwindow *glfw_window);
    };



}; // namespace GUI

#endif // !EVENTS_H
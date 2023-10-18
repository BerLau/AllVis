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
            unsigned int value = 0;
            const char *name;
            unsigned int scancode;
            unsigned int action;
            unsigned int mods;
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

    class Mouse_Watcher
    {
        // structures
    public:
        struct MOUSE_EVENT
        {
            double x_pos;
            double y_pos;
            unsigned int action;
            unsigned int mods;
        };

        // attributes
    public:
        // constructors and deconstructor
    public:
        Mouse_Watcher(){};
        virtual ~Mouse_Watcher(){};
        // methods
    public:
        virtual void callback(MOUSE_EVENT event) = 0;
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

}; // namespace GUI

#endif // !EVENTS_H
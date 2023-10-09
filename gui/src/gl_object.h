#pragma once
#ifndef GUI_GL_OBJECT_H
#define GUI_GL_OBJECT_H

#include <glad/glad.h>
#include <string>
#include <vector>
#include <memory>

namespace GUI
{
    class GLObject;
    using GLObject_U_Ptr = std::unique_ptr<GLObject>;
    using GLObject_S_Ptr = std::shared_ptr<GLObject>;
    using GLObject_W_Ptr = std::weak_ptr<GLObject>;
    using GLObject_Ptr = GLObject_U_Ptr;

    class GLObject
    {
        // attributes
    public:
        GLuint VAO, VBO, shader;
        GLuint FBO, texture;
        float color[3] = {0.3f, 0.0f, 0.7f};

        const char *vertexShaderSource = "\n#version 330 core"
                                         "\nlayout (location = 0) in vec3 aPos;"
                                         "\nvoid main()"
                                         "\n{"
                                         "\n   gl_Position = vec4(aPos, 1.0);"
                                         "\n}";
        const char *fragmentShaderSource = "\n#version 330 core"
                                           "\nout vec4 FragColor;"
                                           "\nvoid main()"
                                           "\n{"
                                           "\n   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
                                           "\n}";

        // constructors and deconstructor
    public:
        GLObject();
        virtual ~GLObject();
        // methods
    public:
        virtual void show() = 0;
        virtual void update() = 0;
        virtual void init(){};
        virtual void destroy(){};
    };
};

#endif // !GUI_GL_OBJECT_H
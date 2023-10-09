#pragma once
#ifndef GUI_TEXT_RENDER_H
#define GUI_TEXT_RENDER_H

#include <glad/glad.h>
#include <core.h>
#include "shader.h"
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

namespace Rendering
{

    class Text_Render
    {
    public:
        struct Character
        {
            GLuint textureID;
            Core::Vector2 size;
            Core::Vector2 bearing;
            GLuint advance;
        };
        // attributes
    public:
        Shader_Program *shader;
        std::map<GLchar, Character> characters;
        std::string font_path;
        GLuint VAO, VBO;
        float font_size = 48.0f;
        // constructors and deconstructor
    public:
        Text_Render(const std::string& font_path, float font_size = 48.0f)
        {
            this->font_path = font_path;
            this->font_size = font_size;
            init();
        }
        ~Text_Render() {
            destroy();
        }
        // methods
    public:
        void init();
        void render_text(const std::string &content, float x, float y, float scale, Core::Vector3 color);
        void update_font_type(const std::string &font_path);
        void update_font_size(float font_size);
        void destroy();
        void load_font();
    };
};

#endif // !GUI_TEXT_RENDER_H
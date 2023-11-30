#include "text_render.h"
#include "ui_log.h"
#include <iostream>
#include "math/base.h"
namespace Rendering
{
    void Text_Render::init()
    {
        Rendering::shader_program_factory.add_shader_from_file("./shaders/text.vert", GL_VERTEX_SHADER, "text_vertex_shader");
        Rendering::shader_program_factory.add_shader_from_file("./shaders/text.frag", GL_FRAGMENT_SHADER, "text_fragment_shader");
        Rendering::shader_program_factory.add_shader_program("text_shader", "text_vertex_shader", "text_fragment_shader");
        shader = Rendering::shader_program_factory.find_shader_program("text_shader");
        load_font();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
        
    }

    void Text_Render::render_text(const std::string &content, float x, float y, float scale, Core::Vector3 color)
    {
        shader->activate();
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        for (auto c : content)
        {
            Character ch = characters[c];

            float xpos = x + ch.bearing.x() * scale;
            float ypos = y - (ch.size.y() - ch.bearing.y()) * scale;

            float w = ch.size.x() * scale;
            float h = ch.size.y() * scale;

            float vertices[6][4] = {
                {xpos, ypos + h, 0.0f, 0.0f},
                {xpos, ypos, 0.0f, 1.0f},
                {xpos + w, ypos, 1.0f, 1.0f},

                {xpos, ypos + h, 0.0f, 0.0f},
                {xpos + w, ypos, 1.0f, 1.0f},
                {xpos + w, ypos + h, 1.0f, 0.0f}};

            glBindTexture(GL_TEXTURE_2D, ch.textureID);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.advance >> 6) * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        shader->deactivate();
    }

    void Text_Render::update_font_type(const std::string &font_path)
    {
        if (this->font_path != font_path)
        {
            this->font_path = font_path;
            load_font();
        }
    }

    void Text_Render::update_font_size(float font_size)
    {
        if (!Core::Math::equal(this->font_size, font_size))
        {
            this->font_size = font_size;
            load_font();
        }
    }

    void Text_Render::destroy()
    {
        if (VAO != 0)
        {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }
    }

    void Text_Render::load_font()
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            GUI::Log::get().error("Failed to initialize FreeType Library");
        }

        FT_Face face;
        if (FT_New_Face(ft, font_path.c_str(), 0, &face))
        {
            GUI::Log::get().error("Failed to load font");
        }

        FT_Set_Pixel_Sizes(face, 0, font_size);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (GLubyte c = 0; c < 128; ++c)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                GUI::Log::get().error("Failed to load Glyph");
                continue;
            }

            GLuint font_texture;
            glGenTextures(1, &font_texture);
            glBindTexture(GL_TEXTURE_2D, font_texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST

            Character character = {
                font_texture,
                Core::Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                Core::Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x};

            characters[c] = character;
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

} // namespace Rendering
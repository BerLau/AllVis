#pragma once
#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include <glad/glad.h>
#include <memory>
#include <core.h>
#include <unordered_map>
#include "ui_log.h"

namespace Rendering
{
    class Texture;
    using Texture_S_Ptr = std::shared_ptr<Texture>;
    using Texture_U_Ptr = std::unique_ptr<Texture>;
    using Texture_W_Ptr = std::weak_ptr<Texture>;
    using Texture_Ptr = Texture_S_Ptr;

    class Texture
    {
    public:
        struct TexParams
        {
            GLenum min_filter;
            GLenum mag_filter;
            GLenum wrap_s;
            GLenum wrap_t;
            GLenum wrap_r;
            float border_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
            TexParams(GLenum min_filter = GL_LINEAR, GLenum mag_filter = GL_LINEAR, GLenum wrap_s = GL_CLAMP_TO_EDGE, GLenum wrap_t = GL_CLAMP_TO_EDGE, GLenum wrap_r = GL_CLAMP_TO_EDGE)
                : min_filter(min_filter),
                  mag_filter(mag_filter),
                  wrap_s(wrap_s),
                  wrap_t(wrap_t),
                  wrap_r(wrap_r)
            {
            }

            static TexParams linear_clamp_edge()
            {
                static TexParams rslt(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
                return rslt;
            }

            static TexParams linear_repeat()
            {
                static TexParams rslt(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT);
                return rslt;
            }

            static TexParams linear_clamp_border(float r, float g, float b, float a)
            {
                static TexParams rslt(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
                rslt.border_color[0] = r;
                rslt.border_color[1] = g;
                rslt.border_color[2] = b;
                rslt.border_color[3] = a;
                return rslt;
            }

            static TexParams nearest_clamp_edge()
            {
                static TexParams rslt(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
                return rslt;
            }

            static TexParams nearest_repeat()
            {
                static TexParams rslt(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT);
                return rslt;
            }

            static TexParams nearest_clamp_border(float r, float g, float b, float a)
            {
                static TexParams rslt(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
                rslt.border_color[0] = r;
                rslt.border_color[1] = g;
                rslt.border_color[2] = b;
                rslt.border_color[3] = a;
                return rslt;
            }

            static TexParams nearest_mipmap_clamp_edge()
            {
                static TexParams rslt(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
                return rslt;
            }

            static TexParams nearest_mipmap_repeat()
            {
                static TexParams rslt(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT);
                return rslt;
            }

            static TexParams nearest_mipmap_clamp_border(float r, float g, float b, float a)
            {
                static TexParams rslt(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
                rslt.border_color[0] = r;
                rslt.border_color[1] = g;
                rslt.border_color[2] = b;
                rslt.border_color[3] = a;
                return rslt;
            }

            static TexParams linear_mipmap_clamp_edge()
            {
                static TexParams rslt(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
                return rslt;
            }

            static TexParams linear_mipmap_repeat()
            {
                static TexParams rslt(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT);
                return rslt;
            }

            static TexParams linear_mipmap_clamp_border(float r, float g, float b, float a)
            {
                static TexParams rslt(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
                rslt.border_color[0] = r;
                rslt.border_color[1] = g;
                rslt.border_color[2] = b;
                rslt.border_color[3] = a;
                return rslt;
            }

            static TexParams nearest_mipmap_nearest_clamp_edge()
            {
                static TexParams rslt(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
                return rslt;
            }

            static TexParams nearest_mipmap_nearest_repeat()
            {
                static TexParams rslt(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT);
                return rslt;
            }

            static TexParams nearest_mipmap_nearest_clamp_border(float r, float g, float b, float a)
            {
                static TexParams rslt(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
                rslt.border_color[0] = r;
                rslt.border_color[1] = g;
                rslt.border_color[2] = b;
                rslt.border_color[3] = a;
                return rslt;
            }
        };

        struct Format
        {
            GLenum internal_format;
            GLenum format;
            GLenum type;
            GLenum target;
            bool is_hdr = false;
            constexpr Format(GLenum target = GL_TEXTURE_2D, GLenum internal_format = GL_RGBA, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE, bool is_hdr = false)
                : internal_format(internal_format),
                  format(format),
                  type(type),
                  target(target),
                  is_hdr(is_hdr)
            {
            }
        };

        // attributes
    public:
        GLuint texture_id = 0;
        Format format;
        TexParams params;
        void *data = nullptr;
        // constructors and deconstructor
    public:
        Texture(Format format = Format(), TexParams params = TexParams());
        ~Texture();
        // methods
    public:
        void bind() const;
        void bind(int index) const;
        void unbind() const;
        void set_data(const void *data, size_t width, size_t height);
        void set_tex_params(const TexParams &params);
        void resize(size_t width, size_t height);
        void generate_mipmap();
    };

    Texture *load_texture(const std::string &path);
    Texture *load_cube_texture(const std::string &path);

    class Texture_Manager
    {
        // attributes
    private:
        std::unordered_map<std::string, Texture_Ptr> textures;
        // constructors
    public:
        ~Texture_Manager(){};

    private:
        Texture_Manager() {}
        // methods
    public:
        Texture *get_texture(const std::string &path)
        {
            if (textures.find(path) != textures.end())
            {
                return textures[path].get();
            }
            return nullptr;
        }
        void add_texture(const std::string &path, Texture *texture)
        {
            if (texture == nullptr)
            {
                GUI::Log::get().error("Texture_Manager: texture is nullptr");
            }
            textures.insert({path, Texture_Ptr(texture)});
            GUI::Log::get().info("Texture_Manager: texture added:" + path);
        }
        void remove_texture(const std::string &path)
        {
            if (textures.find(path) != textures.end())
            {
                textures.erase(path);
            }
        }
        bool has_texture(const std::string &path)
        {
            return textures.find(path) != textures.end();
        }
        void clear()
        {
            textures.clear();
        }
        // static methods
    public:
        static Texture_Manager &instance()
        {
            static Texture_Manager singleton;
            return singleton;
        }
    };
    struct Img_Data
    {
        char *data = nullptr;
        int width = 0;
        int height = 0;
        int channels = 0;
        GLenum format;
        GLenum inner_format;
        GLint type;
        bool is_hdr = false;
        void release()
        {
            delete[] data;
        }
    };

    Img_Data image_data(const std::string &path, bool flip = false);

    Img_Data read_jpg(const std::string &path, bool flip = false);
    Img_Data read_png(const std::string &path, bool flip = false);
    Img_Data read_bmp(const std::string &path, bool flip = false);
    Img_Data read_tga(const std::string &path, bool flip = false);
    Img_Data read_hdr(const std::string &path, bool flip = false);
    Img_Data read_tiff(const std::string &path, bool flip = false);
    // Img_Data read_exr(const std::string &path, bool flip = false);

}

#endif // RENDER_TEXTURE_H
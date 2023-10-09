#pragma once
#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include <glad/glad.h>
#include <memory>
#include <core.h>
#include <unordered_map>

namespace Rendering
{

    class Sampler;
    using Sampler_S_Ptr = std::shared_ptr<Sampler>;
    using Sampler_U_Ptr = std::unique_ptr<Sampler>;
    using Sampler_W_Ptr = std::weak_ptr<Sampler>;
    using Sampler_Ptr = Sampler_U_Ptr;

    class Sampler
    {
    public:
        struct Format
        {
            GLenum min_filter;
            GLenum mag_filter;
            GLenum wrap_s;
            GLenum wrap_t;
            GLenum wrap_r;
            float border_color[4];
        };
        // attributes
    public:
        GLuint sampler_id;
        Format format;
        // constructors and deconstructor
    public:
        Sampler(Format format = {GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, {0.0f, 0.0f, 0.0f, 0.0f}})
        {
            glGenSamplers(1, &sampler_id);
            glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, format.min_filter);
            glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, format.mag_filter);
            glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, format.wrap_s);
            glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, format.wrap_t);
            glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_R, format.wrap_r);
            glSamplerParameterfv(sampler_id, GL_TEXTURE_BORDER_COLOR, format.border_color);
        }
        ~Sampler()
        {
            if (sampler_id)
            {
                glDeleteSamplers(1, &sampler_id);
                sampler_id = 0;
            }
        }
        // methods
    public:
        void bind(GLuint unit = 0) const
        {
            glBindSampler(unit, sampler_id);
        }
        void unbind() const
        {
            glBindSampler(0, 0);
        }
    };

    class Sampler_Manager
    {
        // attributes
    private:
        std::unordered_map<std::string, Sampler_Ptr> samplers;
        // constructors
    private:
        Sampler_Manager()
        {
        }
    public:
        ~Sampler_Manager()
        {
            clear();
        }
        // methods
    public:
        Sampler *get_sampler(const std::string &name)
        {
            if (samplers.find(name) != samplers.end())
            {
                return samplers[name].get();
            }
            return nullptr;
        }
        void add_sampler(const std::string &name, Sampler *sampler)
        {
            if (samplers.find(name) == samplers.end())
            {
                samplers[name] = Sampler_Ptr(sampler);
            }
        }
        void remove_sampler(const std::string &name)
        {
            if (samplers.find(name) != samplers.end())
            {
                samplers.erase(name);
            }
        }
        void clear()
        {
            samplers.clear();
        }
        // static methods
    public:
        static Sampler_Manager& instance()
        {
            static Sampler_Manager instance;
            return instance;
        }
    };

    extern Sampler_Manager sampler_manager;

    class Texture;
    using Texture_S_Ptr = std::shared_ptr<Texture>;
    using Texture_U_Ptr = std::unique_ptr<Texture>;
    using Texture_W_Ptr = std::weak_ptr<Texture>;
    using Texture_Ptr = Texture_S_Ptr;

    class Texture
    {
    public:
        struct Format
        {
            GLenum internal_format;
            GLenum format;
            GLenum type;
            GLenum target;
        };
        // attributes
    public:
        GLuint texture_id;
        Format format;
        Sampler *sampler;
        // constructors and deconstructor
    public:
        Texture(Format format = {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_2D});
        ~Texture();
        // methods
    public:
        void bind(GLuint index = 0) const;
        void unbind() const;
        void set_data(const void *data, size_t width, size_t height);
        void set_sampler(Sampler *sampler);
    };

    Texture *load_texture(const std::string &path);

    class Texture_Manager
    {
        // attributes
    private:
        std::unordered_map<std::string, Texture_Ptr> textures;
        // constructors
    public:
        Texture_Manager();
        ~Texture_Manager();
        // methods
    public:
        Texture_Ptr get_texture(const std::string &name);
        void add_texture(const std::string &name, Texture_Ptr texture);
        void remove_texture(const std::string &name);
        void clear();
    };
}

#endif // RENDER_TEXTURE_H
#pragma once
#ifndef RENDERING_SAMPLER_H
#define RENDERING_SAMPLER_H
#include <glad/glad.h>
#include <memory>
#include <unordered_map>
#include <string>

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
            GLenum min_filter = GL_LINEAR;
            GLenum mag_filter = GL_LINEAR;
            GLenum wrap_s = GL_CLAMP_TO_EDGE;
            GLenum wrap_t = GL_CLAMP_TO_EDGE;
            GLenum wrap_r = GL_CLAMP_TO_EDGE;
            float border_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
            constexpr Format(GLenum min_filter = GL_LINEAR, GLenum mag_filter = GL_LINEAR, GLenum wrap_s = GL_CLAMP_TO_EDGE, GLenum wrap_t = GL_CLAMP_TO_EDGE, GLenum wrap_r = GL_CLAMP_TO_EDGE)
                : min_filter(min_filter),
                  mag_filter(mag_filter),
                  wrap_s(wrap_s),
                  wrap_t(wrap_t),
                  wrap_r(wrap_r)
            {
            }
        };
        // attributes
    public:
        GLuint sampler_id;
        Format format;
        // constructors and deconstructor
    public:
        Sampler(Format format) : format(format)
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
            // default sampler: linear filter, clamp to edge
            Sampler *default_sampler = new Sampler(Sampler::Format());
            samplers["default"] = Sampler_Ptr(default_sampler);

            // shadow sampler: linear filter, clamp to border
            Sampler *linear_clamp_edge = new Sampler(Sampler::Format(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER));

            samplers["linear_clamp_edge"] = Sampler_Ptr(linear_clamp_edge);

            // shadow sampler: linear filter, clamp to border
            Sampler *linear_clamp_border = new Sampler(Sampler::Format(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER));

            samplers["linear_clamp_border"] = Sampler_Ptr(linear_clamp_border);
        }

    public:
        ~Sampler_Manager()
        {
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
        static Sampler_Manager &instance()
        {
            static Sampler_Manager instance;
            return instance;
        }
    };
}

#endif // RENDERING_SAMPLER_H
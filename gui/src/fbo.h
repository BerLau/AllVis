#pragma once
#ifndef RENDERING_FBO_H
#define RENDERING_FBO_H

#include <glad/glad.h>
#include <memory>
#include "texture.h"

namespace Rendering
{
    class Render_Buffer;
    using Render_Buffer_U_Ptr = std::unique_ptr<Render_Buffer>;
    using Render_Buffer_S_Ptr = std::shared_ptr<Render_Buffer>;
    using Render_Buffer_W_Ptr = std::weak_ptr<Render_Buffer>;
    using Render_Buffer_Ptr = Render_Buffer_U_Ptr;

    class Render_Buffer
    {
        // attributes
    public:
        unsigned int id;
        unsigned int width;
        unsigned int height;
        GLenum internal_format;
        GLenum format;
        GLsizei samples;
        // constructors and deconstructor
    public:
        Render_Buffer(unsigned int width, unsigned int height, GLenum internal_format = GL_DEPTH_COMPONENT, GLenum format = GL_DEPTH_COMPONENT, GLsizei samples = 1)
            : id(0),
              width(width),
              height(height),
              internal_format(internal_format),
              format(format),
              samples(samples)
        {
            create();
            bind();
            set_storage(internal_format, format, samples);
            unbind();
        }
        ~Render_Buffer() { destroy(); }

        // methods
    public:
        void create()
        {
            glGenRenderbuffers(1, &this->id);
        }
        void destroy()
        {
            if (this->id)
            {
                glDeleteRenderbuffers(1, &this->id);
            }
        }

        void bind()
        {
            glBindRenderbuffer(GL_RENDERBUFFER, this->id);
        }

        void unbind()
        {
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }

        void resize(unsigned int width, unsigned int height)
        {
            this->width = width;
            this->height = height;
            set_storage(this->internal_format, this->format, this->samples);
        }

        void set_storage(GLenum internal_format, GLenum format, GLsizei samples)
        {
            this->internal_format = internal_format;
            this->format = format;
            this->samples = samples;
            if (samples == 1)
            {
                glRenderbufferStorage(GL_RENDERBUFFER, this->internal_format, this->width, this->height);
            }
            else if (samples > 1)
            {
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, this->internal_format, this->width, this->height);
            }
        }

        void set_storage_multisample(GLsizei samples)
        {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, this->internal_format, this->width, this->height);
        }
    };

    class FBO;
    using FBO_U_Ptr = std::unique_ptr<FBO>;
    using FBO_S_Ptr = std::shared_ptr<FBO>;
    using FBO_W_Ptr = std::weak_ptr<FBO>;
    using FBO_Ptr = FBO_U_Ptr;
    class FBO
    {
        // attributes
    public:
        unsigned int id;
        unsigned int width;
        unsigned int height;
        // Texture_Ptr texture;
        std::vector<Texture_Ptr> attachments;
        Render_Buffer_Ptr rbo;

        // constructors and deconstructor
    public:
        FBO(unsigned int width, unsigned int height)
            : id(0),
              width(width),
              height(height)
        {
            create();
        }
        ~FBO()
        {
            destroy();
        }

        // methods
    public:
        void create();
        void destroy();
        void bind();
        void unbind();
        void resize(unsigned int width, unsigned int height);
        void attach_texture(Texture_Ptr texture);
        void attach_render_buffer(Render_Buffer_Ptr render_buffer);
        bool check_status();
        void set_draw_buffers();
        void clear(Core::Vector4 bg_color = Core::Vector4(1.f, 1.f, 1.f, 1.f));
        Texture* get_color_attachment(unsigned int index = 0);
    };
}; // namespace Rendering

#endif // !RENDERING_FBO_H

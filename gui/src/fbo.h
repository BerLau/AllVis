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
        // constructors and deconstructor
    public:
        Render_Buffer(unsigned int width, unsigned int height, GLenum internal_format = GL_DEPTH_COMPONENT, GLenum format = GL_DEPTH_COMPONENT)
            : id(0),
              width(width),
              height(height),
              internal_format(internal_format),
              format(format)
        {
            create();
        }
        ~Render_Buffer() { destroy(); }

        // methods
    public:
        void create()
        {
            glGenRenderbuffers(1, &this->id);
            glBindRenderbuffer(GL_RENDERBUFFER, this->id);
            glRenderbufferStorage(GL_RENDERBUFFER, this->internal_format, this->width, this->height);
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
            destroy();
            create();
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
        Texture_Ptr texture;
        Render_Buffer_Ptr render_buffer;

        // constructors and deconstructor
    public:
        FBO(unsigned int width, unsigned int height)
            : id(0),
              width(width),
              height(height),
              texture(nullptr),
              render_buffer(nullptr)
        {
            create();
        }
        ~FBO(){
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
    };
} // namespace Rendering

#endif // !RENDERING_FBO_H

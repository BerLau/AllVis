#include "fbo.h"

namespace Rendering
{
    void FBO::destroy()
    {
        if (this->id)
        {
            glDeleteFramebuffers(1, &this->id);
        }
    }

    void FBO::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, this->id);
        if (this->texture)
        {
            this->texture->bind();
        }
        if (this->render_buffer)
        {
            this->render_buffer->bind();
        }
    }

    void FBO::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if (this->texture)
        {
            this->texture->unbind();
        }
        if (this->render_buffer)
        {
            this->render_buffer->unbind();
        }
    }

    void FBO::resize(unsigned int width, unsigned int height)
    {
        if (EQUAL_F(this->width, width) && EQUAL_F(this->height, height))
        {
            return;
        }
        this->width = width;
        this->height = height;
        bind();
        this->texture->bind();
        this->texture->resize(width, height);
        this->render_buffer->bind();
        this->render_buffer->resize(width, height);
        unbind();
        this->texture->unbind();
        this->render_buffer->unbind();
    }

    void FBO::attach_texture(Texture_Ptr texture)
    {
        this->texture = texture;
        bind();
        texture->bind();
        texture->resize(this->width, this->height);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture->texture_id, 0);
        unbind();
        texture->unbind();
    }

    void FBO::attach_render_buffer(Render_Buffer_Ptr render_buffer)
    {
        this->render_buffer = std::move(render_buffer);
        bind();
        this->render_buffer->bind();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->render_buffer->id);
        unbind();
        this->render_buffer->unbind();
    }

    bool FBO::check_status()
    {
        bind();
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        switch (status)
        {
        case GL_FRAMEBUFFER_COMPLETE:
            unbind();
            return true;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
            unbind();
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cout << "[ERROR] Framebuffer incomplete: No image is attached to Framebuffer." << std::endl;
            unbind();
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
            unbind();
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
            unbind();
            return false;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cout << "[ERROR] Unsupported by Framebuffer implementation." << std::endl;
            unbind();
            return false;
        default:
            std::cout << "[ERROR] Unknow error." << std::endl;
            unbind();
            return false;
        }
    }

    void FBO::create()
    {
        glGenFramebuffers(1, &this->id);
        if (this->id == 0)
        {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not created!" << std::endl;
        }
    }

} // namespace Rendering

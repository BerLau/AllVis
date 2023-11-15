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
    }

    void FBO::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FBO::resize(unsigned int width, unsigned int height)
    {
        if (EQUAL_F(this->width, width) && EQUAL_F(this->height, height))
        {
            return;
        }
        this->width = width;
        this->height = height;
        for (auto &attachment : this->attachments)
        {
            attachment->bind();
            attachment->resize(width, height);
            attachment->unbind();
        }
        if (this->rbo)
        {
            this->rbo->bind();
            this->rbo->resize(width, height);
            this->rbo->unbind();
        }
    }

    void FBO::attach_texture(Texture_Ptr texture)
    {
        GLuint index = GL_COLOR_ATTACHMENT0 + this->attachments.size();

        if (texture->format.target == GL_TEXTURE_2D)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, index, GL_TEXTURE_2D, texture->texture_id, 0);
        }
        else if (texture->format.target == GL_TEXTURE_CUBE_MAP)
        {
            for (int i = 0; i < 6; i++)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, index, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture->texture_id, 0);
            }
        }
        this->attachments.push_back(std::move(texture));
    }

    void FBO::attach_render_buffer(Render_Buffer_Ptr render_buffer)
    {
        render_buffer->bind();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer->id);
        render_buffer->unbind();
        this->rbo = std::move(render_buffer);
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

        // check if attachments are complete
        for (int i = 0; i < attachments.size(); ++i)
        {
            int status;
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &status);
            if (status == GL_NONE)
            {
                std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
                unbind();
                return false;
            }
        }
        unbind();
    }

    void FBO::set_draw_buffers()
    {
        std::vector<GLenum> draw_buffers;
        for (int i = 0; i < this->attachments.size(); i++)
        {
            draw_buffers.push_back(GL_COLOR_ATTACHMENT0 + i);
        }
        glDrawBuffers(draw_buffers.size(), draw_buffers.data());
    }

    void FBO::clear(Core::Vector4 bg_color)
    {
        // glClearColor(bg_color.x(), bg_color.y(), bg_color.z(), bg_color.w());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glViewport(0, 0, this->width, this->height);
    }

    Texture *FBO::get_color_attachment(unsigned int index)
    {
        if (index < this->attachments.size())
        {
            return this->attachments[index].get();
        }
        return nullptr;
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

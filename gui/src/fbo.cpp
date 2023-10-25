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
        this->width = width;
        this->height = height;
        destroy();
        create();
        if(this->texture)
        {
            this->texture->resize(width, height);
        }
        if(this->render_buffer)
        {
            this->render_buffer->resize(width, height);
        }
    }

    void FBO::attach_texture(Texture_Ptr texture)
    {
        this->texture = texture;
        bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture->texture_id, 0);
        unbind();
    }

    void FBO::attach_render_buffer(Render_Buffer_Ptr render_buffer)
    {
        this->render_buffer = std::move(render_buffer);
        bind();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->render_buffer->id);
        unbind();
    }

    void FBO::create()
    {
        glGenFramebuffers(1, &this->id);
        glBindFramebuffer(GL_FRAMEBUFFER, this->id);
      
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

} // namespace Rendering

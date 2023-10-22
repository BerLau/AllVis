#include "texture.h"
#include "ui_log.h"

#include <FreeImage.h>

namespace Rendering
{
    Texture::Texture(Format format) : format(format), sampler(nullptr)
    {
        glGenTextures(1, &texture_id);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &texture_id);
        if (sampler)
        {
            glDeleteSamplers(1, &sampler->sampler_id);
            sampler = nullptr;
        }
    }

    void Texture::bind(GLuint index) const
    {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(format.target, texture_id);
        if (sampler)
        {
            sampler->bind(index);
        }
        else
        {
            auto sampler = Sampler_Manager::instance().get_sampler("default");
            sampler->bind(index);
        }
    }

    void Texture::unbind() const
    {
        glBindTexture(format.target, 0);
        if (sampler)
        {
            sampler->unbind();
        }
    }

    void Texture::set_data(const void *data, size_t width, size_t height)
    {
        bind();
        glTexImage2D(format.target, 0, format.internal_format, width, height, 0, format.format, format.type, data);
        unbind();
    }

    void Texture::set_sampler(Sampler *sampler)
    {
        this->sampler = sampler;
    }

    // load texture from file with FreeImage Library
    Texture *load_texture(const std::string &path)
    {
        Texture::Format format_;
        format_.internal_format = GL_RGBA;
        format_.format = GL_BGRA;
        format_.type = GL_UNSIGNED_BYTE;
        format_.target = GL_TEXTURE_2D;

        FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);
        if (format == FIF_UNKNOWN)
        {
            std::cerr << "Unknown file format!" << std::endl;
            return 0;
        }

        FIBITMAP *image = FreeImage_Load(format, path.c_str());
        if (!image)
        {
            std::cerr << "Failed to load image!" << std::endl;
            return 0;
        }

        FIBITMAP *temp = image;
        image = FreeImage_ConvertTo32Bits(image);
        FreeImage_Unload(temp);

        int width = FreeImage_GetWidth(image);
        int height = FreeImage_GetHeight(image);
        BYTE *bits = FreeImage_GetBits(image);

        Texture *texture = new Texture(format_);
        texture->set_data(bits, width, height);

        FreeImage_Unload(image);
        return texture;
    }

} // namespace Rendering

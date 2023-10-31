#include "texture.h"
#include "ui_log.h"

#include <FreeImage.h>

namespace Rendering
{

    Texture::Texture(Format format, TexParams params)
    : format(format), params(params)
    {
        glGenTextures(1, &texture_id);
        set_tex_params(params);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &texture_id);
    }

    void Texture::bind() const
    {
        glBindTexture(format.target, texture_id);
    }

    void Texture::unbind() const
    {
        glBindTexture(format.target, 0);
    }

    void Texture::set_data(const void *data, size_t width, size_t height)
    {
        bind();
        glTexImage2D(format.target, 0, format.internal_format, width, height, 0, format.format, format.type, data);
        glGenerateMipmap(format.target);
        unbind();
    }

    void Texture::set_tex_params(const TexParams &params)
    {
        bind();
        glTexParameteri(format.target, GL_TEXTURE_MIN_FILTER, params.min_filter);
        glTexParameteri(format.target, GL_TEXTURE_MAG_FILTER, params.mag_filter);
        glTexParameteri(format.target, GL_TEXTURE_WRAP_S, params.wrap_s);
        glTexParameteri(format.target, GL_TEXTURE_WRAP_T, params.wrap_t);
        glTexParameteri(format.target, GL_TEXTURE_WRAP_R, params.wrap_r);
        glTexParameterfv(format.target, GL_TEXTURE_BORDER_COLOR, params.border_color);
        unbind();
    }

    void Texture::resize(size_t width, size_t height)
    {
        glTexImage2D(format.target, 0, format.internal_format, width, height, 0, format.format, format.type, NULL);
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
        // texture->set_sampler(Sampler_Manager::instance().get_sampler("default"));

        FreeImage_Unload(image);
        return texture;
    }

    Texture *load_cube_texture(const std::string &path)
    {
        std::vector<std::string> faces = {
            path + "/right.jpg",
            path + "/left.jpg",
            path + "/top.jpg",
            path + "/bottom.jpg",
            path + "/front.jpg",
            path + "/back.jpg",
        };
    }

} // namespace Rendering

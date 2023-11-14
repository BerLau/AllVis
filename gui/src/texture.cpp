#include "texture.h"
#include "ui_log.h"

#include <cstring>

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
        if (format.target == GL_TEXTURE_2D)
        {
            glTexImage2D(format.target, 0, format.internal_format, width, height, 0, format.format, format.type, NULL);
        }
        else if (format.target == GL_TEXTURE_CUBE_MAP)
        {
            for (int i = 0; i < 6; i++)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format.internal_format, width, height, 0, format.format, format.type, NULL);
            }
        }
    }

    // potential bug: the color channels may be vary in different images and os
    bool image_info(FIBITMAP *bitmap, FREE_IMAGE_FORMAT file_type, unsigned int &texture_format, unsigned int &texture_inner_format, int &texture_type, int &channels, bool &is_hdr)
    {
        bool rslt = true;
        unsigned bpp = FreeImage_GetBPP(bitmap);
        FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(bitmap);
        auto color_type = FreeImage_GetColorType(bitmap);
        // four channel images
        if (color_type == FIC_RGBALPHA)
        {
            channels = 4;
            if (file_type == FIF_PNG || file_type == FIF_JPEG)
            {
                texture_format = GL_BGRA;
            }
            else
            {
                texture_format = GL_RGBA;
            }
            // if hdr
            if (imageType == FIT_RGBAF)
            {
                is_hdr = true;
                texture_type = GL_FLOAT;
                if (bpp == 64)
                {
                    texture_inner_format = GL_RGBA16F;
                }
                else if (bpp == 128)
                {
                    texture_inner_format = GL_RGBA32F;
                }
                else
                {
                    rslt = false;
                }
            }
            else
            {
                is_hdr = false;
                if (bpp == 32)
                {
                    texture_type = GL_UNSIGNED_BYTE;
                    texture_inner_format = GL_RGBA8;
                }
                else if (bpp == 64)
                {
                    texture_type = GL_UNSIGNED_SHORT;
                    texture_inner_format = GL_RGBA16;
                }
                else if (bpp == 128)
                {
                    texture_type = GL_UNSIGNED_INT;
                    texture_inner_format = GL_RGBA32F;
                }
                else
                {
                    rslt = false;
                }
            }
        }
        // three channel images
        else if (color_type == FIC_RGB)
        {
            channels = 3;
            if (file_type == FIF_PNG || file_type == FIF_JPEG)
            {
                texture_format = GL_BGR;
            }
            else
            {
                texture_format = GL_RGB;
            }
            // if hdr
            if (imageType == FIT_RGBF)
            {
                is_hdr = true;
                texture_type = GL_FLOAT;
                if (bpp == 48)
                {
                    texture_inner_format = GL_RGB16F;
                }
                else if (bpp == 96)
                {
                    texture_inner_format = GL_RGB32F;
                }
                else
                {
                    rslt = false;
                }
            }
            else
            {
                is_hdr = false;
                if (bpp == 24)
                {
                    texture_type = GL_UNSIGNED_BYTE;
                    texture_inner_format = GL_RGB8;
                }
                else if (bpp == 48)
                {
                    texture_type = GL_UNSIGNED_SHORT;
                    texture_inner_format = GL_RGB16;
                }
                else if (bpp == 96)
                {
                    texture_type = GL_UNSIGNED_INT;
                    texture_inner_format = GL_RGB32F;
                }
                else
                {
                    rslt = false;
                }
            }
        }
        // one channel images
        else if (color_type == FIC_MINISBLACK || color_type == FIC_MINISWHITE)
        {
            channels = 1;
            texture_format = GL_RED;
            // if hdr
            if (imageType == FIT_FLOAT)
            {
                is_hdr = true;
                texture_type = GL_FLOAT;
                if (bpp == 16)
                {
                    texture_inner_format = GL_R16F;
                }
                else if (bpp == 32)
                {
                    texture_inner_format = GL_R32F;
                }
                else
                {
                    rslt = false;
                }
            }
            else
            {
                is_hdr = false;
                if (bpp == 8)
                {
                    texture_type = GL_UNSIGNED_BYTE;
                    texture_inner_format = GL_R8;
                }
                else if (bpp == 16)
                {
                    texture_type = GL_UNSIGNED_SHORT;
                    texture_inner_format = GL_R16;
                }
                else if (bpp == 32)
                {
                    texture_type = GL_UNSIGNED_INT;
                    texture_inner_format = GL_R32F;
                }
                else
                {
                    rslt = false;
                }
            }
        }
        else
        {
            rslt = false;
        }
        return rslt;
    }

    Img_Data image_data(const std::string &path, bool flip)
    {
        static bool freeimage_initialized = false;
        if (!freeimage_initialized)
        {
            FreeImage_Initialise();
            freeimage_initialized = true;
        }
        Img_Data rslt;
        FREE_IMAGE_FORMAT file_type = FreeImage_GetFileType(path.c_str(), 0);
        if (file_type == FIF_UNKNOWN)
        {
            std::cerr << "Unknown file format!" << std::endl;
            return rslt;
        }
        auto *image = FreeImage_Load(file_type, path.c_str());
        if (!image)
        {
            std::cerr << "Failed to load image!" << std::endl;
            return rslt;
        }

        bool identified = image_info(image, file_type, rslt.format, rslt.inner_format, rslt.type, rslt.channels, rslt.is_hdr);
        if (!identified)
        {
            // convert to 24/32 bit based on the channels
            if (rslt.channels == 3)
            {
                image = FreeImage_ConvertTo24Bits(image);
                image_info(image, file_type, rslt.format, rslt.inner_format, rslt.type, rslt.channels, rslt.is_hdr);
            }
            else if (rslt.channels == 4)
            {
                image = FreeImage_ConvertTo32Bits(image);
                image_info(image, file_type, rslt.format, rslt.inner_format, rslt.type, rslt.channels, rslt.is_hdr);
            }
            else
            {
                std::cerr << "Unknown image format!" << std::endl;
                return rslt;
            }
            if (image == nullptr)
            {
                std::cerr << "Failed to convert image!" << std::endl;
                return rslt;
            }
        }

        auto bpp = FreeImage_GetBPP(image);
        rslt.width = FreeImage_GetWidth(image);
        rslt.height = FreeImage_GetHeight(image);
        // rearrange data
        rslt.data = new BYTE[rslt.width * rslt.height * bpp / 8];
        BYTE *bits = FreeImage_GetBits(image);
        FreeImage_ConvertToRawBits(rslt.data, image, rslt.width * bpp / 8, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, flip);
        FreeImage_Unload(image);
        return rslt;
    }

    // load texture from file with FreeImage Library
    Texture *load_texture(const std::string &path)
    {
        Texture::Format format;
        format.target = GL_TEXTURE_2D;
        auto img = image_data(path);
        if (img.data == nullptr)
        {
            return nullptr;
        }
        format.internal_format = img.inner_format;
        format.format = img.format;
        format.type = img.type;
        Texture *texture = new Texture(format);
        texture->set_data(img.data, img.width, img.height);
        img.release();
        return texture;
    }

    Texture *load_cube_texture(const std::string &path)
    {
        Texture::Format format;
        format.target = GL_TEXTURE_CUBE_MAP;

        auto paths = Tools::split_str(path, ";");
        Texture *texture = new Texture(format);
        for (int i = 0; i < 6; i++)
        {
            auto img = image_data(paths[i], true);
            if (img.data == nullptr)
            {
                return nullptr;
            }
            format.internal_format = img.inner_format;
            format.format = img.format;
            format.type = img.type;
            texture->bind();
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format.internal_format, img.width, img.height, 0, format.format, format.type, img.data);
            img.release();
        }
        texture->unbind();
        return texture;
    }

} // namespace Rendering

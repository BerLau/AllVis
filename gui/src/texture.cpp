#include "texture.h"
#include "ui_log.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <tiffio.h>
#include <cstring>
#include <iostream>
#include "file.h"
#include "tools.h"

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

    void Texture::bind(int index) const
    {
        glActiveTexture(GL_TEXTURE0 + index);
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
        glTexParameteri(format.target, GL_TEXTURE_WRAP_S, params.wrap_s);
        glTexParameteri(format.target, GL_TEXTURE_WRAP_T, params.wrap_t);
        glTexParameteri(format.target, GL_TEXTURE_WRAP_R, params.wrap_r);
        glTexParameteri(format.target, GL_TEXTURE_MIN_FILTER, params.min_filter);
        glTexParameteri(format.target, GL_TEXTURE_MAG_FILTER, params.mag_filter);
        if (params.wrap_s == GL_CLAMP_TO_BORDER || params.wrap_t == GL_CLAMP_TO_BORDER || params.wrap_r == GL_CLAMP_TO_BORDER)
        {
            glTexParameterfv(format.target, GL_TEXTURE_BORDER_COLOR, params.border_color);
        }
        unbind();
    }

    void Texture::resize(size_t width, size_t height)
    {
        bind();
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
        unbind();
    }

    void Texture::generate_mipmap()
    {
        bind();
        glGenerateMipmap(format.target);
        unbind();
    }

    void Texture::update_pixels(const void *data, size_t x_offset, size_t y_offset, size_t width, size_t height)
    {
        if (format.target == GL_TEXTURE_2D)
        {
            bind();
            glTexSubImage2D(format.target, 0, x_offset, y_offset, width, height, format.format, format.type, data);
            unbind();
        }
        else
        {
            bind();
            for (int i = 0; i < 6; ++i)
            {
                glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, x_offset, y_offset, width, height, format.format, format.type, data);
            }
            unbind();
        }
    }

    Img_Data image_data(const std::string &path, bool flip)
    {
        Img_Data rslt;
        auto ext = Core::file_extension(path);
        if (ext == ".jpg" || ext == ".jpeg")
        {
            rslt = read_jpg(path, flip);
        }
        else if (ext == ".png")
        {
            rslt = read_png(path, flip);
        }
        else if (ext == ".bmp")
        {
            rslt = read_bmp(path, flip);
        }
        else if (ext == ".tga")
        {
            rslt = read_tga(path, flip);
        }
        else if (ext == ".hdr")
        {
            rslt = read_hdr(path, flip);
        }
        else if (ext == ".tif")
        {
            rslt = read_tiff(path, flip);
        }
        else
        {
            std::cerr << "Unknown image format!" << std::endl;
        }
        return rslt;
    }

    Img_Data read_jpg(const std::string &path, bool flip)
    {
        // read jpg with stb_image
        Img_Data rslt;
        stbi_set_flip_vertically_on_load(flip);
        if (stbi_is_hdr(path.c_str()))
        {
            rslt.data = (char *)stbi_loadf(path.c_str(), &rslt.width, &rslt.height, &rslt.channels, 0);
            rslt.is_hdr = true;
            rslt.type = GL_FLOAT;
            if (rslt.channels == 3)
            {
                rslt.format = GL_RGB;
                rslt.inner_format = GL_RGB16F;
            }
            else if (rslt.channels == 4)
            {
                rslt.format = GL_RGBA;
                rslt.inner_format = GL_RGBA16F;
            }
            else
            {
                std::cerr << "Unknown image format!" << std::endl;
                return rslt;
            }
        }
        else
        {
            rslt.data = (char *)stbi_load(path.c_str(), &rslt.width, &rslt.height, &rslt.channels, 0);
            rslt.is_hdr = false;
            rslt.type = GL_UNSIGNED_BYTE;
            if (rslt.channels == 3)
            {
                rslt.format = GL_RGB;
                rslt.inner_format = GL_RGB8;
            }
            else if (rslt.channels == 4)
            {
                rslt.format = GL_RGBA;
                rslt.inner_format = GL_RGBA8;
            }
            else
            {
                std::cerr << "Unknown image format!" << std::endl;
                return rslt;
            }
        }
        return rslt;
    }

    Img_Data read_png(const std::string &path, bool flip)
    {
        // read png with stb_image
        Img_Data rslt;
        stbi_set_flip_vertically_on_load(flip);
        rslt.data = (char *)stbi_load(path.c_str(), &rslt.width, &rslt.height, &rslt.channels, 0);
        rslt.is_hdr = false;
        rslt.type = GL_UNSIGNED_BYTE;
        if (rslt.channels == 3)
        {
            rslt.format = GL_RGB;
            rslt.inner_format = GL_RGB8;
        }
        else if (rslt.channels == 4)
        {
            rslt.format = GL_RGBA;
            rslt.inner_format = GL_RGBA8;
        }
        else
        {
            std::cerr << "Unknown image format!" << std::endl;
            return rslt;
        }
        return rslt;
    }

    Img_Data read_bmp(const std::string &path, bool flip)
    {
        // read bmp with stb_image
        Img_Data rslt;
        stbi_set_flip_vertically_on_load(flip);
        rslt.data = (char *)stbi_load(path.c_str(), &rslt.width, &rslt.height, &rslt.channels, 0);
        rslt.is_hdr = false;
        rslt.type = GL_UNSIGNED_BYTE;
        if (rslt.channels == 3)
        {
            rslt.format = GL_RGB;
            rslt.inner_format = GL_RGB8;
        }
        else if (rslt.channels == 4)
        {
            rslt.format = GL_RGBA;
            rslt.inner_format = GL_RGBA8;
        }
        else
        {
            std::cerr << "Unknown image format!" << std::endl;
            return rslt;
        }
        return rslt;
    }

    Img_Data read_tga(const std::string &path, bool flip)
    {
        // read tga with stb_image
        Img_Data rslt;
        stbi_set_flip_vertically_on_load(flip);
        rslt.data = (char *)stbi_load(path.c_str(), &rslt.width, &rslt.height, &rslt.channels, 0);
        rslt.is_hdr = false;
        rslt.type = GL_UNSIGNED_BYTE;
        if (rslt.channels == 3)
        {
            rslt.format = GL_RGB;
            rslt.inner_format = GL_RGB8;
        }
        else if (rslt.channels == 4)
        {
            rslt.format = GL_RGBA;
            rslt.inner_format = GL_RGBA8;
        }
        else
        {
            std::cerr << "Unknown image format!" << std::endl;
            return rslt;
        }
        return rslt;
    }

    Img_Data read_hdr(const std::string &path, bool flip)
    {
        // read hdr with stb_image
        Img_Data rslt;
        stbi_set_flip_vertically_on_load(flip);
        if (stbi_is_hdr(path.c_str()))
        {
            rslt.data = (char *)stbi_loadf(path.c_str(), &rslt.width, &rslt.height, &rslt.channels, 0);
            rslt.is_hdr = true;
            rslt.type = GL_FLOAT;
            if (rslt.channels == 3)
            {
                rslt.format = GL_RGB;
                rslt.inner_format = GL_RGB16F;
            }
            else if (rslt.channels == 4)
            {
                rslt.format = GL_RGBA;
                rslt.inner_format = GL_RGBA16F;
            }
            else
            {
                std::cerr << "Unknown image format!" << std::endl;
                return rslt;
            }
        }
        else
        {
            std::cerr << "Unknown image format!" << std::endl;
            return rslt;
        }
        return rslt;
    }

    Img_Data read_tiff(const std::string &path, bool flip)
    {
        // read tiff with libtiff
        Img_Data rslt;
        TIFF *tif = TIFFOpen(path.c_str(), "r");
        if (tif)
        {
            uint32_t width, height;
            uint16_t bits_per_sample, samples_per_pixel;
            uint32_t *raster;
            TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
            TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
            TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
            raster = (uint32_t *)_TIFFmalloc(width * height * sizeof(uint32_t));
            if (raster != NULL)
            {
                if (TIFFReadRGBAImage(tif, width, height, raster, 0))
                {
                    rslt.width = width;
                    rslt.height = height;
                    rslt.channels = 4;
                    rslt.is_hdr = false;
                    rslt.type = GL_UNSIGNED_BYTE;
                    rslt.format = GL_RGBA;
                    rslt.inner_format = GL_RGBA8;
                    rslt.data = (char *)raster;
                }
                else
                {
                    std::cerr << "Unknown image format!" << std::endl;
                }
            }
            else
            {
                std::cerr << "Unknown image format!" << std::endl;
            }
            TIFFClose(tif);
        }
        else
        {
            std::cerr << "Unknown image format!" << std::endl;
        }
        return rslt;
    }

    // load texture from file with FreeImage Library
    Texture *load_texture(const std::string &path)
    {
        Texture::Format format;
        format.target = GL_TEXTURE_2D;
        auto img = image_data(path, true);
        if (img.data == nullptr)
        {
            return nullptr;
        }
        format.internal_format = img.inner_format;
        format.format = img.format;
        format.type = img.type;

        auto tex_params = Texture::TexParams::linear_mipmap_repeat();
        Texture *texture = new Texture(format, tex_params);
        texture->set_data(img.data, img.width, img.height);
        texture->generate_mipmap();
        img.release();
        return texture;
    }

    Texture *load_cube_texture(const std::string &path)
    {
        Texture::Format format;
        format.target = GL_TEXTURE_CUBE_MAP;

        auto paths = Core::Tools::split_str(path, ";");
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

    Texture *Texture_Manager::create_1pixel_2d_texture(float r, float g, float b, float a)

    {
        // create a one-pixel placeholder 2d texture
        Texture::Format format = Texture::Format(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_FLOAT);
        Texture::TexParams params = Texture::TexParams::nearest_clamp_edge();
        Texture *texture = new Texture(format, params);
        float data[] = {r, g, b, a};
        texture->set_data(data, 1, 1);
        return texture;
    }

    Texture *Texture_Manager::create_1pixel_cubemap_texture(float r, float g, float b, float a)
    {
        // create a one-pixel placeholder cubemap texture
        Texture::Format format = Texture::Format(GL_TEXTURE_CUBE_MAP, GL_RGBA, GL_RGBA, GL_FLOAT);
        Texture::TexParams params = Texture::TexParams::nearest_clamp_edge();
        Texture *texture = new Texture(format, params);
        float data[] = {r, g, b, a};
        texture->bind();
        for (int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format.internal_format, 1, 1, 0, format.format, format.type, data);
        }
        texture->unbind();
        return texture;
    }

} // namespace Rendering

#include "texture.h"
#include "ui_log.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <tiffio.h>
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
        glTexParameteri(format.target, GL_TEXTURE_WRAP_S, params.wrap_s);
        glTexParameteri(format.target, GL_TEXTURE_WRAP_T, params.wrap_t);
        glTexParameteri(format.target, GL_TEXTURE_WRAP_R, params.wrap_r);
        glTexParameterfv(format.target, GL_TEXTURE_BORDER_COLOR, params.border_color);
        glTexParameteri(format.target, GL_TEXTURE_MIN_FILTER, params.min_filter);
        glTexParameteri(format.target, GL_TEXTURE_MAG_FILTER, params.mag_filter);
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

        auto tex_params = Texture::TexParams(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT);
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

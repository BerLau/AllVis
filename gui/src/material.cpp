#include "material.h"

namespace Rendering
{
    Material::Material(Core::Vector3 color, float metallic, float roughness, float ao, Core::Vector3 emissive)
        : color(color),
          metallic(metallic),
          roughness(std::fmax(roughness, 0.00001)),
          ao(ao),
          albedo_map(nullptr),
          metallic_map(nullptr),
          roughness_map(nullptr),
          ao_map(nullptr),
          emissive_map(nullptr),
          normal_map(nullptr),
          height_map(nullptr)
    {
    }
    Material::Material(Texture *albedo_map, Texture *metallic_map, Texture *roughness_map, Texture *ao_map, Texture *emissive_map, Texture *normal_map, Texture *height_map)
        : color(Core::Vector3(1.0f, 1.0f, 1.0f)),
          metallic(0.0f),
          roughness(0.00001f),
          ao(1.0f),
          albedo_map(albedo_map),
          metallic_map(metallic_map),
          roughness_map(roughness_map),
          ao_map(ao_map),
          emissive_map(emissive_map),
          normal_map(normal_map),
          height_map(height_map)
    {
    }

    void Material::bind() const
    {
        if (albedo_map)
        {
            glActiveTexture(GL_TEXTURE0);
            albedo_map->bind();
            // albedo_map->sampler->bind(0);
        }
        if (normal_map)
        {
            glActiveTexture(GL_TEXTURE1);
            normal_map->bind();
            // normal_map->sampler->bind(1);
        }
        if (height_map)
        {
            glActiveTexture(GL_TEXTURE2);
            height_map->bind();
            // height_map->sampler->bind(2);
        }
        if (metallic_map)
        {
            glActiveTexture(GL_TEXTURE3);
            metallic_map->bind();
            // metallic_map->sampler->bind(3);
        }
        if (roughness_map)
        {
            glActiveTexture(GL_TEXTURE4);
            roughness_map->bind();
            // roughness_map->sampler->bind(4);
        }
        if (ao_map)
        {
            glActiveTexture(GL_TEXTURE5);
            ao_map->bind();
            // ao_map->sampler->bind(5);
        }
        if (emissive_map)
        {
            glActiveTexture(GL_TEXTURE6);
            emissive_map->bind();
            // emissive_map->sampler->bind(6);
        }

    }

    void Material::unbind() const
    {
        if (albedo_map)
        {
            albedo_map->unbind();
        }
        if (metallic_map)
        {
            metallic_map->unbind();
        }
        if (roughness_map)
        {
            roughness_map->unbind();
        }
        if (ao_map)
        {
            ao_map->unbind();
        }
        if (emissive_map)
        {
            emissive_map->unbind();
        }
        if (normal_map)
        {
            normal_map->unbind();
        }
        if (height_map)
        {
            height_map->unbind();
        }
    }

    void Material::write_to_shader(const std::string &name_, Shader_Program *shader)
    {
        shader->set_vec3(name_ + ".color", get_albedo().data());
        shader->set_float(name_ + ".metallic", get_metallic());
        shader->set_float(name_ + ".roughness", get_roughness());
        shader->set_float(name_ + ".ao", get_ao());
        if (is_emissive)
        {
            shader->set_bool(name_ + ".is_emissive", true);
        }
        else
        {
            shader->set_bool(name_ + ".is_emissive", false);
        }
        if (get_albedo_map() != nullptr)
        {
            shader->set_bool(name_ + ".has_albedo_map", true);
            shader->set_int(name_ + ".albedo_map", 0);
        }
        else
        {
            shader->set_bool(name_ + ".has_albedo_map", false);
        }
                if (get_normal_map() != nullptr)
        {
            shader->set_bool(name_ + ".has_normal_map", true);
            shader->set_int(name_ + ".normal_map", 1);
        }
        else
        {
            shader->set_bool(name_ + ".has_normal_map", false);
        }
        if (get_height_map() != nullptr)
        {
            shader->set_bool(name_ + ".has_height_map", true);
            shader->set_int(name_ + ".height_map", 2);
        }
        else
        {
            shader->set_bool(name_ + ".has_height_map", false);
        }
        if (get_metallic_map() != nullptr)
        {
            shader->set_bool(name_ + ".has_metallic_map", true);
            shader->set_int(name_ + ".metallic_map", 3);
        }
        else
        {
            shader->set_bool(name_ + ".has_metallic_map", false);
        }
        if (get_roughness_map() != nullptr)
        {
            shader->set_bool(name_ + ".has_roughness_map", true);
            shader->set_int(name_ + ".roughness_map", 4);
        }
        else
        {
            shader->set_bool(name_ + ".has_roughness_map", false);
        }
        if (get_ao_map() != nullptr)
        {
            shader->set_bool(name_ + ".has_ao_map", true);
            shader->set_int(name_ + ".ao_map", 5);
        }
        else
        {
            shader->set_bool(name_ + ".has_ao_map", false);
        }
        if (get_emissive_map() != nullptr)
        {
            shader->set_bool(name_ + ".has_emissive_map", true);
            shader->set_int(name_ + ".emissive_map", 6);
        }
        else
        {
            shader->set_bool(name_ + ".has_emissive_map", false);
        }

    }

    void Material::write_to_shader(const std::string &name_, unsigned int index, Shader_Program *shader)
    {
        auto name_i = name_ + "[" + std::to_string(index) + "]";
        shader->set_vec3(name_ + ".color", get_albedo().data());
    }

} // namespace Rendering

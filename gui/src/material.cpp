#include "material.h"

namespace Rendering
{
    Material_PBR::Material_PBR(Core::Vector3 color, float metallic, float roughness, float ao, Core::Vector3 emissive)
        : Material("Material_PBR"),
          color(color),
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
    Material_PBR::Material_PBR(Texture *albedo_map, Texture *metallic_map, Texture *roughness_map, Texture *ao_map, Texture *emissive_map, Texture *normal_map, Texture *height_map)
        : Material("Material_PBR"),
          color(Core::Vector3(1.0f, 1.0f, 1.0f)),
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

    void Material_PBR::bind(unsigned int start_index) const
    {
        if (albedo_map)
        {
            glActiveTexture(GL_TEXTURE0 + start_index + 0);
            albedo_map->bind();
        }
        if (normal_map)
        {
            glActiveTexture(GL_TEXTURE0 + start_index + 1);
            normal_map->bind();
        }
        if (height_map)
        {
            glActiveTexture(GL_TEXTURE0 + start_index + 2);
            height_map->bind();
        }
        if (metallic_map)
        {
            glActiveTexture(GL_TEXTURE0 + start_index + 3);
            metallic_map->bind();
        }
        if (roughness_map)
        {
            glActiveTexture(GL_TEXTURE0 + start_index + 4);
            roughness_map->bind();
        }
        if (ao_map)
        {
            glActiveTexture(GL_TEXTURE0 + start_index + 5);
            ao_map->bind();
        }
        if (emissive_map)
        {
            glActiveTexture(GL_TEXTURE0 + start_index + 6);
            emissive_map->bind();
        }
    }

    void Material_PBR::unbind() const
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

    void Material_PBR::set_map(Texture *tex, const std::string &path, Map_Type type)
    {
        switch (type)
        {
        case ALBEDO_MAP:
            albedo_map = tex;
            albedo_map_path = path;
            break;
        case METALLIC_MAP:
            metallic_map = tex;
            metallic_map_path = path;
            break;
        case ROUGHNESS_MAP:
            roughness_map = tex;
            roughness_map_path = path;
            break;
        case AO_MAP:
            ao_map = tex;
            ao_map_path = path;
            break;
        case EMISSIVE_MAP:
            emissive_map = tex;
            emissive_map_path = path;
            break;
        case NORMAL_MAP:
            normal_map = tex;
            normal_map_path = path;
            break;
        case HEIGHT_MAP:
            height_map = tex;
            height_map_path = path;
            break;
        default:
            break;
        }
    }

    void Material_PBR::write_to_shader(const std::string &m_name, Shader_Program *shader, unsigned int start_index)
    {
        shader->set_vec3(m_name + ".color", get_albedo().data());
        shader->set_float(m_name + ".metallic", get_metallic());
        shader->set_float(m_name + ".roughness", get_roughness());
        shader->set_float(m_name + ".ao", get_ao());

        if (get_albedo_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_albedo_map", true);
            glActiveTexture(GL_TEXTURE0 + start_index + 0);
            shader->set_int(m_name + ".albedo_map", start_index + 0);
        }
        else
        {
            shader->set_bool(m_name + ".has_albedo_map", false);
        }
        if (get_normal_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_normal_map", true);
            glActiveTexture(GL_TEXTURE0 + start_index + 1);
            shader->set_int(m_name + ".normal_map", start_index + 1);
        }
        else
        {
            shader->set_bool(m_name + ".has_normal_map", false);
        }
        if (get_height_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_height_map", true);
            shader->set_int(m_name + ".height_map", start_index + 2);
        }
        else
        {
            shader->set_bool(m_name + ".has_height_map", false);
        }
        if (get_metallic_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_metallic_map", true);
            shader->set_int(m_name + ".metallic_map", start_index + 3);
        }
        else
        {
            shader->set_bool(m_name + ".has_metallic_map", false);
        }
        if (get_roughness_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_roughness_map", true);
            shader->set_int(m_name + ".roughness_map", start_index + 4);
        }
        else
        {
            shader->set_bool(m_name + ".has_roughness_map", false);
        }
        if (get_ao_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_ao_map", true);
            shader->set_int(m_name + ".ao_map", start_index + 5);
        }
        else
        {
            shader->set_bool(m_name + ".has_ao_map", false);
        }
        if (is_emissive)
        {
            shader->set_bool(m_name + ".is_emissive", true);
            shader->set_vec3(m_name + ".emissive", get_albedo().data());
            if (get_emissive_map() != nullptr)
            {
                shader->set_bool(m_name + ".has_emissive_map", true);
                shader->set_int(m_name + ".emissive_map", start_index + 6);
            }
            else
            {
                shader->set_bool(m_name + ".has_emissive_map", false);
            }
        }
        else
        {
            shader->set_bool(m_name + ".is_emissive", false);
        }
    }

    Material_PHONG::Material_PHONG(Core::Vector3 ambient, Core::Vector3 diffuse, Core::Vector3 specular, float shininess)
        : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
    {
    }

    Material_PHONG::Material_PHONG(Texture *ambient_map, Texture *diffuse_map, Texture *specular_map, Texture *normal_map, Texture *height_map)
        : ambient_map(ambient_map), diffuse_map(diffuse_map), specular_map(specular_map), normal_map(normal_map), height_map(height_map)
    {
    }

    void Material_PHONG::bind(unsigned int start_index) const
    {
        if (ambient_map)
        {
            glActiveTexture(GL_TEXTURE0 + start_index + 0);
            ambient_map->bind();
        }
        if (diffuse_map)
        {
            glActiveTexture(GL_TEXTURE0 + start_index + 1);
            diffuse_map->bind();
        }
        if (specular_map)
        {
            glActiveTexture(GL_TEXTURE0 + start_index + 2);
            specular_map->bind();
        }
        if (normal_map)
        {
            glActiveTexture(GL_TEXTURE0 + start_index + 3);
            normal_map->bind();
        }
        if (height_map)
        {
            glActiveTexture(GL_TEXTURE0 + start_index + 4);
            height_map->bind();
        }
    }

    void Material_PHONG::unbind() const
    {
        if (ambient_map)
        {
            ambient_map->unbind();
        }
        if (diffuse_map)
        {
            diffuse_map->unbind();
        }
        if (specular_map)
        {
            specular_map->unbind();
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

    void Material_PHONG::write_to_shader(const std::string &m_name, Shader_Program *shader, unsigned int start_index)
    {
        shader->set_vec3(m_name + ".ambient", get_ambient().data());
        shader->set_vec3(m_name + ".diffuse", get_diffuse().data());
        shader->set_vec3(m_name + ".specular", get_specular().data());
        shader->set_float(m_name + ".shininess", get_shininess());
        if (get_ambient_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_ambient_map", true);
            shader->set_int(m_name + ".ambient_map", start_index + 0);
        }
        else
        {
            shader->set_bool(m_name + ".has_ambient_map", false);
        }
        if (get_diffuse_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_diffuse_map", true);
            shader->set_int(m_name + ".diffuse_map", start_index + 1);
        }
        else
        {
            shader->set_bool(m_name + ".has_diffuse_map", false);
        }
        if (get_specular_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_specular_map", true);
            shader->set_int(m_name + ".specular_map", start_index + 2);
        }
        else
        {
            shader->set_bool(m_name + ".has_specular_map", false);
        }
        if (get_normal_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_normal_map", true);
            shader->set_int(m_name + ".normal_map", start_index + 3);
        }
        else
        {
            shader->set_bool(m_name + ".has_normal_map", false);
        }
        if (get_height_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_height_map", true);
            shader->set_int(m_name + ".height_map", start_index + 4);
        }
        else
        {
            shader->set_bool(m_name + ".has_height_map", false);
        }
    }

} // namespace Rendering

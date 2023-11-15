#include "material.h"
#include "scene.h"
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

    void Material_PBR::unbind() const
    {
        glActiveTexture(GL_TEXTURE0);
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

    void Material_PBR::write_to_shader(const std::string &m_name, Shader_Program *shader)
    {
        shader->set_vec3(m_name + ".color", get_albedo().data());
        shader->set_float(m_name + ".metallic", get_metallic());
        shader->set_float(m_name + ".roughness", get_roughness());
        shader->set_float(m_name + ".ao", get_ao());
        auto albedo_map = get_albedo_map();
        if (albedo_map)
        {
            shader->set_bool(m_name + ".has_albedo_map", true);
            albedo_map->bind(PBR_TEXTURE_UNIT::ALBEDO);
            shader->set_int(m_name + ".albedo_map", PBR_TEXTURE_UNIT::ALBEDO);
        }
        else
        {
            shader->set_bool(m_name + ".has_albedo_map", false);
        }
        auto normal_map = get_normal_map();
        if (normal_map)
        {
            shader->set_bool(m_name + ".has_normal_map", true);
            normal_map->bind(PBR_TEXTURE_UNIT::NORMAL);
            shader->set_int(m_name + ".normal_map", PBR_TEXTURE_UNIT::NORMAL);
        }
        else
        {
            shader->set_bool(m_name + ".has_normal_map", false);
        }
        auto height_map = get_height_map();
        if (height_map)
        {
            shader->set_bool(m_name + ".has_height_map", true);
            height_map->bind(PBR_TEXTURE_UNIT::HEIGHT);
            shader->set_int(m_name + ".height_map", PBR_TEXTURE_UNIT::HEIGHT);
        }
        else
        {
            shader->set_bool(m_name + ".has_height_map", false);
        }
        auto metallic_map = get_metallic_map();
        if (metallic_map)
        {
            shader->set_bool(m_name + ".has_metallic_map", true);
            metallic_map->bind(PBR_TEXTURE_UNIT::METALLIC);
            shader->set_int(m_name + ".metallic_map", PBR_TEXTURE_UNIT::METALLIC);
        }
        else
        {
            shader->set_bool(m_name + ".has_metallic_map", false);
        }
        auto roughness_map = get_roughness_map();
        if (roughness_map)
        {
            shader->set_bool(m_name + ".has_roughness_map", true);
            roughness_map->bind(PBR_TEXTURE_UNIT::ROUGHNESS);
            shader->set_int(m_name + ".roughness_map", PBR_TEXTURE_UNIT::ROUGHNESS);
        }
        else
        {
            shader->set_bool(m_name + ".has_roughness_map", false);
        }
        auto ao_map = get_ao_map();
        if (ao_map)
        {
            shader->set_bool(m_name + ".has_ao_map", true);
            ao_map->bind(PBR_TEXTURE_UNIT::AO);
            shader->set_int(m_name + ".ao_map", PBR_TEXTURE_UNIT::AO);
        }
        else
        {
            shader->set_bool(m_name + ".has_ao_map", false);
        }
        if (is_emissive)
        {
            shader->set_bool(m_name + ".is_emissive", true);
            shader->set_vec3(m_name + ".emissive", get_albedo().data());
            auto emissive_map = get_emissive_map();
            if (emissive_map)
            {
                shader->set_bool(m_name + ".has_emissive_map", true);
                emissive_map->bind(PBR_TEXTURE_UNIT::EMISSIVE);
                shader->set_int(m_name + ".emissive_map", PBR_TEXTURE_UNIT::EMISSIVE);
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

    void Material_PHONG::write_to_shader(const std::string &m_name, Shader_Program *shader)
    {
        shader->set_vec3(m_name + ".ambient", get_ambient().data());
        shader->set_vec3(m_name + ".diffuse", get_diffuse().data());
        shader->set_vec3(m_name + ".specular", get_specular().data());
        shader->set_float(m_name + ".shininess", get_shininess());
        if (get_ambient_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_ambient_map", true);
            get_ambient_map()->bind(PHONG_TEXTURE_UNIT::AMBIENT);
            shader->set_int(m_name + ".ambient_map", PHONG_TEXTURE_UNIT::AMBIENT);
        }
        else
        {
            shader->set_bool(m_name + ".has_ambient_map", false);
        }
        if (get_diffuse_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_diffuse_map", true);
            get_diffuse_map()->bind(PHONG_TEXTURE_UNIT::DIFFUSE);
            shader->set_int(m_name + ".diffuse_map", PHONG_TEXTURE_UNIT::DIFFUSE);
        }
        else
        {
            shader->set_bool(m_name + ".has_diffuse_map", false);
        }
        if (get_specular_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_specular_map", true);
            get_specular_map()->bind(PHONG_TEXTURE_UNIT::SPECULAR);
            shader->set_int(m_name + ".specular_map", PHONG_TEXTURE_UNIT::SPECULAR);
        }
        else
        {
            shader->set_bool(m_name + ".has_specular_map", false);
        }
        if (get_normal_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_normal_map", true);
            get_normal_map()->bind(PHONG_TEXTURE_UNIT::NORMAL);
            shader->set_int(m_name + ".normal_map", PHONG_TEXTURE_UNIT::NORMAL);
        }
        else
        {
            shader->set_bool(m_name + ".has_normal_map", false);
        }
        if (get_height_map() != nullptr)
        {
            shader->set_bool(m_name + ".has_height_map", true);
            get_height_map()->bind(PHONG_TEXTURE_UNIT::HEIGHT);
            shader->set_int(m_name + ".height_map", PHONG_TEXTURE_UNIT::HEIGHT);
        }
        else
        {
            shader->set_bool(m_name + ".has_height_map", false);
        }
    }

} // namespace Rendering

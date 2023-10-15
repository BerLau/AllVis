#include "material.h"

namespace Rendering
{
    Material::Material(Core::Vector3 color, float metallic, float roughness, float ao, Core::Vector3 emissive)
        : albedo(color),
          metallic(metallic),
          roughness(roughness),
          ao(ao),
          emissive(emissive),
          albedo_map(nullptr),
          metallic_map(nullptr),
          roughness_map(nullptr),
          ao_map(nullptr),
          emissive_map(nullptr),
          normal_map(nullptr),
          height_map(nullptr)
    {
    }
    Material::Material(Texture_Ptr albedo_map, Texture_Ptr metallic_map, Texture_Ptr roughness_map, Texture_Ptr ao_map, Texture_Ptr emissive_map, Texture_Ptr normal_map, Texture_Ptr height_map)
        : albedo(Core::Vector3(1.0f, 1.0f, 1.0f)),
          metallic(0.0f),
          roughness(0.0f),
          ao(1.0f),
          emissive(Core::Vector3(0.0f, 0.0f, 0.0f)),
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
            albedo_map->bind(0);
        }
        if (metallic_map)
        {
            metallic_map->bind(1);
        }
        if (roughness_map)
        {
            roughness_map->bind(2);
        }
        if (ao_map)
        {
            ao_map->bind(3);
        }
        if (emissive_map)
        {
            emissive_map->bind(4);
        }
        if (normal_map)
        {
            normal_map->bind(5);
        }
        if (height_map)
        {
            height_map->bind(6);
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

} // namespace Rendering

#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include "core.h"
#include "texture.h"
#include "shader.h"

namespace Rendering
{
    class Material;
    using Material_Ptr_S = std::shared_ptr<Material>;
    using Material_Ptr_U = std::unique_ptr<Material>;
    using Material_Ptr_W = std::weak_ptr<Material>;
    using Material_Ptr = Material_Ptr_S;

    class Material : public Core::Configurable
    {
        // struct
    public:
        enum Map_Type
        {
            ALBEDO_MAP = 1,
            METALLIC_MAP,
            ROUGHNESS_MAP,
            AO_MAP,
            EMISSIVE_MAP,
            NORMAL_MAP,
            HEIGHT_MAP
        };
        // attributes
    public:
        Core::Vector3 color;
        float metallic;
        float roughness;
        float ao;
        bool is_emissive = false;
        std::string albedo_map_path = "";
        Texture *albedo_map = nullptr;

        std::string metallic_map_path = "";
        Texture *metallic_map = nullptr;

        std::string roughness_map_path = "";
        Texture *roughness_map;

        std::string ao_map_path = "";
        Texture *ao_map;

        std::string emissive_map_path = "";
        Texture *emissive_map;

        std::string normal_map_path = "";
        Texture *normal_map;

        std::string height_map_path = "";
        Texture *height_map;
        // constructors and deconstructor
    public:
        Material(Core::Vector3 color = Core::Vector3(0.5,0.5,0.5), float metallic=0.5, float roughness=0.5, float ao = 0.5, Core::Vector3 emissive = Core::Vector3(0.0f, 0.0f, 0.0f));
        Material(Texture *albedo_map, Texture *metallic_map, Texture *roughness_map, Texture *ao_map, Texture *emissive_map = nullptr, Texture *normal_map = nullptr, Texture *height_map = nullptr);
        ~Material() {}
        // methods
    public:
        void bind() const;
        void unbind() const;
        void set_albedo(Core::Vector3 color) { color = color; }
        void set_metallic(float metallic) { this->metallic = metallic; }
        void set_roughness(float roughness) { this->roughness = roughness; }
        void set_ao(float ao) { this->ao = ao; }

        void set_map(Texture *tex, const std::string &path, Map_Type type)
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
        Core::Vector3 get_albedo() const { return color; }
        float get_metallic() const { return metallic; }
        float get_roughness() const { return roughness; }
        float get_ao() const { return ao; }

        Texture *get_albedo_map() const { return albedo_map; }
        Texture *get_metallic_map() const { return metallic_map; }
        Texture *get_roughness_map() const { return roughness_map; }
        Texture *get_ao_map() const { return ao_map; }
        Texture *get_emissive_map() const { return emissive_map; }
        Texture *get_normal_map() const { return normal_map; }
        Texture *get_height_map() const { return height_map; }

        void write_to_shader(const std::string& name, Shader_Program *shader);
        void write_to_shader(const std::string& name, unsigned int index, Shader_Program *shader);
    };
} // namespace Rendering

#endif // MATERIAL_H
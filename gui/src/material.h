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
        // attributes
        // constructors and deconstructor
    public:
        Material(const std::string &name = "Material") : Core::Configurable(name) {}
        virtual ~Material() {}
        // methods
    public:
        virtual void unbind() const = 0;
        virtual void write_to_shader(const std::string &m_name, Shader_Program *shader) = 0;
        void write_to_shader(const std::string &m_name, unsigned int index, Shader_Program *shader)
        {
            auto name_i = m_name + "[" + std::to_string(index) + "]";
            write_to_shader(name_i, shader);
        }
        // static methods
    };

    class Material_PBR;
    using Material_PBR_Ptr_S = std::shared_ptr<Material_PBR>;
    using Material_PBR_Ptr_U = std::unique_ptr<Material_PBR>;
    using Material_PBR_Ptr_W = std::weak_ptr<Material_PBR>;
    using Material_PBR_Ptr = Material_PBR_Ptr_S;

    class Material_PBR : public Material
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
        Texture *roughness_map = nullptr;

        std::string ao_map_path = "";
        Texture *ao_map = nullptr;

        std::string emissive_map_path = "";
        Texture *emissive_map = nullptr;

        std::string normal_map_path = "";
        Texture *normal_map = nullptr;

        std::string height_map_path = "";
        Texture *height_map = nullptr;
        // constructors and deconstructor
    public:
        Material_PBR(Core::Vector3 color = Core::Vector3(0.5, 0.5, 0.5), float metallic = 0.5, float roughness = 0.5, float ao = 0.5, Core::Vector3 emissive = Core::Vector3(0.0f, 0.0f, 0.0f));
        Material_PBR(Texture *albedo_map, Texture *metallic_map, Texture *roughness_map, Texture *ao_map, Texture *emissive_map = nullptr, Texture *normal_map = nullptr, Texture *height_map = nullptr);
        ~Material_PBR() {}
        // methods
    public:
        void unbind() const;
        void set_albedo(Core::Vector3 color) { color = color; }
        void set_metallic(float metallic) { this->metallic = metallic; }
        void set_roughness(float roughness) { this->roughness = roughness; }
        void set_ao(float ao) { this->ao = ao; }

        void set_map(Texture *tex, const std::string &path, Map_Type type);
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

        void write_to_shader(const std::string &m_name, Shader_Program *shader);
    };

    class Material_PHONG;
    using Material_PHONG_Ptr_S = std::shared_ptr<Material_PHONG>;
    using Material_PHONG_Ptr_U = std::unique_ptr<Material_PHONG>;
    using Material_PHONG_Ptr_W = std::weak_ptr<Material_PHONG>;
    using Material_PHONG_Ptr = Material_PHONG_Ptr_S;

    class Material_PHONG : public Material
    {
        // interface
    public:
        void bind(unsigned int start_index = 0) const;
        void unbind() const;
        void write_to_shader(const std::string &m_name, Shader_Program *shader);
        // struct
    public:
        enum Map_Type
        {
            DIFFUSE_MAP = 1,
            SPECULAR_MAP,
            NORMAL_MAP,
            HEIGHT_MAP
        };
        // attributes
    public:
        Core::Vector3 ambient;
        Core::Vector3 diffuse;
        Core::Vector3 specular;
        float shininess;
        bool is_emissive = false;

        std::string ambient_map_path = "";
        Texture *ambient_map = nullptr;

        std::string diffuse_map_path = "";
        Texture *diffuse_map = nullptr;

        std::string specular_map_path = "";
        Texture *specular_map = nullptr;

        std::string normal_map_path = "";
        Texture *normal_map = nullptr;

        std::string height_map_path = "";
        Texture *height_map = nullptr;

        // constructors and deconstructor
    public:
        Material_PHONG(Core::Vector3 ambient = Core::Vector3(0.1f, 0.1f, 0.1f), Core::Vector3 diffuse = Core::Vector3(0.5f, 0.5f, 0.5f), Core::Vector3 specular = Core::Vector3(1.0f, 1.0f, 1.0f), float shininess = 32.0f);
        Material_PHONG(Texture *ambient_map, Texture *diffuse_map, Texture *specular_map, Texture *normal_map = nullptr, Texture *height_map = nullptr);
        ~Material_PHONG() {}
        // methods
    public:
        void set_ambient(Core::Vector3 ambient) { this->ambient = ambient; }
        void set_diffuse(Core::Vector3 diffuse) { this->diffuse = diffuse; }
        void set_specular(Core::Vector3 specular) { this->specular = specular; }
        void set_shininess(float shininess) { this->shininess = shininess; }
        void set_map(Texture *tex, const std::string &path, Map_Type type)
        {
            switch (type)
            {
            case DIFFUSE_MAP:
                diffuse_map = tex;
                diffuse_map_path = path;
                break;
            case SPECULAR_MAP:
                specular_map = tex;
                specular_map_path = path;
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
        Core::Vector3 get_ambient() const { return ambient; }
        Core::Vector3 get_diffuse() const { return diffuse; }
        Core::Vector3 get_specular() const { return specular; }
        float get_shininess() const { return shininess; }
        Texture *get_ambient_map() const { return ambient_map; }
        Texture *get_diffuse_map() const { return diffuse_map; }
        Texture *get_specular_map() const { return specular_map; }
        Texture *get_normal_map() const { return normal_map; }
        Texture *get_height_map() const { return height_map; }
        // static methods
    };
} // namespace Rendering

#endif // MATERIAL_H
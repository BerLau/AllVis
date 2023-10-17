#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include "core.h"
#include "texture.h"

namespace Rendering
{
    class Material;
    using Material_Ptr_S = std::shared_ptr<Material>;
    using Material_Ptr_U = std::unique_ptr<Material>;
    using Material_Ptr_W = std::weak_ptr<Material>;
    using Material_Ptr = Material_Ptr_S;

    class Material: public Core::Configurable
    {
        // attributes
    public:
        Core::Vector3 albedo;
        float metallic;
        float roughness;
        float ao;
        Core::Vector3 emissive;

        Texture_Ptr albedo_map;
        Texture_Ptr metallic_map;
        Texture_Ptr roughness_map;
        Texture_Ptr ao_map;
        Texture_Ptr emissive_map;
        Texture_Ptr normal_map;
        Texture_Ptr height_map;
        // constructors and deconstructor
    public:
        Material(Core::Vector3 color, float metallic, float roughness, float ao, Core::Vector3 emissive = Core::Vector3(0.0f, 0.0f, 0.0f));
        Material(Texture_Ptr albedo_map, Texture_Ptr metallic_map, Texture_Ptr roughness_map, Texture_Ptr ao_map, Texture_Ptr emissive_map = nullptr, Texture_Ptr normal_map = nullptr, Texture_Ptr height_map = nullptr);
        ~Material() {}
        // methods
    public:
        void bind() const;
        void unbind() const;
        void set_albedo(Core::Vector3 color) { albedo = color; }
        void set_metallic(float metallic) { this->metallic = metallic; }
        void set_roughness(float roughness) { this->roughness = roughness; }
        void set_ao(float ao) { this->ao = ao; }
        void set_emissive(Core::Vector3 emissive) { this->emissive = emissive; }
        void set_albedo_map(Texture_Ptr albedo_map) { this->albedo_map = albedo_map; }
        void set_metallic_map(Texture_Ptr metallic_map) { this->metallic_map = metallic_map; }
        void set_roughness_map(Texture_Ptr roughness_map) { this->roughness_map = roughness_map; }
        void set_ao_map(Texture_Ptr ao_map) { this->ao_map = ao_map; }
        void set_emissive_map(Texture_Ptr emissive_map) { this->emissive_map = emissive_map; }
        void set_normal_map(Texture_Ptr normal_map) { this->normal_map = normal_map; }
        void set_height_map(Texture_Ptr height_map) { this->height_map = height_map; }
        
        Core::Vector3 get_albedo() const { return albedo; }
        float get_metallic() const { return metallic; }
        float get_roughness() const { return roughness; }
        float get_ao() const { return ao; }
        Core::Vector3 get_emissive() const { return emissive; }
        Texture_Ptr get_albedo_map() const { return albedo_map; }
        Texture_Ptr get_metallic_map() const { return metallic_map; }
        Texture_Ptr get_roughness_map() const { return roughness_map; }
        Texture_Ptr get_ao_map() const { return ao_map; }
        Texture_Ptr get_emissive_map() const { return emissive_map; }
        Texture_Ptr get_normal_map() const { return normal_map; }
        Texture_Ptr get_height_map() const { return height_map; }
    };
} // namespace Rendering

#endif // MATERIAL_H
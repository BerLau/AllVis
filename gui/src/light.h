#pragma once
#ifndef RENDERING_LIGHT_H
#define RENDERING_LIGHT_H

#include <core.h>
#include <memory>
#include "models.h"

namespace Rendering
{
    class Light;
    using Light_Ptr_U = std::unique_ptr<Light>;
    using Light_Ptr_S = std::shared_ptr<Light>;
    using Light_Ptr_W = std::weak_ptr<Light>;
    using Light_Ptr = Light_Ptr_U;

    class Light : public Sphere_Model
    {
    public:
        enum Light_Type
        {
            PARALLEL_LIGHT = 0,
            POINT_LIGHT,
            SPOT_LIGHT
        };
        struct Properties
        {
            Light_Type type = POINT_LIGHT;
            Core::Vector3 color = Core::Vector3{1.0, 1.0, 1.0};
            float intensity = 1.0;
        };
        // attributes
    public:
        Properties properties;
        // constructors and deconstructor
    public:
        Light(Properties properties = {POINT_LIGHT, Core::Vector3{1.0, 1.0, 1.0}, 1.0}) : properties(properties)
        {
            transform->set_scale(0.1);
            material->is_emissive = true;
            material->editable = false;
        }
        Light(Core::Transform *transform, Properties properties = {POINT_LIGHT, Core::Vector3{1.0, 1.0, 1.0}, 1.0}) : properties(properties)
        {
            material->is_emissive = true;
            material->editable = false;
            transform->set_scale(0.1);

        }
        virtual ~Light() {}
        // methods
    public:
        Core::Vector3 get_position() const { return transform->get_position(); }
        Core::Vector3 get_direction() const { return transform->get_front(); }
        Core::Vector3 get_color() const { return properties.color; }
        unsigned int get_type() const { return properties.type; }
        void set_position(Core::Vector3 position) { transform->set_position(position); }
        void set_direction(Core::Vector3 direction) { transform->set_front(direction); }
        void set_color(Core::Vector3 color) { properties.color = color; material->color = color; }
        void set_type(Light_Type type) { properties.type = type; }
        void set_intensity(float intensity) { properties.intensity = intensity; }
        float get_intensity() const { return properties.intensity; }

        void write_to_shader(const std::string &name, Shader_Program *shader);
        void write_to_shader(const std::string &name, int index, Shader_Program *shader);
    };

};

#endif // !RENDERING_LIGHT_H

#pragma once
#ifndef RENDERING_LIGHT_H
#define RENDERING_LIGHT_H

#include <memory>
#include "models.h"
#include "configurable.h"
#include "transform.h"

namespace Rendering
{
    class Light;
    using Light_Ptr_U = std::unique_ptr<Light>;
    using Light_Ptr_S = std::shared_ptr<Light>;
    using Light_Ptr_W = std::weak_ptr<Light>;
    using Light_Ptr = Light_Ptr_U;

    class Light : virtual public Core::Configurable
    {
    public:
        enum Light_Type
        {
            PARALLEL_LIGHT = 0,
            POINT_LIGHT,
            SPOT_LIGHT
        };
        // attributes
    public:
        Light_Type type = POINT_LIGHT;
        Core::Vector3 color = Core::Vector3{1.0, 1.0, 1.0};
        float intensity = 1.0;
        Core::Transform_Ptr transform;
        OGL_Mesh *mesh = nullptr;
        // constructors and deconstructor
    public:
        Light(Light_Type light_type = POINT_LIGHT, Core::Vector3 color = Core::Vector3{1.0, 1.0, 1.0}, float intensity = 1.0)
            : Configurable("Light"),
              type(light_type), color(color), intensity(intensity), transform(std::move(Core::Transform_Ptr(new Core::Transform())))
        {
            mesh = OGL_Mesh::sphere_mesh().release();
            transform->set_scale(0.05);
        }
        Light(Core::Transform *transform, Light_Type light_type = POINT_LIGHT, Core::Vector3 color = Core::Vector3{1.0, 1.0, 1.0}, float intensity = 1.0)
            : Configurable("Light"),
              type(light_type), color(color), intensity(intensity), transform(Core::Transform_Ptr(transform))
        {
            mesh = OGL_Mesh::sphere_mesh().release();
            transform->set_scale(0.05);
        }
        virtual ~Light() {}
        // methods
    public:
        Core::Vector3 get_position() const { return transform->get_position(); }
        Core::Vector3 get_direction() const { return transform->get_front(); }
        void set_position(Core::Vector3 position) { transform->set_position(position); }
        void set_direction(Core::Vector3 direction) { transform->set_front(direction); }
        void spot_on(Core::Vector3 target) { transform->look_at(target); }
        void write_to_shader(const std::string &name, Shader_Program *shader);
        void write_to_shader(const std::string &name, int index, Shader_Program *shader);
        void visualize(Shader_Program *shader);
    };

};

#endif // !RENDERING_LIGHT_H

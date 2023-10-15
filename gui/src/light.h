#pragma once
#ifndef RENDERING_LIGHT_H
#define RENDERING_LIGHT_H

#include <core.h>
#include <memory>

namespace Rendering
{
    class Light;
    using Light_Ptr_U = std::unique_ptr<Light>;
    using Light_Ptr_S = std::shared_ptr<Light>;
    using Light_Ptr_W = std::weak_ptr<Light>;
    using Light_Ptr = Light_Ptr_U;

    class Light
    {
    public:
        enum Light_Type
        {
            PARALLEL_LIGHT = 0,
            POINT_LIGHT,
            SPOT_LIGHT
        };
        struct Property
        {
            Light_Type type;
            Core::Vector3 color;
        };
        // attributes
    public:
        Core::Transform_Ptr transform;
        Property property;
        // constructors and deconstructor
    public:
        Light(Property property = {POINT_LIGHT, Core::Vector3{1.0, 1.0, 1.0}})
            : transform(Core::Transform_Ptr(new Core::Transform())),
              property(property) {}
        Light(Core::Transform *transform, Property property = {POINT_LIGHT, Core::Vector3{1.0, 1.0, 1.0}})
            : transform(transform),
              property(property) {}
        virtual ~Light() {}
        // methods
    public:
        Core::Vector3 get_position() const { return transform->get_position(); }
        Core::Vector3 get_direction() const { return transform->get_front(); }
        Core::Vector3 get_color() const { return property.color; }
        unsigned int get_type() const { return property.type; }
        void set_position(Core::Vector3 position) { transform->set_position(position); }
        void set_direction(Core::Vector3 direction) { transform->set_front(direction); }
        void set_color(Core::Vector3 color) { property.color = color; }
        void set_type(Light_Type type) { property.type = type; }
    };

};

#endif // !RENDERING_LIGHT_H

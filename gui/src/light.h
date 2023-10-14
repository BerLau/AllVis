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
    const unsigned int LIGHT_TYPE_DIRECTIONAL = 0;
    const unsigned int LIGHT_TYPE_POINT = 1;
    const unsigned int LIGHT_TYPE_SPOT = 2;
    class Light
    {
    public:
        struct Property
        {
            unsigned int type;
            Core::Vector3 color;
        };
        // attributes
    public:
        Core::Transform_Ptr transform;
        Property property;
        // constructors and deconstructor
    public:
        Light(Property property = {LIGHT_TYPE_POINT, Core::Vector3{1.0, 1.0, 1.0}})
            : transform(Core::Transform_Ptr(new Core::Transform())),
              property(property) {}
        Light(Core::Transform *transform, Property property = {LIGHT_TYPE_POINT, Core::Vector3{1.0, 1.0, 1.0}})
            : transform(transform),
              property(property) {}
        virtual ~Light() {}
        // methods
    public:
        Core::Vector3 get_position() const { return transform->get_position(); }
        Core::Vector3 get_direction() const { return transform->get_front(); }
        void set_position(Core::Vector3 position) { transform->set_position(position); }
        // void set_direction(Core::Vector3 direction) { transform->set_front(direction); }
        void set_color(Core::Vector3 color) { property.color = color; }
    };

};

#endif // !RENDERING_LIGHT_H

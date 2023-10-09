#pragma once
#ifndef RENDERING_LIGHT_H
#define RENDERING_LIGHT_H

#include <core.h>
#include <memory>

#include "transform.h"

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
            float intensity;
        };
        // attributes
    public:
        Core::Transform_Ptr transform;
        Property property;
        // constructors and deconstructor
    public:
        Light() : transform(nullptr) {}
        Light(Core::Transform *transform) : transform(transform) {}
        virtual ~Light() {}
    };
    // methods

};

#endif // !RENDERING_LIGHT_H

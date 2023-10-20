#pragma once
#ifndef CONFIGURABLE_H
#define CONFIGURABLE_H
#include <string>

namespace Core
{
    class Configurable
    {
        // attributes
    public:
        std::string name;
        // constructors and deconstructor
    public:
        Configurable(const std::string &name = "Configurable Object") : name(name){};
        virtual ~Configurable() {}
        // methods
    };
}; // namespace Core

#endif // !CONFIGURABLE_Hs
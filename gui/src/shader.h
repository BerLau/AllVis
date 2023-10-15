#pragma once
#ifndef GUI_SHADER_H
#define GUI_SHADER_H

#include <glad/glad.h>
#include <string>
#include <memory>
#include <map>
#include "light.h"

namespace Rendering
{
    class Shader;
    using Shader_S_Ptr = std::shared_ptr<Shader>;
    using Shader_U_Ptr = std::unique_ptr<Shader>;
    using Shader_W_Ptr = std::weak_ptr<Shader>;
    using Shader_Ptr = Shader_U_Ptr;

    class Shader
    {
    public:
        std::string source;
        GLenum type;
        GLuint id;
        bool is_compiled;
        Shader(const std::string &source, GLenum type) : source(source),
                                                         type(type),
                                                         id(0),
                                                         is_compiled(false){};
        ~Shader()
        {
            if (id != 0)
            {
                glDeleteShader(id);
            }
        }
    };

    int compile_shader(Shader &shader);

    class Shader_Program;
    using Shader_Program_S_Ptr = std::shared_ptr<Shader_Program>;
    using Shader_Program_U_Ptr = std::unique_ptr<Shader_Program>;
    using Shader_Program_W_Ptr = std::weak_ptr<Shader_Program>;
    using Shader_Program_Ptr = Shader_Program_U_Ptr;

    class Shader_Program
    {
        // attributes
    private:
        GLuint program_id;

    public:
        std::string name;
        bool is_linked;
        // constructors
    public:
        Shader_Program(const std::string &name) : program_id(0),
                                                  name(name),
                                                  is_linked(false){};
        ~Shader_Program()
        {
            if (program_id != 0)
            {
                glDeleteProgram(program_id);
                is_linked = false;
            }
        };
        // methods
    public:
        int link(Shader *vertex_shader, Shader *fragment_shader, Shader *geometry_shader = nullptr);
        void activate();
        void deactivate();
        void set_bool(const std::string &name, bool value) const;
        void set_int(const std::string &name, int value) const;
        void set_float(const std::string &name, float value) const;
        void set_vec2(const std::string &name, float x, float y) const;
        void set_vec2(const std::string &name, const float *vec) const;
        void set_vec3(const std::string &name, float x, float y, float z) const;
        void set_vec3(const std::string &name, const float *vec) const;
        void set_vec4(const std::string &name, float x, float y, float z, float w) const;
        void set_vec4(const std::string &name, const float *vec) const;
        void set_mat2(const std::string &name, const float *mat) const;
        void set_mat3(const std::string &name, const float *mat) const;
        void set_mat4(const std::string &name, const float *mat) const;

        void set_light(const std::string &name, const Rendering::Light &light) const;
        void set_light(const std::string &name, size_t index, const Rendering::Light_Ptr &light) const;
    };

    class Shader_Program_Factory
    {
        // attributes
    public:
        std::map<std::string, Shader_Program_Ptr> shader_programs;
        std::map<std::string, Shader_Ptr> shaders;
        // constructors
    public:
        Shader_Program_Factory(){};
        ~Shader_Program_Factory()
        {
            destroy();
        };

        // methods

    public:
        Shader *find_shader(const std::string &name);
        Shader_Program *find_shader_program(const std::string &name);
        Shader *add_shader_from_source(const std::string &source, GLenum type, const std::string &name);
        Shader *add_shader_from_file(const std::string &path, GLenum type, const std::string &name);
        Shader_Program *add_shader_program(const std::string &name, const std::string &vertex_shader_name, const std::string &fragment_shader_name, const std::string &geometry_shader_name = "");
        Shader_Program *add_shader_program(const std::string &name, Shader *vertex_shader, Shader *fragment_shader, Shader *geometry_shader = nullptr);
        void remove_shader(const std::string &name);
        void remove_shader_program(const std::string &name);
        void destroy()
        {
            shader_programs.clear();
            shaders.clear();
        }
    };

    extern Shader_Program_Factory shader_program_factory;
}; // namespace GUI

#endif // GUI_SHADER_H
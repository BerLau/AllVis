#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace Rendering
{
    Shader_Program_Factory shader_program_factory;
    std::string read_file(const std::string &path)
    {
        std::string code = "";
        std::ifstream fs;
        fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            fs.open(path);
            std::stringstream ss;
            ss << fs.rdbuf();
            fs.close();
            code = ss.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n"
                      << "FILE: " << path << std::endl;
        }
        return code;
    }

    int compile_shader(Shader &shader)
    {
        if (!shader.is_compiled)
        {
            shader.id = glCreateShader(shader.type);
            const char *shader_source = shader.source.c_str();
            glShaderSource(shader.id, 1, &shader_source, NULL);
            glCompileShader(shader.id);
            int success;
            char info_log[512];
            glGetShaderiv(shader.id, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader.id, 512, NULL, info_log);
                std::cout << "ERROR::SHADER::" << shader.type << "::COMPILATION_FAILED\n"
                          << info_log << std::endl;
                glDeleteShader(shader.id);
                shader.id = 0;
                return -1;
            }
            shader.is_compiled = true;
        }
        return 0;
    }

    int Shader_Program::link(Shader *vertex_shader, Shader *fragment_shader, Shader *geometry_shader)
    {
        if (program_id == 0)
        {
            program_id = glCreateProgram();
        }
        if (vertex_shader != nullptr)
        {
            glAttachShader(program_id, vertex_shader->id);
        }
        if (fragment_shader != nullptr)
        {
            glAttachShader(program_id, fragment_shader->id);
        }
        if (geometry_shader != nullptr)
        {
            glAttachShader(program_id, geometry_shader->id);
        }
        glLinkProgram(program_id);
        int success;
        char info_log[512];
        glGetProgramiv(program_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program_id, 512, NULL, info_log);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << info_log << std::endl;
            glDeleteProgram(program_id);
            program_id = 0;
            return -1;
        }
        is_linked = true;
        glDetachShader(program_id, vertex_shader->id);
        glDetachShader(program_id, fragment_shader->id);
        if (geometry_shader != nullptr)
        {
            glDetachShader(program_id, geometry_shader->id);
        }
        return 0;
    }

    void Shader_Program::activate()
    {
        glUseProgram(program_id);
    }

    void Shader_Program::deactivate()
    {
        glUseProgram(0);
    }

    void Shader_Program::set_bool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
    }

    void Shader_Program::set_int(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
    }

    void Shader_Program::set_float(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
    }

    void Shader_Program::set_vec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(program_id, name.c_str()), x, y);
    }

    void Shader_Program::set_vec2(const std::string &name, const float *vec) const
    {
        glUniform2fv(glGetUniformLocation(program_id, name.c_str()), 1, vec);
    }

    void Shader_Program::set_vec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(program_id, name.c_str()), x, y, z);
    }

    void Shader_Program::set_vec3(const std::string &name, const float *vec) const
    {
        glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, vec);
    }

    void Shader_Program::set_vec4(const std::string &name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(program_id, name.c_str()), x, y, z, w);
    }

    void Shader_Program::set_vec4(const std::string &name, const float *vec) const
    {
        glUniform4fv(glGetUniformLocation(program_id, name.c_str()), 1, vec);
    }

    void Shader_Program::set_mat2(const std::string &name, const float *mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, mat);
    }

    void Shader_Program::set_mat3(const std::string &name, const float *mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, mat);
    }

    void Shader_Program::set_mat4(const std::string &name, const float *mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, mat);
    }

    Shader *Shader_Program_Factory::find_shader(const std::string &name)
    {
        if (shaders.find(name) != shaders.end())
        {
            return shaders[name].get();
        }
        return nullptr;
    }

    Shader_Program *Shader_Program_Factory::find_shader_program(const std::string &name)
    {
        if (shader_programs.find(name) != shader_programs.end())
        {
            return shader_programs[name].get();
        }
        return nullptr;
    }

    Shader *Shader_Program_Factory::add_shader_from_source(const std::string &source, GLenum type, const std::string &name)
    {
        Shader_Ptr shader = Shader_Ptr(new Shader(source, type));
        if (compile_shader(*shader) == 0)
        {
            shaders[name] = std::move(shader);
            return shaders[name].get();
        }
        return nullptr;
    }

    Shader *Shader_Program_Factory::add_shader_from_file(const std::string &path, GLenum type, const std::string &name)
    {
        std::string code = read_file(path);
        if (code == "")
        {
            return nullptr;
        }
        return add_shader_from_source(code, type, name);
    }

    Shader_Program *Shader_Program_Factory::add_shader_program(const std::string &name, const std::string &vertex_shader_name, const std::string &fragment_shader_name, const std::string &geometry_shader_name)
    {
        Shader *vertex_shader = find_shader(vertex_shader_name);
        Shader *fragment_shader = find_shader(fragment_shader_name);
        Shader *geometry_shader = nullptr;
        if (vertex_shader == nullptr)
        {
            std::cout << "ERROR::SHADER_PROGRAM_FACTORY::ADD_SHADER_PROGRAM::VERTEX_SHADER_NOT_FOUND\n"
                      << "VERTEX_SHADER: " << vertex_shader_name << std::endl;
            return nullptr;
        }
        if (fragment_shader == nullptr)
        {
            std::cout << "ERROR::SHADER_PROGRAM_FACTORY::ADD_SHADER_PROGRAM::FRAGMENT_SHADER_NOT_FOUND\n"
                      << "FRAGMENT_SHADER: " << fragment_shader_name << std::endl;
            return nullptr;
        }

        if (geometry_shader_name != "")
        {
            geometry_shader = find_shader(geometry_shader_name);

            if (geometry_shader == nullptr)
            {
                std::cout << "ERROR::SHADER_PROGRAM_FACTORY::ADD_SHADER_PROGRAM::GEOMETRY_SHADER_NOT_FOUND\n"
                          << "GEOMETRY_SHADER: " << geometry_shader_name << std::endl;
                return nullptr;
            }
        }
        Shader_Program_Ptr shader_program = Shader_Program_Ptr(new Shader_Program(name));
        if (shader_program->link(vertex_shader, fragment_shader, geometry_shader) == 0)
        {
            shader_programs[name] = std::move(shader_program);
            return shader_programs[name].get();
        }
        return nullptr;
    }

    Shader_Program * Shader_Program_Factory::add_shader_program(const std::string &name, Shader *vertex_shader, Shader *fragment_shader, Shader *geometry_shader)
    {
        Shader_Program_Ptr shader_program = Shader_Program_Ptr(new Shader_Program(name));
        if (shader_program->link(vertex_shader, fragment_shader, geometry_shader) == 0)
        {
            shader_programs[name] = std::move(shader_program);
            return shader_programs[name].get();
        }
        return nullptr;
    }

    void Shader_Program_Factory::remove_shader(const std::string &name)
    {
        if (find_shader_program(name) != nullptr)
        {
            shader_programs.erase(name);
        }
    }

    void Shader_Program_Factory::remove_shader_program(const std::string &name)
    {
        if (find_shader_program(name) != nullptr)
        {
            shader_programs.erase(name);
        }
    }

}; // namespace Rendering
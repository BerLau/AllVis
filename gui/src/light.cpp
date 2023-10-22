#include "light.h"

void Rendering::Light::write_to_shader(const std::string &name_, Shader_Program *shader)
{
    shader->set_vec3(name_ + ".position", get_position().data());
    shader->set_vec3(name_ + ".direction", get_direction().data());
    shader->set_vec3(name_ + ".color", get_color().data());
    shader->set_int(name_ + ".type", get_type());
    shader->set_float(name_ + ".intensity", get_intensity());
}

void Rendering::Light::write_to_shader(const std::string &name_, int index, Shader_Program *shader)
{
    auto name_i = name_ + "[" + std::to_string(index) + "]";
    write_to_shader(name_i, shader);
}

#include "light.h"

void Rendering::Light::visualize(Shader_Program *shader)
{
    if (mesh)
    {
        Core::Matrix4 model = transform->get_model_matrix();
        Core::Matrix3 normal_matrix = transform->get_normal_matrix();
        shader->activate();
        shader->set_vec3("u_color", color.data());
        shader->set_mat4("u_model", model.data());
        shader->set_mat3("u_normal_matrix", normal_matrix.data());
        mesh->bind_buffer();
        // enable face culling
        glEnable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
        glDisable(GL_CULL_FACE);
        mesh->unbind_buffer();
    }
}

void Rendering::Light::write_to_shader(const std::string &name_, Shader_Program *shader)
{
    shader->set_vec3(name_ + ".position", get_position().data());
    shader->set_vec3(name_ + ".direction", get_direction().data());
    shader->set_vec3(name_ + ".color", color.data());
    shader->set_int(name_ + ".type", type);
    shader->set_float(name_ + ".intensity", intensity);
}

void Rendering::Light::write_to_shader(const std::string &name_, int index, Shader_Program *shader)
{
    auto name_i = name_ + "[" + std::to_string(index) + "]";
    write_to_shader(name_i, shader);
}

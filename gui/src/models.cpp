#include "models.h"
#include <cstring>

namespace Rendering
{
    Core::Matrix4 OGL_Model::get_model_matrix() const
    {
        Core::Matrix4 model = Core::Matrix4::identity();
        if (transform == nullptr)
        {
            return model;
        }
        else
        {
            model = transform->get_model_matrix();
            return model;
        }
    }
    void OGL_Model::draw(Shader_Program *shader)
    {
        auto mesh_ = get_mesh();
        if (mesh_ == nullptr || shader == nullptr)
        {
            return;
        }
        Core::Matrix4 model = get_model_matrix();
        Core::Matrix3 normal_matrix = transform->get_normal_matrix();
        shader->activate();
        // material->bind();
        // material->write_to_shader("u_material", shader);
        shader->set_mat4("u_model", model.data());
        shader->set_mat3("u_normal_matrix", normal_matrix.data());
        mesh_->bind_buffer();
        // enable face culling
        glEnable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, mesh_->index_count, GL_UNSIGNED_INT, 0);
        glDisable(GL_CULL_FACE);
        mesh_->unbind_buffer();
    }

    void OGL_Model::update()
    {
        get_mesh()->update();
    }

    void OGL_Model::init()
    {
        auto mesh_ = get_mesh();
        if (mesh_ != nullptr)
        {
            mesh_->init();
        }
    }

    // void Sphere_Model::create_mesh()
    // {
    //     // create a sphere mesh with 32 slices and 32 stacks
    //     int slices = 32;
    //     int stacks = 32;
    //     Rendering::Mesh::Layout layout;
    //     layout.add(3, GL_FLOAT, sizeof(float)); // position
    //     layout.add(3, GL_FLOAT, sizeof(float)); // normal
    //     layout.add(3, GL_FLOAT, sizeof(float)); // tangent
    //     layout.add(2, GL_FLOAT, sizeof(float)); // texcoord
    //     std::vector<float> vertices;
    //     std::vector<unsigned int> indices;
    //     for (int i = 0; i <= slices; ++i)
    //     {
    //         float theta = i * M_PI / slices;
    //         float sin_theta = std::sin(theta);
    //         float cos_theta = std::cos(theta);
    //         for (int j = 0; j <= stacks; ++j)
    //         {
    //             float phi = j * 2 * M_PI / stacks;
    //             float sin_phi = std::sin(phi);
    //             float cos_phi = std::cos(phi);

    //             float x = cos_phi * sin_theta;
    //             float y = cos_theta;
    //             float z = sin_theta * sin_phi;
    //             float u = 1 - (float)j / slices;
    //             float v = 1 - (float)i / stacks;

    //             // position
    //             vertices.push_back(x);
    //             vertices.push_back(y);
    //             vertices.push_back(z);

    //             // normal
    //             vertices.push_back(x);
    //             vertices.push_back(y);
    //             vertices.push_back(z);

    //             // tangent : right hand coordinate
    //             vertices.push_back(-sin_phi);
    //             vertices.push_back(0);
    //             vertices.push_back(cos_phi);

    //             // texcoord
    //             vertices.push_back(u);
    //             vertices.push_back(v);
    //         }
    //     }

    //     for (int i = 0; i < slices; ++i)
    //     {
    //         for (int j = 0; j < stacks; ++j)
    //         {
    //             int first = (i * (stacks + 1)) + j;
    //             int second = first + stacks + 1;
    //             indices.push_back(first);
    //             indices.push_back(first + 1);
    //             indices.push_back(second);

    //             indices.push_back(second);
    //             indices.push_back(first + 1);
    //             indices.push_back(second + 1);
    //         }
    //     }

    //     mesh = Rendering::Mesh_Ptr(new Rendering::Mesh(layout, vertices.size() / 11, indices.size()));
    //     memcpy(mesh->vertices, vertices.data(), vertices.size() * sizeof(float));
    //     memcpy(mesh->indices, indices.data(), indices.size() * sizeof(unsigned int));
    // }
};

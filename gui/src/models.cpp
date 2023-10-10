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
            model = Geometry::translate(model, transform->get_position());
            model = model * transform->get_rotation_matrix();
            model = Geometry::scale(model, transform->get_scale());
            return model;
        }
    }
    void OGL_Model::create_vao()
    {
        glGenVertexArrays(1, &vao);
    }

    void OGL_Model::create_vbo()
    {
        glGenBuffers(1, &vbo);
    }

    void OGL_Model::create_ebo()
    {
        glGenBuffers(1, &ebo);
    }

    void OGL_Model::bind_buffer()
    {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    }

    void OGL_Model::unbind_buffer()
    {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Cube_Model::draw()
    {
        if (mesh == nullptr || shader == nullptr)
        {
            return;
        }
        glEnable(GL_DEPTH_TEST);
        Core::Matrix4 model = get_model_matrix();
        Core::Matrix3 normal_matrix = Core::Matrix3(model).inverse().transpose();
        shader->activate();
        shader->set_mat4("u_model", get_model_matrix().data());
        bind_buffer();
        glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
        unbind_buffer();

    }

    void Cube_Model::update()
    {
    }

    void Cube_Model::init()
    {
        create_mesh();
        create_vao();
        create_vbo();
        create_ebo();
        mesh->map_buffers(vao, vbo, ebo);
    }

    void Cube_Model::create_mesh()
    {
        // position data for a cube
        Core::Vector3 vertices[] = {
            // face 1
            {-1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {-1.0f, 1.0f, 1.0f},
            // face 2
            {-1.0f, -1.0f, -1.0f},
            {-1.0f, 1.0f, -1.0f},
            {1.0f, 1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            // face 3
            {-1.0f, 1.0f, -1.0f},
            {-1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, -1.0f},
            // face 4
            {-1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, 1.0f},
            {-1.0f, -1.0f, 1.0f},
            // face 5
            {1.0f, -1.0f, -1.0f},
            {1.0f, 1.0f, -1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f},
            // face 6
            {-1.0f, -1.0f, -1.0f},
            {-1.0f, -1.0f, 1.0f},
            {-1.0f, 1.0f, 1.0f},
            {-1.0f, 1.0f, -1.0f}};

        // normal data for a cube
        Core::Vector3 normals[] = {
            // face 1
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            // face 2
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f},
            // face 3
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            // face 4
            {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            // face 5
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            // face 6
            {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f}};

        Core::Vector3 tangent[] = {
            // face 1
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            // face 2
            {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            // face 3
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f},
            // face 4
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            // face 5
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            // face 6
            {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f}};

        Core::Vector3 bitangent[]{
            Core::Vector3::cross(normals[0], tangent[0]),
            Core::Vector3::cross(normals[1], tangent[1]),
            Core::Vector3::cross(normals[2], tangent[2]),
            Core::Vector3::cross(normals[3], tangent[3]),
            Core::Vector3::cross(normals[4], tangent[4]),
            Core::Vector3::cross(normals[5], tangent[5]),
            Core::Vector3::cross(normals[6], tangent[6]),
            Core::Vector3::cross(normals[7], tangent[7]),
            Core::Vector3::cross(normals[8], tangent[8]),
            Core::Vector3::cross(normals[9], tangent[9]),
            Core::Vector3::cross(normals[10], tangent[10]),
            Core::Vector3::cross(normals[11], tangent[11]),
            Core::Vector3::cross(normals[12], tangent[12]),
            Core::Vector3::cross(normals[13], tangent[13]),
            Core::Vector3::cross(normals[14], tangent[14]),
            Core::Vector3::cross(normals[15], tangent[15]),
            Core::Vector3::cross(normals[16], tangent[16]),
            Core::Vector3::cross(normals[17], tangent[17]),
            Core::Vector3::cross(normals[18], tangent[18]),
            Core::Vector3::cross(normals[19], tangent[19]),
            Core::Vector3::cross(normals[20], tangent[20]),
            Core::Vector3::cross(normals[21], tangent[21]),
            Core::Vector3::cross(normals[22], tangent[22]),
            Core::Vector3::cross(normals[23], tangent[23])};

        // texture coordinate data for a cube
        Core::Vector2 tex_coords[] = {
            // face 1
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f},
            // face 2
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f},
            {0.0f, 0.0f},
            // face 3
            {0.0f, 1.0f},
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            // face 4
            {1.0f, 1.0f},
            {0.0f, 1.0f},
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            // face 5
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f},
            {0.0f, 0.0f},
            // face 6
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f}};

        Rendering::Mesh::Layout layout;
        layout.add(3, GL_FLOAT, sizeof(float));
        layout.add(2, GL_FLOAT, sizeof(float));
        layout.add(3, GL_FLOAT, sizeof(float));
        layout.add(3, GL_FLOAT, sizeof(float));
        layout.add(3, GL_FLOAT, sizeof(float));
        struct Normal
        {
            float x, y, z;
        };
        mesh = Rendering::Mesh_Ptr(new Rendering::Mesh(layout, 24, 36));
        for (int i = 0; i < 24; i++)
        {
            std::memcpy(mesh->vertex<float>(i, 0), vertices[i].data(), vertices[0].bytes());
            std::memcpy(mesh->vertex<float>(i, 1), tex_coords[i].data(), normals[0].bytes());
            std::memcpy(mesh->vertex<float>(i, 2), normals[i].data(), normals[0].bytes());
            std::memcpy(mesh->vertex<float>(i, 3), tangent[i].data(), normals[0].bytes());
            std::memcpy(mesh->vertex<float>(i, 4), bitangent[i].data(), normals[0].bytes());
        }

        for (int i = 0; i < 6; i++)
        {
            mesh->index(i * 6 + 0) = i * 4 + 0;
            mesh->index(i * 6 + 1) = i * 4 + 1;
            mesh->index(i * 6 + 2) = i * 4 + 2;
            mesh->index(i * 6 + 3) = i * 4 + 0;
            mesh->index(i * 6 + 4) = i * 4 + 2;
            mesh->index(i * 6 + 5) = i * 4 + 3;
        }
    }
};

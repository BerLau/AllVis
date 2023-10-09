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
        shader->activate();
        shader->set_mat4("model", get_model_matrix().data());
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
        // create a Cube mesh
        struct Pos
        {
            float x;
            float y;
            float z;
        };

        struct Normal
        {
            float x;
            float y;
            float z;
        };

        struct TexCoord
        {
            float u;
            float v;
        };

        // position data for a cube
        Pos vertices[] = {
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
        Normal normals[] = {
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

        // texture coordinate data for a cube
        TexCoord tex_coords[] = {
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
        layout.add(3, GL_FLOAT, sizeof(float));
        layout.add(2, GL_FLOAT, sizeof(float));
        mesh = Rendering::Mesh_Ptr(new Rendering::Mesh(layout, 24, 36));
        for (int i = 0; i < 24; i++)
        {
            std::memcpy(mesh->vertex<float>(i, 0), &vertices[i], sizeof(Pos));
            std::memcpy(mesh->vertex<float>(i, 1), &normals[i], sizeof(Normal));
            std::memcpy(mesh->vertex<float>(i, 2), &tex_coords[i], sizeof(TexCoord));
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

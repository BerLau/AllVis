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
        Core::Matrix3 normal_matrix = transform->get_normal_matrix();
        shader->activate();
        shader->set_material("u_material", *material.get());
        shader->set_mat4("u_model", model.data());
        shader->set_mat3("u_normal_matrix", normal_matrix.data());
        bind_buffer();
        // enable face culling
        glEnable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
        glDisable(GL_CULL_FACE);
        unbind_buffer();
    }

    void Cube_Model::update()
    {
    }

    void Cube_Model::init()
    {
        material = Rendering::Material_Ptr(new Rendering::Material(Core::Vector3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 1.0f));
        create_mesh();
        create_vao();
        create_vbo();
        create_ebo();
        mesh->map_buffers(vao, vbo, ebo);
    }

    void Cube_Model::create_mesh()
    {
        // Create a cube mesh
        float vertices[] = {
            // front
            -0.5f, -0.5f, 0.5, 0.0f, 0.0f, 1.0, 1.0, 0.0, 0.0, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5, 0.0f, 0.0f, 1.0, 1.0, 0.0, 0.0, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5, 0.0f, 0.0f, 1.0, 1.0, 0.0, 0.0, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5, 0.0f, 0.0f, 1.0, 1.0, 0.0, 0.0, 0.0f, 1.0f,

            // back
            -0.5f, -0.5f, -0.5, 0.0f, 0.0f, -1.0, -1.0, 0.0, 0.0, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5, 0.0f, 0.0f, -1.0, -1.0, 0.0, 0.0, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5, 0.0f, 0.0f, -1.0, -1.0, 0.0, 0.0, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5, 0.0f, 0.0f, -1.0, -1.0, 0.0, 0.0, 1.0f, 0.0f,

            // left
            -0.5f, 0.5f, -0.5, -1.0f, 0.0f, 0.0, 0.0, 1.0, 0.0, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5, -1.0f, 0.0f, 0.0, 0.0, 1.0, 0.0, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5, -1.0f, 0.0f, 0.0, 0.0, 1.0, 0.0, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5, -1.0f, 0.0f, 0.0, 0.0, 1.0, 0.0, 1.0f, 1.0f,

            // right
            0.5f, 0.5f, 0.5, 1.0f, 0.0f, 0.0, 0.0, -1.0, 0.0, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5, 1.0f, 0.0f, 0.0, 0.0, -1.0, 0.0, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5, 1.0f, 0.0f, 0.0, 0.0, -1.0, 0.0, 0.0f, 0.0f,
            0.5f, 0.5f, -0.5, 1.0f, 0.0f, 0.0, 0.0, -1.0, 0.0, 1.0f, 0.0f,

            // top
            -0.5f, 0.5f, -0.5, 0.0f, 1.0f, 0.0, 1.0, 0.0, 0.0, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5, 0.0f, 1.0f, 0.0, 1.0, 0.0, 0.0, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5, 0.0f, 1.0f, 0.0, 1.0, 0.0, 0.0, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5, 0.0f, 1.0f, 0.0, 1.0, 0.0, 0.0, 1.0f, 1.0f,

            // bottom
            -0.5f, -0.5f, -0.5, 0.0f, -1.0f, 0.0, -1.0, 0.0, 0.0, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5, 0.0f, -1.0f, 0.0, -1.0, 0.0, 0.0, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5, 0.0f, -1.0f, 0.0, -1.0, 0.0, 0.0, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5, 0.0f, -1.0f, 0.0, -1.0, 0.0, 0.0, 1.0f, 0.0f};

        unsigned int indices[] = {
            0, 1, 2, 0, 2, 3,       // front
            4, 5, 6, 4, 6, 7,       // back
            8, 9, 10, 8, 10, 11,    // left
            12, 13, 14, 12, 14, 15, // right
            16, 17, 18, 16, 18, 19, // top
            20, 21, 22, 20, 22, 23  // bottom
        };

        Rendering::Mesh::Layout layout;
        layout.add(3, GL_FLOAT, sizeof(float));
        layout.add(3, GL_FLOAT, sizeof(float));
        layout.add(3, GL_FLOAT, sizeof(float));
        layout.add(2, GL_FLOAT, sizeof(float));

        mesh = Rendering::Mesh_Ptr(new Rendering::Mesh(layout, 24, 36));

        memcpy(mesh->vertices, vertices, sizeof(vertices));
        memcpy(mesh->indices, indices, sizeof(indices));
    }
};

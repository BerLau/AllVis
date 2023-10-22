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
        Core::Matrix4 model = get_model_matrix();
        Core::Matrix3 normal_matrix = transform->get_normal_matrix();
        shader->activate();
        material->bind();
        material->write_to_shader("u_material", shader);
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
        name = "Cube_Model";
        material = Material_Ptr(new Material(Core::Vector3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 1.0f));
        create_mesh();
        create_vao();
        create_vbo();
        create_ebo();
        mesh->map_buffers(vao, vbo, ebo);
    }

    void Cube_Model::create_mesh()
    {
        Rendering::Mesh::Layout layout;
        layout.add(3, GL_FLOAT, sizeof(float));
        layout.add(3, GL_FLOAT, sizeof(float));
        layout.add(3, GL_FLOAT, sizeof(float));
        layout.add(2, GL_FLOAT, sizeof(float));
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

        mesh = Rendering::Mesh_Ptr(new Rendering::Mesh(layout, 24, 36));

        memcpy(mesh->vertices, vertices, sizeof(vertices));
        memcpy(mesh->indices, indices, sizeof(indices));
    }

    void Sphere_Model::draw()
    {
        if (mesh == nullptr || shader == nullptr)
        {
            return;
        }
        Core::Matrix4 model = get_model_matrix();
        Core::Matrix3 normal_matrix = transform->get_normal_matrix();
        shader->activate();
        material->bind();
        material->write_to_shader("u_material", shader);
        shader->set_mat4("u_model", model.data());
        shader->set_mat3("u_normal_matrix", normal_matrix.data());
        bind_buffer();
        // enable face culling
        glEnable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
        glDisable(GL_CULL_FACE);
        unbind_buffer();
    }

    void Sphere_Model::update()
    {
    }

    void Sphere_Model::init()
    {
        name = "Sphere_Model";
        material = Material_Ptr(new Material(Core::Vector3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 1.0f));
        create_mesh();
        create_vao();
        create_vbo();
        create_ebo();
        mesh->map_buffers(vao, vbo, ebo);
    }

    void Sphere_Model::create_mesh()
    {
        // create a sphere mesh with 32 slices and 32 stacks
        int slices = 32;
        int stacks = 32;
        Rendering::Mesh::Layout layout;
        layout.add(3, GL_FLOAT, sizeof(float)); // position
        layout.add(3, GL_FLOAT, sizeof(float)); // normal
        layout.add(3, GL_FLOAT, sizeof(float)); // tangent
        layout.add(2, GL_FLOAT, sizeof(float)); // texcoord
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        for (int i = 0; i <= slices; ++i)
        {
            float theta = i * M_PI / slices;
            float sin_theta = std::sin(theta);
            float cos_theta = std::cos(theta);
            for (int j = 0; j <= stacks; ++j)
            {
                float phi = j * 2* M_PI / stacks;
                float sin_phi = std::sin(phi);
                float cos_phi = std::cos(phi);

                float x = cos_phi * sin_theta;
                float y = cos_theta;
                float z = sin_theta * sin_phi;
                float u = 1 - (float)j / slices;
                float v = 1 - (float)i / stacks;

                // position
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                // normal
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                // tangent : right hand coordinate
                vertices.push_back(-sin_phi);
                vertices.push_back(0);
                vertices.push_back(cos_phi);

                // texcoord
                vertices.push_back(u);
                vertices.push_back(v);
            }
        }

        for (int i = 0; i < slices; ++i)
        {
            for (int j = 0; j < stacks; ++j)
            {
                int first = (i * (stacks + 1)) + j;
                int second = first + stacks + 1;
                indices.push_back(first);
                indices.push_back(first + 1);
                indices.push_back(second);

                indices.push_back(second);
                indices.push_back(first + 1);
                indices.push_back(second + 1);
            }
        }

        mesh = Rendering::Mesh_Ptr(new Rendering::Mesh(layout, vertices.size() / 11, indices.size()));
        memcpy(mesh->vertices, vertices.data(), vertices.size() * sizeof(float));
        memcpy(mesh->indices, indices.data(), indices.size() * sizeof(unsigned int));
    }
};

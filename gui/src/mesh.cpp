#include "mesh.h"
#include <cstdarg>
#include <vector>
#include <cstring>
#include <cmath>

namespace Rendering
{
    void OGL_Mesh::create_vao()
    {
        glGenVertexArrays(1, &vao);
    }

    void OGL_Mesh::create_vbo()
    {
        glGenBuffers(1, &vbo);
    }

    void OGL_Mesh::create_ebo()
    {
        glGenBuffers(1, &ebo);
    }

    void OGL_Mesh::bind_buffer()
    {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    }

    void OGL_Mesh::unbind_buffer()
    {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void OGL_Mesh::map_buffers()
    {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * layout.size, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

        int offset = 0;
        for (int i = 0; i < layout.num_segments(); i++)
        {
            Segment &segment = layout[i];
            glVertexAttribPointer(i, segment.count, segment.type, GL_FALSE, layout.size, (void *)offset);
            glEnableVertexAttribArray(i);
            offset += segment.size;
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void OGL_Mesh::init()
    {
        create_vao();
        create_vbo();
        create_ebo();
        bind_buffer();
        map_buffers();
        unbind_buffer();
    }

    void OGL_Mesh::destroy()
    {
        unbind_buffer();
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

    void OGL_Mesh::update()
    {
        bind_buffer();
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_count * layout.size, vertices);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, index_count * sizeof(unsigned int), indices);
        unbind_buffer();
    }

    void OGL_Mesh::render(Shader_Program *shader)
    {
        bind_buffer();
        // glCullFace(GL_FRONT);
        // glDepthFunc(GL_LEQUAL);
        glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
        // glDepthFunc(GL_LESS);
        // glCullFace(GL_BACK);
        unbind_buffer();
    }

    OGL_Mesh_Ptr OGL_Mesh::cube_mesh(float width, float height, float depth)
    {
        Rendering::Mesh::Layout layout;
        layout.add(3, GL_FLOAT, sizeof(float));
        layout.add(3, GL_FLOAT, sizeof(float));
        layout.add(3, GL_FLOAT, sizeof(float));
        layout.add(2, GL_FLOAT, sizeof(float));

        float pos_x = width / 2.0f;
        float pos_y = height / 2.0f;
        float pos_z = depth / 2.0f;

        float neg_x = -pos_x;
        float neg_y = -pos_y;
        float neg_z = -pos_z;

        float norm_fr[] = {0.0f, 0.0f, 1.0f};
        float norm_ba[] = {0.0f, 0.0f, -1.0f};
        float norm_le[] = {-1.0f, 0.0f, 0.0f};
        float norm_ri[] = {1.0f, 0.0f, 0.0f};
        float norm_to[] = {0.0f, 1.0f, 0.0f};
        float norm_bo[] = {0.0f, -1.0f, 0.0f};

        float tan_fr[] = {1.0f, 0.0f, 0.0f};
        float tan_ba[] = {-1.0f, 0.0f, 0.0f};
        float tan_le[] = {0.0f, 0.0f, -1.0f};
        float tan_ri[] = {0.0f, 0.0f, 1.0f};
        float tan_to[] = {1.0f, 0.0f, 0.0f};
        float tan_bo[] = {1.0f, 0.0f, 0.0f};

        float vertices[] = {
            // layout pos.x, pos.y, pos.z, norm.x, norm.y, norm.z, tan.x, tan.y, tan.z, tex.u, tex.v

            // front
            neg_x, neg_y, pos_z, norm_fr[0], norm_fr[1], norm_fr[2], tan_fr[0], tan_fr[1], tan_fr[2], 0.0f, 0.0f,
            pos_x, neg_y, pos_z, norm_fr[0], norm_fr[1], norm_fr[2], tan_fr[0], tan_fr[1], tan_fr[2], 1.0f, 0.0f,
            pos_x, pos_y, pos_z, norm_fr[0], norm_fr[1], norm_fr[2], tan_fr[0], tan_fr[1], tan_fr[2], 1.0f, 1.0f,
            neg_x, pos_y, pos_z, norm_fr[0], norm_fr[1], norm_fr[2], tan_fr[0], tan_fr[1], tan_fr[2], 0.0f, 1.0f,

            // back
            neg_x, neg_y, neg_z, norm_ba[0], norm_ba[1], norm_ba[2], tan_ba[0], tan_ba[1], tan_ba[2], 0.0f, 0.0f,
            neg_x, pos_y, neg_z, norm_ba[0], norm_ba[1], norm_ba[2], tan_ba[0], tan_ba[1], tan_ba[2], 0.0f, 1.0f,
            pos_x, pos_y, neg_z, norm_ba[0], norm_ba[1], norm_ba[2], tan_ba[0], tan_ba[1], tan_ba[2], 1.0f, 1.0f,
            pos_x, neg_y, neg_z, norm_ba[0], norm_ba[1], norm_ba[2], tan_ba[0], tan_ba[1], tan_ba[2], 1.0f, 0.0f,

            // left
            neg_x, pos_y, neg_z, norm_le[0], norm_le[1], norm_le[2], tan_le[0], tan_le[1], tan_le[2], 1.0f, 0.0f,
            neg_x, neg_y, neg_z, norm_le[0], norm_le[1], norm_le[2], tan_le[0], tan_le[1], tan_le[2], 0.0f, 0.0f,
            neg_x, neg_y, pos_z, norm_le[0], norm_le[1], norm_le[2], tan_le[0], tan_le[1], tan_le[2], 0.0f, 1.0f,
            neg_x, pos_y, pos_z, norm_le[0], norm_le[1], norm_le[2], tan_le[0], tan_le[1], tan_le[2], 1.0f, 1.0f,

            // right
            pos_x, pos_y, pos_z, norm_ri[0], norm_ri[1], norm_ri[2], tan_ri[0], tan_ri[1], tan_ri[2], 1.0f, 0.0f,
            pos_x, neg_y, pos_z, norm_ri[0], norm_ri[1], norm_ri[2], tan_ri[0], tan_ri[1], tan_ri[2], 0.0f, 0.0f,
            pos_x, neg_y, neg_z, norm_ri[0], norm_ri[1], norm_ri[2], tan_ri[0], tan_ri[1], tan_ri[2], 0.0f, 1.0f,
            pos_x, pos_y, neg_z, norm_ri[0], norm_ri[1], norm_ri[2], tan_ri[0], tan_ri[1], tan_ri[2], 1.0f, 1.0f,

            // top
            neg_x, pos_y, pos_z, norm_to[0], norm_to[1], norm_to[2], tan_to[0], tan_to[1], tan_to[2], 0.0f, 1.0f,
            pos_x, pos_y, pos_z, norm_to[0], norm_to[1], norm_to[2], tan_to[0], tan_to[1], tan_to[2], 1.0f, 1.0f,
            pos_x, pos_y, neg_z, norm_to[0], norm_to[1], norm_to[2], tan_to[0], tan_to[1], tan_to[2], 1.0f, 0.0f,
            neg_x, pos_y, neg_z, norm_to[0], norm_to[1], norm_to[2], tan_to[0], tan_to[1], tan_to[2], 0.0f, 0.0f,

            // bottom
            neg_x, neg_y, neg_z, norm_bo[0], norm_bo[1], norm_bo[2], tan_bo[0], tan_bo[1], tan_bo[2], 0.0f, 1.0f,
            pos_x, neg_y, neg_z, norm_bo[0], norm_bo[1], norm_bo[2], tan_bo[0], tan_bo[1], tan_bo[2], 1.0f, 1.0f,
            pos_x, neg_y, pos_z, norm_bo[0], norm_bo[1], norm_bo[2], tan_bo[0], tan_bo[1], tan_bo[2], 1.0f, 0.0f,
            neg_x, neg_y, pos_z, norm_bo[0], norm_bo[1], norm_bo[2], tan_bo[0], tan_bo[1], tan_bo[2], 0.0f, 0.0f};

        unsigned int indices[] = {
            0, 1, 2, 0, 2, 3,       // front
            4, 5, 6, 4, 6, 7,       // back
            8, 9, 10, 8, 10, 11,    // left
            12, 13, 14, 12, 14, 15, // right
            16, 17, 18, 16, 18, 19, // top
            20, 21, 22, 20, 22, 23  // bottom
        };

        auto mesh = OGL_Mesh_Ptr(new OGL_Mesh(layout, 24, 36));
        memcpy(mesh->vertices, vertices, sizeof(vertices));
        memcpy(mesh->indices, indices, sizeof(indices));
        mesh->init();
        return mesh;
    }

    OGL_Mesh_Ptr OGL_Mesh::sphere_mesh(float radius, unsigned int slices, unsigned int stacks)
    {
        Mesh::Layout layout;
        layout.add(3, GL_FLOAT, sizeof(float)); // postion
        layout.add(3, GL_FLOAT, sizeof(float)); // normal
        layout.add(3, GL_FLOAT, sizeof(float)); // tangent
        layout.add(2, GL_FLOAT, sizeof(float)); // texture

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i <= slices; ++i)
        {
            float theta = i * M_PI / slices;
            float sin_theta = std::sin(theta);
            float cos_theta = std::cos(theta);
            for (unsigned int j = 0; j <= stacks; ++j)
            {
                float phi = j * 2 * M_PI / stacks;
                float sin_phi = std::sin(phi);
                float cos_phi = std::cos(phi);

                float x = cos_phi * sin_theta;
                float y = cos_theta;
                float z = sin_phi * sin_theta;

                float u = 1 - (float)i / slices;
                float v = 1 - (float)j / stacks;

                // push positions
                vertices.push_back(x * radius);
                vertices.push_back(y * radius);
                vertices.push_back(z * radius);
                // push normals
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                // push tangents
                vertices.push_back(-sin_phi);
                vertices.push_back(0);
                vertices.push_back(cos_phi);
                // push texture
                vertices.push_back(u);
                vertices.push_back(v);
            }
        }

        for (int i = 0; i < slices; i++)
        {
            for (int j = 0; j < stacks; j++)
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

        auto mesh = OGL_Mesh_Ptr(new OGL_Mesh(layout, vertices.size() / layout.element_count(), indices.size()));
        memcpy(mesh->vertices, vertices.data(), vertices.size() * sizeof(float));
        memcpy(mesh->indices, indices.data(), indices.size() * sizeof(unsigned int));
        mesh->init();
        return mesh;
    }

    OGL_Mesh_Ptr OGL_Mesh::quad_mesh(float width, float height)
    {
        Mesh::Layout layout;
        layout.add(2, GL_FLOAT, sizeof(float)); // postion
        layout.add(2, GL_FLOAT, sizeof(float)); // texture

        float pos_x = width / 2.0f;
        float pos_y = height / 2.0f;
        float neg_x = -pos_x;
        float neg_y = -pos_y;

        float vertices[] = {
            // layout pos.x, pos.y, tex.u, tex.v
            neg_x, neg_y, 0.0f, 0.0f,
            pos_x, neg_y, 1.0f, 0.0f,
            pos_x, pos_y, 1.0f, 1.0f,
            neg_x, pos_y, 0.0f, 1.0f};
        
        unsigned int indices[] = {
            0, 1, 2, 0, 2, 3
        };

        auto mesh = OGL_Mesh_Ptr(new OGL_Mesh(layout, 4, 6));
        memcpy(mesh->vertices, vertices, sizeof(vertices));
        memcpy(mesh->indices, indices, sizeof(indices));
        mesh->init();
        return mesh;
    }

    OGL_Mesh *OGL_Mesh::instanced_cube_mesh()
    {
        static OGL_Mesh *mesh = nullptr;
        if (!mesh)
        {
            mesh = OGL_Mesh::cube_mesh(1, 1, 1).release();
        }
        return mesh;
    }

    OGL_Mesh *OGL_Mesh::instanced_sphere_mesh()
    {
        static OGL_Mesh *mesh = nullptr;
        if (!mesh)
        {
            mesh = OGL_Mesh::sphere_mesh().release();
        }
        return mesh;
    }
    OGL_Mesh *OGL_Mesh::instanced_quad_mesh()
    {
        static OGL_Mesh *mesh = nullptr;
        if (!mesh)
        {
            mesh = OGL_Mesh::quad_mesh(2.f, 2.f).release();
        }
        return mesh;
    }
};
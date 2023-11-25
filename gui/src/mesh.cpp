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
        glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        unsigned int offset = 0;
        for (int i = 0; i < layout.count(); i++)
        {
            Segment &segment = layout.segments[i];
            glVertexAttribPointer(i, segment.count, segment.element_type, GL_FALSE, layout.size(), (void *)offset);
            glEnableVertexAttribArray(i);
            offset += segment.size();
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void OGL_Mesh::setup_buffers()
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
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size(), vertices.data());
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), indices.data());
        unbind_buffer();
    }

    void OGL_Mesh::render(Shader_Program *shader)
    {
        bind_buffer();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        unbind_buffer();
    }

    OGL_Mesh_Ptr OGL_Mesh::cube_mesh(float width, float height, float depth)
    {
        Rendering::Mesh::Layout layout;

        layout.add_segment(GL_FLOAT, sizeof(float), 3); // postion
        layout.add_segment(GL_FLOAT, sizeof(float), 3); // normal
        layout.add_segment(GL_FLOAT, sizeof(float), 3); // tangent
        layout.add_segment(GL_FLOAT, sizeof(float), 2); // texture

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

        auto mesh = OGL_Mesh_Ptr(new OGL_Mesh(layout));

        mesh->append_vertex<float>(vertices, sizeof(vertices) / sizeof(float));

        mesh->append_index(indices, sizeof(indices) / sizeof(unsigned int));
        mesh->setup_buffers();
        return mesh;
    }

    OGL_Mesh_Ptr OGL_Mesh::plane_mesh(float width, float height)
    {
        // build a width_segments x height_segments grid
        Rendering::Mesh::Layout layout;
        layout.add_segment(GL_FLOAT, sizeof(float), 3); // postion
        layout.add_segment(GL_FLOAT, sizeof(float), 3); // normal
        layout.add_segment(GL_FLOAT, sizeof(float), 3); // tangent
        layout.add_segment(GL_FLOAT, sizeof(float), 2); // texture

        size_t vertex_count = 4;
        size_t index_count = 6;

        auto mesh = OGL_Mesh_Ptr(new OGL_Mesh(layout));

        float pos_x = width / 2.0f;
        float pos_y = height / 2.0f;
        float neg_x = -pos_x;
        float neg_y = -pos_y;

        float norm[3] = {0.0f, 0.0f, 1.0f};
        float tan[3] = {1.0f, 0.0f, 0.0f};

        float vertices[] = {
            // layout pos.x, pos.y, pos.z, norm.x, norm.y, norm.z, tan.x, tan.y, tan.z, tex.u, tex.v
            neg_x, neg_y, 0.0f, norm[0], norm[1], norm[2], tan[0], tan[1], tan[2], 0.0f, 0.0f,
            pos_x, neg_y, 0.0f, norm[0], norm[1], norm[2], tan[0], tan[1], tan[2], 1.0f, 0.0f,
            pos_x, pos_y, 0.0f, norm[0], norm[1], norm[2], tan[0], tan[1], tan[2], 1.0f, 1.0f,
            neg_x, pos_y, 0.0f, norm[0], norm[1], norm[2], tan[0], tan[1], tan[2], 0.0f, 1.0f};

        mesh->append_vertex<float>(vertices, sizeof(vertices) / sizeof(float));

        unsigned int indices[] = {0, 1, 2, 0, 2, 3};
        mesh->append_index(indices, index_count);
        mesh->setup_buffers();
        return mesh;
    }

    OGL_Mesh_Ptr OGL_Mesh::grid_mesh(float width, float height, unsigned int width_segments, unsigned int height_segments)
    {
        Rendering::Mesh::Layout layout;
        layout.add_segment(GL_FLOAT, sizeof(float), 3); // postion
        layout.add_segment(GL_FLOAT, sizeof(float), 3); // normal
        layout.add_segment(GL_FLOAT, sizeof(float), 3); // tangent
        layout.add_segment(GL_FLOAT, sizeof(float), 2); // texture

        auto mesh = OGL_Mesh_Ptr(new OGL_Mesh(layout));

        float normal[3] = {0.0f, 0.0f, 1.0f};
        float tangent[3] = {1.0f, 0.0f, 0.0f};
        float dx = width / width_segments;
        float dy = height / height_segments;
        float du = 1.0f / width_segments;
        float dv = 1.0f / height_segments;

        for (unsigned int i = 0; i <= height_segments; ++i)
        {
            for (unsigned int j = 0; j <= width_segments; ++j)
            {
                float x = -width / 2.0f + j * dx;
                float y = -height / 2.0f + i * dy;
                float u = j * du;
                float v = i * dv;

                float pos[3] = {x, y, 0.0f};
                float norm[3] = {normal[0], normal[1], normal[2]};
                float tan[3] = {tangent[0], tangent[1], tangent[2]};
                float tex[2] = {u, v};

                mesh->append_vertex_attr(0, pos);
                mesh->append_vertex_attr(1, norm);
                mesh->append_vertex_attr(2, tan);
                mesh->append_vertex_attr(3, tex);
            }
        }
        std::cout << "vertices.size() = " << mesh->vertex_count() << std::endl;

        // Create indices for triangles
        for (unsigned int i = 0; i < height_segments; ++i)
        {
            for (unsigned int j = 0; j < width_segments; ++j)
            {
                unsigned int topLeft = i * (width_segments + 1) + j;
                unsigned int topRight = topLeft + 1;
                unsigned int bottomLeft = (i + 1) * (width_segments + 1) + j;
                unsigned int bottomRight = bottomLeft + 1;

                // // First triangle
                // indices.push_back(topLeft);
                // indices.push_back(bottomLeft);
                // indices.push_back(topRight);

                // // Second triangle
                // indices.push_back(topRight);
                // indices.push_back(bottomLeft);
                // indices.push_back(bottomRight);

                mesh->append_index(topLeft);
                mesh->append_index(bottomLeft);
                mesh->append_index(topRight);

                mesh->append_index(topRight);
                mesh->append_index(bottomLeft);
                mesh->append_index(bottomRight);
            }
        }
        std::cout << "indices.size() = " << mesh->index_count() << std::endl;
        mesh->setup_buffers();
        return mesh;
    }

    OGL_Mesh_Ptr OGL_Mesh::circle_mesh(float radius, unsigned int segments)
    {
        Rendering::Mesh::Layout layout;
        layout.add_segment(GL_FLOAT, sizeof(float), 3); // postion
        layout.add_segment(GL_FLOAT, sizeof(float), 3); // normal
        layout.add_segment(GL_FLOAT, sizeof(float), 3); // tangent
        layout.add_segment(GL_FLOAT, sizeof(float), 2); // texture

        size_t vertex_count = segments + 1;
        size_t index_count = (segments + 1) * 3;

        auto mesh = OGL_Mesh_Ptr(new OGL_Mesh(layout, vertex_count, index_count));

        // center point
        float pos[3] = {0.0f, 0.0f, 0.0f};
        float norm[3] = {0.0f, 0.0f, 1.0f};
        float tan[3] = {1.0f, 0.0f, 0.0f};
        float tex[2] = {0.5, 0.5};
        mesh->set_vertex_attr<float>(0, 0, pos);
        mesh->set_vertex_attr<float>(0, 1, norm);
        mesh->set_vertex_attr<float>(0, 2, tan);
        mesh->set_vertex_attr<float>(0, 3, tex);

        float angle = 2 * M_PI / segments;
        for (int i = 0; i < segments; i++)
        {
            float x = std::cos(angle * i) * radius;
            float y = std::sin(angle * i) * radius;
            float u = std::cos(angle * i) * 0.5 + 0.5;
            float v = std::sin(angle * i) * 0.5 + 0.5;

            float pos[3] = {x, y, 0.0f};
            float norm[3] = {0.0f, 0.0f, 1.0f};
            float tan[3] = {1.0f, 0.0f, 0.0f};
            float tex[2] = {u, v};
            mesh->set_vertex_attr<float>(i + 1, 0, pos);
            mesh->set_vertex_attr<float>(i + 1, 1, norm);
            mesh->set_vertex_attr<float>(i + 1, 2, tan);
            mesh->set_vertex_attr<float>(i + 1, 3, tex);
        }

        for (int i = 0; i < segments; i++)
        {
            unsigned int indices[3] = {0, i + 1u, i + 2u};
            mesh->set_index(i * 3 + 0, indices[0]);
            mesh->set_index(i * 3 + 1, indices[1]);
            mesh->set_index(i * 3 + 2, indices[2]);
        }

        // close the circle
        // mesh->indices[segments * 3 + 0] = 0;
        // mesh->indices[segments * 3 + 1] = segments;
        // mesh->indices[segments * 3 + 2] = 1;
        mesh->set_index(segments * 3 + 0, 0);
        mesh->set_index(segments * 3 + 1, segments);
        mesh->set_index(segments * 3 + 2, 1);

        mesh->setup_buffers();
        return mesh;
    }

    OGL_Mesh_Ptr OGL_Mesh::sphere_mesh(float radius, unsigned int slices, unsigned int stacks)
    {
        Mesh::Layout layout;
        layout.add_segment(GL_FLOAT, sizeof(float), 3); // postion
        layout.add_segment(GL_FLOAT, sizeof(float), 3); // normal
        layout.add_segment(GL_FLOAT, sizeof(float), 3); // tangent
        layout.add_segment(GL_FLOAT, sizeof(float), 2); // texture
        auto mesh = OGL_Mesh_Ptr(new OGL_Mesh(layout));

        std::vector<float> vertex;
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
                vertex.push_back(x * radius);
                vertex.push_back(y * radius);
                vertex.push_back(z * radius);
                // push normals
                vertex.push_back(x);
                vertex.push_back(y);
                vertex.push_back(z);
                // push tangents
                vertex.push_back(-sin_phi);
                vertex.push_back(0);
                vertex.push_back(cos_phi);
                // push texture
                vertex.push_back(u);
                vertex.push_back(v);
            }
        }

        mesh->append_vertex(vertex.data(), vertex.size());

        for (int i = 0; i < slices; i++)
        {
            for (int j = 0; j < stacks; j++)
            {
                static unsigned int batch[6];
                unsigned int first = (i * (stacks + 1)) + j;
                unsigned int second = first + stacks + 1;
                batch[0] = first;
                batch[1] = first + 1;
                batch[2] = second;
                batch[3] = second;
                batch[4] = first + 1;
                batch[5] = second + 1;
                mesh->append_index(batch, 6);
            }
        }

        mesh->setup_buffers();
        return mesh;
    }

    OGL_Mesh_Ptr OGL_Mesh::quad_mesh(float width, float height)
    {
        Mesh::Layout layout;
        layout.add_segment(GL_FLOAT, sizeof(float), 2); // postion
        layout.add_segment(GL_FLOAT, sizeof(float), 2); // texture
        auto mesh = OGL_Mesh_Ptr(new OGL_Mesh(layout));

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
            0, 1, 2, 0, 2, 3};
        mesh->append_vertex(vertices, sizeof(vertices) / sizeof(float));
        mesh->append_index(indices, sizeof(indices) / sizeof(unsigned int));
        mesh->setup_buffers();
        return mesh;
    }

    OGL_Mesh *OGL_Mesh::instanced_cube_mesh()
    {
        static OGL_Mesh *mesh = nullptr;
        if (!mesh)
        {
            mesh = OGL_Mesh::cube_mesh(2, 2, 2).release();
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

    void Mesh::resize(unsigned int vertex_count, unsigned int index_count)
    {
        vertices.resize(layout.size() * vertex_count);
        indices.resize(index_count);
    }

    void Mesh::reserve(unsigned int vertex_count, unsigned int index_count)
    {
        vertices.reserve(layout.size() * vertex_count);
        indices.reserve(index_count);
    }

    void Mesh::clear()

    {
        vertices.clear();
        indices.clear();
    }

    size_t Mesh::Layout::size() const
    {
        size_t rslt = 0;
        for (auto &segment : segments)
        {
            rslt += segment.size();
        }
        return rslt;
    }
};
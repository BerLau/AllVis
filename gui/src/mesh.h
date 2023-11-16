#pragma once
#ifndef RENDERING_MESH_H
#define RENDERING_MESH_H

#include <glad/glad.h>
#include <vector>
#include <memory>
#include "shader.h"
#include <typeinfo>
#include <iostream>

namespace Rendering
{

    class Mesh;
    using Mesh_U_Ptr = std::unique_ptr<Mesh>;
    using Mesh_S_Ptr = std::shared_ptr<Mesh>;
    using Mesh_W_Ptr = std::weak_ptr<Mesh>;
    using Mesh_Ptr = Mesh_U_Ptr;

    class Mesh
    {
    public:
        // Segment has 3 attributes: the number of the elements, the type of the elements, and the size of the type
        struct Segment
        {
            const unsigned int element_type;
            const unsigned int element_size;
            const unsigned int count;

            constexpr Segment(unsigned int element_type, unsigned int element_size, unsigned int count) : element_type(element_type), element_size(element_size), count(count)
            {
            }
            constexpr Segment() : element_type(0), element_size(0), count(0)
            {
            }
            constexpr size_t size() const
            {
                return count * element_size;
            }
        };

        class Layout
        {
            // attributes
        public:
            std::vector<Segment> segments;
            Layout() {}
            ~Layout() {}
            // methods
        public:
            void add_segment(unsigned int element_type, unsigned int element_size, unsigned int count)
            {
                segments.emplace_back(element_type, element_size, count);
            }
            void add_segment(const Segment &segment)
            {
                segments.push_back(segment);
            }

            size_t size() const
            {
                size_t rslt = 0;
                for (auto &segment : segments)
                {
                    rslt += segment.size();
                }
                return rslt;
            }

            size_t count() const
            {
                return segments.size();
            }

            const Segment &operator[](unsigned int index) const
            {
                return segments[index];
            }
        };
        // attributes

    public:
        std::vector<char> vertices;
        std::vector<unsigned int> indices;
        Layout layout;

    private:
        // constructors and deconstructor
    public:
        Mesh(Layout layout)
            : layout(layout)
        {
        }
        Mesh(Layout layout, unsigned int vertex_count, unsigned int index_count)
            : layout(layout)
        {
            vertices.resize(layout.size() * vertex_count);
            indices.resize(index_count);
        }
        virtual ~Mesh()
        {
        }
        // methods
    public:
        void resize(unsigned int vertex_count, unsigned int index_count)
        {
            vertices.resize(layout.size() * vertex_count);
            indices.resize(index_count);
        }
        void expand(unsigned int vertex_count, unsigned int index_count)
        {
            vertices.reserve(layout.size() * vertex_count);
            indices.reserve(index_count);
        }
        void add_vertices(void *data, size_t size)
        {
            vertices.insert(vertices.end(), (char *)data, (char *)data + size);
        }
        void add_indices(unsigned int *data, size_t size)
        {
            indices.insert(indices.end(), data, data + size);
        }

        void add_vertex(void *data, size_t size)
        {
            if (size == layout.size())
            {
                vertices.insert(vertices.end(), (char *)data, (char *)data + size);
            }
            else
            {
                std::cerr << "Error: size of data is not equal to size of layout" << std::endl;
            }
        }

        void add_index(unsigned int data)
        {
            indices.push_back(data);
        }

        void *vertex(unsigned int index)
        {
            return &vertices[index * layout.size()];
        }

        template <typename T>
        T *vertex_attr(unsigned int index, unsigned int offset)
        {
            if (offset + sizeof(T) > layout.size())
            {
                std::cerr << "Error: offset + sizeof(T) > layout.size()" << std::endl;
                return nullptr;
            }
            return (T *)vertex(index) + offset;
        }
        // static methods
    };

    class OGL_Mesh;
    using OGL_Mesh_U_Ptr = std::unique_ptr<OGL_Mesh>;
    using OGL_Mesh_S_Ptr = std::shared_ptr<OGL_Mesh>;
    using OGL_Mesh_W_Ptr = std::weak_ptr<OGL_Mesh>;
    using OGL_Mesh_Ptr = OGL_Mesh_U_Ptr;
    class OGL_Mesh : public Mesh
    {
        // attributes
    public:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;
        // constructors and deconstructor
    public:
        OGL_Mesh(Layout layout)
            : Mesh(layout),
              vao(0),
              vbo(0),
              ebo(0)
        {
        }
        virtual ~OGL_Mesh()
        {
            destroy();
        }
        // methods
    public:
        void create_vao();
        void create_vbo();
        void create_ebo();
        void bind_buffer();
        void unbind_buffer();
        void map_buffers();
        void init();
        void destroy();
        void update();
        void render(Shader_Program *shader);
        // static methods
    public:
        static OGL_Mesh_Ptr cube_mesh(float width = 1.0f, float height = 1.0f, float depth = 1.0f);
        static OGL_Mesh_Ptr plane_mesh(float width = 1.0f, float height = 1.0f, unsigned int width_segments = 1, unsigned int height_segments = 1, float u_tile = 1.0f, float v_tile = 1.0f);
        static OGL_Mesh_Ptr sphere_mesh(float radius = 1.0f, unsigned int slices = 32, unsigned int stacks = 32);
        static OGL_Mesh_Ptr cylinder_mesh(float radius = 1.0f, float height = 1.0f, unsigned int radial_segments = 32, unsigned int height_segments = 1);
        static OGL_Mesh_Ptr cone_mesh(float radius = 1.0f, float height = 1.0f, unsigned int radial_segments = 32, unsigned int height_segments = 1);
        static OGL_Mesh_Ptr torus_mesh(float radius = 1.0f, float tube_radius = 0.5f, unsigned int radial_segments = 32, unsigned int tubular_segments = 32);
        static OGL_Mesh_Ptr quad_mesh(float width = 1.0f, float height = 1.0f);

        static OGL_Mesh *instanced_cube_mesh();
        static OGL_Mesh *instanced_plane_mesh();
        static OGL_Mesh *instanced_sphere_mesh();
        static OGL_Mesh *instanced_cylinder_mesh();
        static OGL_Mesh *instanced_cone_mesh();
        static OGL_Mesh *instanced_torus_mesh();
        static OGL_Mesh *instanced_quad_mesh();
    };

}; // namespace Rendering

#endif // !RENDERING_MESH_H
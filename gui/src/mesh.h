#pragma once
#ifndef RENDERING_MESH_H
#define RENDERING_MESH_H

#include <glad/glad.h>
#include <vector>
#include <memory>
#include "shader.h"
#include <typeinfo>
#include <iostream>
#include <cstring>

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

            constexpr Segment(unsigned int element_type, unsigned int element_size, unsigned int count) : element_type(element_type), element_size(element_size), count(count) {}
            constexpr Segment() : element_type(0), element_size(0), count(0) {}
            constexpr size_t size() const { return count * element_size; }
        };

        class Layout
        {
            // attributes
        public:
            std::vector<Segment> segments;
            std::vector<size_t> prefix_byte_sizes;
            Layout() {}
            ~Layout() {}
            // methods
        public:
            void add_segment(unsigned int element_type, unsigned int element_size, unsigned int count)
            {
                segments.push_back(Segment(element_type, element_size, count));
                prefix_byte_sizes.push_back(prefix_byte_sizes.empty() ? 0 : prefix_byte_sizes.back() + segments.back().size());
            }
            void add_segment(const Segment &segment) { segments.push_back(segment); }
            size_t size() const;
            size_t count() const { return segments.size(); }
            size_t bytes_off(unsigned int index) const { return prefix_byte_sizes[index]; }

            const Segment &operator[](unsigned int index) const { return segments[index]; }
        };
        // attributes

    public:
        std::vector<char> vertices;
        std::vector<unsigned int> indices;
        Layout layout;

    private:
        // constructors and deconstructor
    public:
        Mesh(Layout layout) : layout(layout) {}
        Mesh(Layout layout, unsigned int vertex_count, unsigned int index_count)
            : layout(layout) { resize(vertex_count, index_count); }
        virtual ~Mesh() {}
        // methods
    public:
        void resize(unsigned int vertex_count, unsigned int index_count);
        void reserve(unsigned int vertex_count, unsigned int index_count);

        template <typename T>
        void append_vertex(T *data, size_t count);

        template <typename T>
        void set_vertex(size_t index, T *data);

        void *vertex(unsigned int index) { return &vertices[index * layout.size()]; }

        template <typename T>
        T *vertex_attr(unsigned int index, unsigned int attr_index);

        template <typename T>
        void append_vertex(T *data);

        template <typename T>
        void append_vertex_attr(unsigned int attr_index, T *data);

        template <typename T>
        void set_vertex_attr(unsigned int index, unsigned int attr_index, T *data);

        unsigned int index(unsigned int index) { return indices[index]; }
        void set_index(unsigned int index, unsigned int data) { indices[index] = data; }
        void set_index(unsigned int index, unsigned int *data, unsigned int count) { memcpy(&indices[index], data, count * sizeof(unsigned int)); }
        void append_index(unsigned int data) { indices.push_back(data); }
        void append_index(unsigned int *data, size_t count) { indices.insert(indices.end(), data, data + count); }

        void clear();

        size_t vertex_count() const { return vertices.size() / layout.size(); }
        size_t index_count() const { return indices.size(); }
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
        OGL_Mesh(Layout layout) : Mesh(layout), vao(0), vbo(0), ebo(0) {}
        OGL_Mesh(Layout layout, unsigned int vertex_count, unsigned int index_count)
            : Mesh(layout, vertex_count, index_count), vao(0), vbo(0), ebo(0) {}
        virtual ~OGL_Mesh() { destroy(); }
        // methods
    public:
        void create_vao();
        void create_vbo();
        void create_ebo();
        void bind_buffer();
        void unbind_buffer();
        void map_buffers();
        void setup_buffers();
        void destroy();
        void update();
        void render(Shader_Program *shader);
        // static methods
    public:
        static OGL_Mesh_Ptr cube_mesh(float width = 1.0f, float height = 1.0f, float depth = 1.0f);
        static OGL_Mesh_Ptr plane_mesh(float width = 1.0f, float height = 1.0f);
        static OGL_Mesh_Ptr grid_mesh(float width = 1.0f, float height = 1.0f, unsigned int width_segments = 1, unsigned int height_segments = 1);
        static OGL_Mesh_Ptr circle_mesh(float radius = 1.0f, unsigned int segments = 32);
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

    template <typename T>
    inline void Mesh::append_vertex(T *data, size_t count)
    {
        size_t offset = vertices.size();
        size_t byte_size = count * sizeof(T);
        vertices.resize(offset + byte_size);
        memcpy(&vertices[offset], (void *)data, byte_size);
    }

    template <typename T>
    inline void Mesh::set_vertex(size_t index, T *data)
    {
        size_t offset = index * layout.size();
        memcpy(&vertices[offset], (void *)data, layout.size());
    }

    template <typename T>
    inline T *Mesh::vertex_attr(unsigned int index, unsigned int attr_index)
    {
        if (attr_index >= layout.count())
        {
            std::cerr << "Error: offset + sizeof(T) > layout.size()" << std::endl;
            return nullptr;
        }
        size_t offset = 0;
        for (unsigned int i = 0; i < attr_index; ++i)
        {
            offset += layout[i].size();
        }
        return (T *)&vertices[index * layout.size() + offset];
    }

    template <typename T>
    inline void Mesh::append_vertex(T *data)
    {
        size_t offset = vertices.size();
        vertices.resize(offset + layout.size());
        memcpy(&vertices[offset], (void *)data, layout.size());
    }

    template <typename T>
    inline void Mesh::append_vertex_attr(unsigned int attr_index, T *data)
    {
        if (attr_index >= layout.count())
        {
            std::cerr << "Error: offset + sizeof(T) > layout.size()" << std::endl;
            return;
        }
        size_t offset = vertices.size();
        vertices.resize(offset + layout[attr_index].size());
        memcpy(&vertices[offset], (void *)data, layout[attr_index].size());
    }

    template <typename T>
    inline void Mesh::set_vertex_attr(unsigned int index, unsigned int attr_index, T *data)
    {
        if (attr_index >= layout.count())
        {
            std::cerr << "Error: offset + sizeof(T) > layout.size()" << std::endl;
            return;
        }
        size_t offset = index * layout.size() + layout.bytes_off(attr_index);
        memcpy(&vertices[offset], (void *)data, layout[attr_index].size());
    }

}; // namespace Rendering

#endif // !RENDERING_MESH_H
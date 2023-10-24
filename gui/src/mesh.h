#pragma once
#ifndef RENDERING_MESH_H
#define RENDERING_MESH_H

#include <glad/glad.h>
#include <vector>
#include <memory>

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
            unsigned int count;
            GLenum type;
            unsigned int stride;
            unsigned int size = count * stride;
        };

        class Layout
        {
            // attributes
        public:
            unsigned int size;
            std::vector<Segment> segments;
            Layout() : size(0), segments(std::vector<Segment>()) {}
            ~Layout() {}
            void add(unsigned int count, GLenum type, unsigned int type_size)
            {
                segments.push_back(Segment{count, type, type_size});
                this->size += count * type_size;
            }
            Segment &operator[](unsigned int index)
            {
                return segments[index];
            }
            unsigned int num_segments() const
            {
                return segments.size();
            }

            unsigned int offset(unsigned int index) const
            {
                unsigned int offset = 0;
                for (unsigned int i = 0; i < index; i++)
                {
                    offset += segments[i].size;
                }
                return offset;
            }
            unsigned int stride() const
            {
                unsigned int stride = 0;
                for (unsigned int i = 0; i < segments.size(); i++)
                {
                    stride += segments[i].stride;
                }
                return stride;
            }
            unsigned int element_count() const
            {
                unsigned int element_count = 0;
                for (unsigned int i = 0; i < segments.size(); i++)
                {
                    element_count += segments[i].count;
                }
                return element_count;
            }
        };
        // attributes

    public:
        char *vertices;
        unsigned int *indices;
        unsigned int vertex_count;
        unsigned int index_count;
        Layout layout;

    private:
        // constructors and deconstructor
    public:
        Mesh(Layout layout, unsigned int vertex_count, unsigned int index_count)
            : vertex_count(vertex_count),
              index_count(index_count),
              layout(layout)
        {
            vertices = new char[vertex_count * layout.size];
            indices = new unsigned int[index_count];
        }
        virtual ~Mesh()
        {
            destroy();
        }
        // methods
    public:
        // template function to access vertex by index
        template <typename T>
        T *vertex(unsigned int index)
        {
            return (T *)(vertices + index * layout.size);
        }

        template <typename T>
        const T *vertex(unsigned int index) const
        {
            return (T *)(vertices + index * layout.size);
        }

        // template function to access vertex by index and segment
        template <typename T>
        T *vertex(unsigned int index, unsigned int segment)
        {
            return (T *)(vertices + index * layout.size + layout.offset(segment));
        }

        template <typename T>
        const T *vertex(unsigned int index, unsigned int segment) const
        {
            return (T *)(vertices + index * layout.size + layout.offset(segment));
        }

        // function to access index by index
        unsigned int &index(unsigned int index)
        {
            return indices[index];
        }
        const unsigned int &index(unsigned int index) const
        {
            return indices[index];
        }
        virtual void init() {}
        virtual void destroy()
        {
            delete[] vertices;
            delete[] indices;
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
        OGL_Mesh(Layout layout, unsigned int vertex_count, unsigned int index_count)
            : Mesh(layout, vertex_count, index_count),
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
        // static methods
    public:
        static OGL_Mesh_Ptr cube_mesh(float width = 1.0f, float height = 1.0f, float depth = 1.0f);
        static OGL_Mesh_Ptr plane_mesh(float width = 1.0f, float height = 1.0f, unsigned int width_segments = 1, unsigned int height_segments = 1);
        static OGL_Mesh_Ptr sphere_mesh(float radius = 1.0f, unsigned int slices = 32, unsigned int stacks = 32);
        static OGL_Mesh_Ptr cylinder_mesh(float radius = 1.0f, float height = 1.0f, unsigned int radial_segments = 32, unsigned int height_segments = 1);
        static OGL_Mesh_Ptr cone_mesh(float radius = 1.0f, float height = 1.0f, unsigned int radial_segments = 32, unsigned int height_segments = 1);
        static OGL_Mesh_Ptr torus_mesh(float radius = 1.0f, float tube_radius = 0.5f, unsigned int radial_segments = 32, unsigned int tubular_segments = 32);
        static OGL_Mesh_Ptr quad_mesh(float width = 1.0f, float height = 1.0f);

        static OGL_Mesh* instanced_cube_mesh();
        static OGL_Mesh* instanced_plane_mesh();
        static OGL_Mesh* instanced_sphere_mesh();
        static OGL_Mesh* instanced_cylinder_mesh();
        static OGL_Mesh* instanced_cone_mesh();
        static OGL_Mesh* instanced_torus_mesh();
        static OGL_Mesh* instanced_quad_mesh();

    };

}; // namespace Rendering

#endif // !RENDERING_MESH_H
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
        ~Mesh()
        {
            delete[] vertices;
            delete[] indices;
        }
        // methods
    public:
        void map_buffers(GLuint vao, GLuint vbo, GLuint ebo);

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
        // static methods
    };
    
}; // namespace Rendering

#endif // !RENDERING_MESH_H
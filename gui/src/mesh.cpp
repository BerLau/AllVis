#include "mesh.h"
#include <cstdarg>

#include <vector>

namespace Rendering
{
    void Mesh::map_buffers(GLuint vao, GLuint vbo, GLuint ebo)
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

};
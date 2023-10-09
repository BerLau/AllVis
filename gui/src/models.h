#pragma once
#ifndef MODELS_H
#define MODELS_H

#include <glad/glad.h>
#include <memory>
#include "mesh.h"
#include "transform.h"
#include "shader.h"

namespace Rendering
{
    class OGL_Model;
    using OGLModel_U_Ptr = std::unique_ptr<OGL_Model>;
    using OGLModel_S_Ptr = std::shared_ptr<OGL_Model>;
    using OGLModel_W_Ptr = std::weak_ptr<OGL_Model>;
    using OGLModel_Ptr = OGLModel_U_Ptr;

    class OGL_Model
    {
        // attributes
    public:
        Mesh_Ptr mesh;
        Core::Transform_Ptr transform;

    protected:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;
        // constructors and deconstructor
    public:
        OGL_Model() : mesh(nullptr), transform(nullptr), vao(0), vbo(0), ebo(0) { init(); }
        OGL_Model(Mesh *mesh, Core::Transform *transform) : mesh(mesh), transform(transform), vao(0), vbo(0), ebo(0) {}
        virtual ~OGL_Model() { destroy(); }
        // methods
    public:
        Core::Matrix4 get_model_matrix() const;
        virtual void draw(Shader_Program *shader) = 0;
        virtual void update() = 0;
        virtual void init()
        {
            create_vao();
            create_vbo();
            create_ebo();
        };
        virtual void destroy()
        {
            unbind_buffer();
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &ebo);
        };

    protected:
        void create_vao();
        void create_vbo();
        void create_ebo();
        void bind_buffer();
        void unbind_buffer();
    };

    class Cube_Model : public OGL_Model
    {
        // attributes
    public:
        // constructors and deconstructor
    public:
        Cube_Model() : OGL_Model() {init(); }
        virtual ~Cube_Model() {}
        // methods
    public:
        virtual void draw(Shader_Program *shader);
        virtual void update();
        virtual void init() override;
    private:
        void create_mesh();
    };

} // namespace Rendering

#endif // !MODELS_H
#pragma once
#ifndef MODELS_H
#define MODELS_H

#include <glad/glad.h>
#include <memory>
#include "mesh.h"
#include "shader.h"
#include "core.h"

namespace Rendering
{

    class Model;
    using Model_U_Ptr = std::unique_ptr<Model>;
    using Model_S_Ptr = std::shared_ptr<Model>;
    using Model_W_Ptr = std::weak_ptr<Model>;

    class Model
    {
        // attributes
    public:
        Mesh_Ptr mesh;
        Core::Transform_Ptr transform;
        // constructors and deconstructor
    public:
        Model()
            : mesh(nullptr),
              transform(Core::Transform_Ptr(
                  new Core::Transform()))
        {
        }
        Model(Mesh *mesh, Core::Transform *transform) : mesh(mesh), transform(transform) {}
        virtual ~Model() {}
        // methods
    public:
        virtual void init() = 0;
        virtual void draw() = 0;
        virtual void update() = 0;
        virtual void destroy() = 0;
    };

    class OGL_Model;
    using OGL_Model_U_Ptr = std::unique_ptr<OGL_Model>;
    using OGL_Model_S_Ptr = std::shared_ptr<OGL_Model>;
    using OGL_Model_W_Ptr = std::weak_ptr<OGL_Model>;
    using OGL_Model_Ptr = OGL_Model_U_Ptr;

    class OGL_Model : public Model
    {
        // attributes
    public:
    protected:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;
        Shader_Program *shader;
        // constructors and deconstructor
    public:
        OGL_Model() : Model(), vao(0), vbo(0), ebo(0)
        {
            init();
        }
        OGL_Model(Mesh *mesh, Core::Transform *transform) : Model(), vao(0), vbo(0), ebo(0) {}
        virtual ~OGL_Model() { destroy(); }
        // methods
    public:
        Core::Matrix4 get_model_matrix() const;
        virtual void draw() = 0;
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

        void bind_shader(Shader_Program *shader) { this->shader = shader; }
        void unbind_shader() { this->shader = nullptr; }

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
        Cube_Model() : OGL_Model() { init(); }
        virtual ~Cube_Model() {}
        // methods
    public:
        virtual void draw();
        virtual void update();
        virtual void init() override;

    private:
        void create_mesh();
    };

} // namespace Rendering

#endif // !MODELS_H
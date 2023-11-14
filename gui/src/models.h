#pragma once
#ifndef MODELS_H
#define MODELS_H

#include <glad/glad.h>
#include <memory>
#include "mesh.h"
#include "shader.h"
#include "core.h"
#include "material.h"

namespace Rendering
{

    class Model;
    using Model_U_Ptr = std::unique_ptr<Model>;
    using Model_S_Ptr = std::shared_ptr<Model>;
    using Model_W_Ptr = std::weak_ptr<Model>;

    class Model : virtual public Core::Configurable
    {
        // attributes
    public:
        Core::Transform_Ptr transform;

    private:
        Mesh_Ptr mesh;
        // constructors and deconstructor
    public:
        Model(const std::string &name = "Model", Mesh_Ptr mesh = nullptr, Core::Transform_Ptr transform = Core::Transform_Ptr(new Core::Transform()))
            : Configurable(name),
              mesh(std::move(mesh)),
              transform(std::move(transform))
        {
        }
        virtual ~Model() {}
        // methods
    public:
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void destroy() = 0;
        virtual Mesh *get_mesh() const { return mesh.get(); }
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
        Material_PBR_Ptr material = nullptr;

        // constructors and deconstructor
    public:
        OGL_Model(const std::string &name = "OGL_Model", OGL_Mesh_Ptr mesh = nullptr, Core::Transform_Ptr transform = Core::Transform_Ptr(new Core::Transform()), Material_PBR_Ptr material = Material_PBR_Ptr(new Material_PBR()))
            : Configurable(name),
              Model(name, Mesh_Ptr(std::move(mesh)), std::move(transform)),
              material(material) { init(); }
        virtual ~OGL_Model() { destroy(); }
        // methods
    public:
        virtual void draw(Shader_Program *shader);
        virtual void update();
        virtual void init();
        virtual void destroy() {}
        Core::Matrix4 get_model_matrix() const;
        virtual OGL_Mesh *get_mesh() const { return dynamic_cast<OGL_Mesh *>(Model::get_mesh()); }
    };

} // namespace Rendering

#endif // !MODELS_H
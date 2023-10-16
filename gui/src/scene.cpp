#include "scene.h"

namespace Rendering
{
    void OGL_Scene::create_framebuffer()
    {
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

        glGenTextures(1, &this->fb_tex);
        glBindTexture(GL_TEXTURE_2D, this->fb_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, properties->width, properties->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->fb_tex, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OGL_Scene::resize_framebuffer()
    {
        glBindTexture(GL_TEXTURE_2D, this->fb_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, properties->width, properties->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OGL_Scene::bind_framebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    }

    void OGL_Scene::unbind_framebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OGL_Scene::release_framebuffer()
    {
        if (this->fbo)
        {
            glDeleteFramebuffers(1, &this->fbo);
        }
        if (this->fb_tex)
        {
            glDeleteTextures(1, &this->fb_tex);
        }
    }

    OGL_Scene_3D::OGL_Scene_3D(float width, float height, Properties *properties)
        : OGL_Scene(width, height, properties)
    {
        camera = Rendering::Camera_Ptr(new Rendering::Camera(Core::Vector3(0.0f, 2.0f, 4.0f)));
        camera->focus_on(Core::Vector3(0.f, 0.0f, 0.0f), Core::Vector3(0.0f, 1.0f, 0.0f));

        init();
    }

    OGL_Scene_3D::~OGL_Scene_3D()
    {
        destroy();
    }

    void OGL_Scene_3D::init()
    {
        cube_model = Rendering::OGL_Model_U_Ptr(new Rendering::Cube_Model());
        // set light
        light = Rendering::Light_Ptr(new Rendering::Light());
        light->set_position(Core::Vector3(2.0f, 3.0f, 3.0f));
        // light->set_direction(Core::Vector3(-1.0f, -1.0f, -1.0f));
        light->set_color(Core::Vector3(0.75f, 0.75f, 0.75f));
        light->set_type(Rendering::Light::POINT_LIGHT);
    }
    void OGL_Scene_3D::update()
    {
        // TODO: update scene
    }
    void OGL_Scene_3D::destroy()
    {
        // TODO: destroy scene
    }
    void OGL_Scene_3D::render()
    {
        using namespace Core;
        bind_framebuffer();
        auto props = get_properties();
        // temporarily set the light properties
        glViewport(0, 0, properties->width, properties->height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Core::Matrix4 projection = Geometry::perspective(props->fov, properties->width / properties->height, props->near, props->far);
        Core::Matrix4 view = camera->get_view_matrix();
        shader->activate();
        shader->set_mat4("u_view", view.data());
        shader->set_mat4("u_projection", projection.data());
        shader->set_light("u_light", *light.get());
        shader->set_vec3("u_view_position", Vector3(0.0f, 2.0f, 4.0f).data());
        cube_model->bind_shader(shader);
        cube_model->draw();
        unbind_framebuffer();
        cube_model->unbind_shader();
    }
};

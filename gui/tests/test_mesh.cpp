#include <gtest/gtest.h>
#include <gui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct Pos
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};
struct Tex
{
    float u = 0.f;
    float v = 0.f;
};
struct Norm
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

TEST(TestMesh, MESH_CONSTRUCTION)
{

    Pos pos{1.0f, 2.0f, 3.0f};
    Norm normal{4.0f, 5.0f, 6.0f};
    Tex tex{7.0f, 8.0f};
    Rendering::Mesh::Layout layout;
    layout.add_segment(GL_FLOAT, sizeof(Pos), 1);
    layout.add_segment(GL_FLOAT, sizeof(Norm), 1);
    layout.add_segment(GL_FLOAT, sizeof(Tex), 1);

    Rendering::Mesh mesh(layout, 1, 0);

    Pos *pos_ = mesh.vertex_attr<Pos>(0, 0);
    Norm *norm_ = mesh.vertex_attr<Norm>(0, 1);
    Tex *tex_ = mesh.vertex_attr<Tex>(0, 2);

    //debug
    float* data_f = (float*)mesh.vertex(0); 

    pos_->x = pos.x;
    pos_->y = pos.y;
    pos_->z = pos.z;

    norm_->x = normal.x;
    norm_->y = normal.y;
    norm_->z = normal.z;

    tex_->u = tex.u;
    tex_->v = tex.v;


    EXPECT_EQ(pos_->x, pos.x);
    EXPECT_EQ(pos_->y, pos.y);
    EXPECT_EQ(pos_->z, pos.z);

    EXPECT_EQ(norm_->x, normal.x);
    EXPECT_EQ(norm_->y, normal.y);
    EXPECT_EQ(norm_->z, normal.z);

    EXPECT_EQ(tex_->u, tex.u);
    EXPECT_EQ(tex_->v, tex.v);
}

TEST(TestMesh, MEMCPY)
{
    struct Pos
    {
        float x;
        float y;
        float z;
    };
    struct TexCoord
    {
        float u;
        float v;
    };

    Pos vertices[] = {
        {-0.5f, -0.5f, 0.0f}, // left
        {0.5f, -0.5f, 0.0f},  // right
        {0.5f, 0.5f, 0.0f},   // top
        {-0.5f, 0.5f, 0.0f}   // top
    };
    TexCoord texcoords[] = {
        {0.0f, 0.0f}, // left
        {1.0f, 0.0f}, // right
        {1.0f, 1.0f}, // top
        {0.0f, 1.0f}  // top
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3};

    Rendering::Mesh::Layout layout;
    layout.add_segment(GL_FLOAT, sizeof(float), 3);
    layout.add_segment(GL_FLOAT, sizeof(float), 2);
    auto mesh = Rendering::Mesh_Ptr(new Rendering::Mesh(layout, 4, 6));
    for (int i = 0; i < 4; ++i)
    {
        auto p = mesh->vertex_attr<Pos>(i, 0);
        p->x = vertices[i].x;
        p->y = vertices[i].y;
        p->z = vertices[i].z;

        TexCoord *t = mesh->vertex_attr<TexCoord>(i, 1);
        t->u = texcoords[i].u;
        t->v = texcoords[i].v;
    }
    mesh->add_indices(indices, 6);

    for (int i = 0; i < 4; ++i)
    {
        auto p = mesh->vertex_attr<Pos>(i, 0);
        auto t = mesh->vertex_attr<TexCoord>(i, 1);

        EXPECT_EQ(p->x, vertices[i].x);
        EXPECT_EQ(p->y, vertices[i].y);
        EXPECT_EQ(p->z, vertices[i].z);

        EXPECT_EQ(t->u, texcoords[i].u);
        EXPECT_EQ(t->v, texcoords[i].v);
    }
}
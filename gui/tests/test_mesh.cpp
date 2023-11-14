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
    layout.add(1, GL_FLOAT, sizeof(Pos));
    layout.add(1, GL_FLOAT, sizeof(Norm));
    layout.add(1, GL_FLOAT, sizeof(Tex));

    Rendering::Mesh mesh(layout, 1, 0);

    Pos *pos_ = mesh.vertex<Pos>(0,0);
    Norm *norm_ = mesh.vertex<Norm>(0,1);
    Tex *tex_ = mesh.vertex<Tex>(0,2);

    *pos_ = pos;
    *norm_ = normal;
    *tex_ = tex;

    EXPECT_TRUE(true);
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
    layout.add(3, GL_FLOAT, sizeof(float));
    layout.add(2, GL_FLOAT, sizeof(float));
    auto mesh = Rendering::Mesh_Ptr(new Rendering::Mesh(layout, 4, 6));
    for (int i = 0; i < 4; i++)
    {
        memcpy(mesh->vertex<float>(i, 0), &vertices[i], sizeof(Pos));
        memcpy(mesh->vertex<float>(i, 1), &texcoords[i], sizeof(TexCoord));
    }
    memcpy(mesh->indices, indices, sizeof(indices));

    for (int i = 0; i < 4; ++i)
    {
        auto p = mesh->vertex<float>(i, 0);
        auto t = mesh->vertex<float>(i, 1);

        EXPECT_EQ(p[0], vertices[i].x);
        EXPECT_EQ(p[1], vertices[i].y);
        EXPECT_EQ(p[2], vertices[i].z);
        EXPECT_EQ(t[0], texcoords[i].u);
        EXPECT_EQ(t[1], texcoords[i].v);
    }
}
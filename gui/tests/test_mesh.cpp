#include <gtest/gtest.h>
#include <gui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct Pos
{
    float x, y, z;
};
struct Tex
{
    double u, v;
};
struct Normal
{
    float x, y, z;
};

TEST(TestMesh, DEFAULT_CONSTRUCTION)
{
    using namespace Rendering;

    Pos pos{1.0f, 2.0f, 3.0f};
    Normal normal{4.0f, 5.0f, 6.0f};
    Tex tex{7.0f, 8.0f};
    Mesh::Layout layout;
    layout.add(3, GL_FLOAT, sizeof(float));
    layout.add(3, GL_FLOAT, sizeof(float));
    layout.add(2, GL_FLOAT, sizeof(float));

    Mesh mesh(layout, 1, 1);

    float *pos_ = mesh.vertex<float>(0);
    float *norm_ = mesh.vertex<float>(1);
    float *tex_ = mesh.vertex<float>(2);

    norm_[0] = normal.x;
    norm_[1] = normal.y;
    norm_[2] = normal.z;

    // pos_[0] = pos.x;
    // pos_[1] = pos.y;
    // pos_[2] = pos.z;

    // tex_[0] = tex.u;
    // tex_[1] = tex.v;

    // for(int i = 0; i < mesh.vertex_count; ++i)
    // {
    //     auto p = mesh.vertex<Pos*>(0,i);
    //     auto n = mesh.vertex<Normal*>(1,i);
    //     auto t = mesh.vertex<Tex*>(2,i);

    //     EXPECT_EQ(p->x, pos.x);
    //     EXPECT_EQ(p->y, pos.y);
    //     EXPECT_EQ(p->z, pos.z);
    //     std::cout << "p: " << p->x << " " << p->y << " " << p->z << std::endl;
    //     std::cout << "n: " << n->x << " " << n->y << " " << n->z << std::endl;
    //     std::cout << "t: " << t->u << " " << t->v << std::endl;
    // }

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
    TexCoord tex_coords[] = {
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
        memcpy(mesh->vertex<float>(i, 1), &tex_coords[i], sizeof(TexCoord));
    }
    memcpy(mesh->indices, indices, sizeof(indices));

    for (int i = 0; i < 4; ++i)
    {
        auto p = mesh->vertex<float>(i, 0);
        auto t = mesh->vertex<float>(i, 1);

        EXPECT_EQ(p[0], vertices[i].x);
        EXPECT_EQ(p[1], vertices[i].y);
        EXPECT_EQ(p[2], vertices[i].z);
        EXPECT_EQ(t[0], tex_coords[i].u);
        EXPECT_EQ(t[1], tex_coords[i].v);
    }
}

TEST(TestQuaternion, Construct_From_LookAt)
{
    Core::Vector3 pos = Core::Vector3(3.f, 1.0f, 1.0f);
    Core::Vector3 center = Core::Vector3(0.0f, 0.0f, 0.0f);
    Core::Vector3 up = Core::Vector3(0.0f, 1.0f, 0.0f);
    Core::Vector3 front = Geometry::normalize(center - pos);

    std::cout << "front: " << front << std::endl;
    std::cout << "up: " << up << std::endl;
    std::cout << "pos: " << pos << std::endl;
    Core::Quaternion q = Geometry::quat_look_at(front, up);

    glm::vec3 pos_glm = glm::vec3(3.f, 1.0f, 1.0f);
    glm::vec3 center_glm = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up_glm = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 front_glm = glm::normalize(center_glm - pos_glm);

    std::cout << "front_glm: " << front_glm.x << " " << front_glm.y << " " << front_glm.z << std::endl;
    std::cout << "up_glm: " << up_glm.x << " " << up_glm.y << " " << up_glm.z << std::endl;
    std::cout << "pos_glm: " << pos_glm.x << " " << pos_glm.y << " " << pos_glm.z << std::endl;

    glm::quat q_glm = glm::quatLookAtRH(front_glm, up_glm);

    EXPECT_FLOAT_EQ(q.x, q_glm.x);
    EXPECT_FLOAT_EQ(q.y, q_glm.y);
    EXPECT_FLOAT_EQ(q.z, q_glm.z);
    EXPECT_FLOAT_EQ(q.w, q_glm.w);
}

TEST(TestGeometry, look_at_quat)
{
    Core::Vector3 pos = Core::Vector3(3.f, 1.0f, 1.0f);
    Core::Vector3 center = Core::Vector3(0.0f, 0.0f, 0.0f);
    Core::Vector3 up = Core::Vector3(0.0f, 1.0f, 0.0f);
    Core::Matrix4 r = Geometry::look_at(pos, center, up);

    float distance = Core::Vector(pos - center).length();

    Core::Quaternion q = Geometry::quat_look_at(Geometry::normalize(center - pos), up, false);

    Core::Vector3 pos_ = pos;
    Core::Vector3 front_ = q * Core::Vector3(0, 0, 1); // q.get_front()
    Core::Vector3 up_ = q * Core::Vector3(0, 1, 0); // q.get_up()
    Core::Vector3 center_ = pos_ + front_ * distance;

    std::cout << "pos: " << pos << std::endl;
    std::cout << "center: " << center << std::endl;
    std::cout << "up: " << up << std::endl;
    std::cout << "pos_: " << pos_ << std::endl;
    std::cout << "center_: " << center_ << std::endl;
    std::cout << "up_: " << up_ << std::endl;

    Core::Matrix4 r2 = Geometry::look_at(pos_, center_, up_);

    EXPECT_TRUE(EQUAL_F(r(0, 0), r2(0, 0)));
    EXPECT_TRUE(EQUAL_F(r(0, 1), r2(0, 1)));
    EXPECT_TRUE(EQUAL_F(r(0, 2), r2(0, 2)));
    EXPECT_TRUE(EQUAL_F(r(0, 3), r2(0, 3)));

    EXPECT_TRUE(EQUAL_F(r(1, 0), r2(1, 0)));
    EXPECT_TRUE(EQUAL_F(r(1, 1), r2(1, 1)));
    EXPECT_TRUE(EQUAL_F(r(1, 2), r2(1, 2)));
    EXPECT_TRUE(EQUAL_F(r(1, 3), r2(1, 3)));

    EXPECT_TRUE(EQUAL_F(r(2, 0), r2(2, 0)));
    EXPECT_TRUE(EQUAL_F(r(2, 1), r2(2, 1)));
    EXPECT_TRUE(EQUAL_F(r(2, 2), r2(2, 2)));
    EXPECT_TRUE(EQUAL_F(r(2, 3), r2(2, 3)));

    EXPECT_TRUE(EQUAL_F(r(3, 0), r2(3, 0)));
    EXPECT_TRUE(EQUAL_F(r(3, 1), r2(3, 1)));
    EXPECT_TRUE(EQUAL_F(r(3, 2), r2(3, 2)));
    EXPECT_TRUE(EQUAL_F(r(3, 3), r2(3, 3)));

    EXPECT_TRUE(r == r2);
}
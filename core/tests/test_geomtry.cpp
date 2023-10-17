#include "core.h"
#include "gtest/gtest.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "test_utils.h"
#include <glm/gtx/quaternion.hpp>

TEST(TestGeometry, radians)
{
    EXPECT_FLOAT_EQ(Geometry::radians(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(Geometry::radians(90.0f), 1.5707963267948966f);
    EXPECT_FLOAT_EQ(Geometry::radians(180.0f), 3.141592653589793f);
    EXPECT_FLOAT_EQ(Geometry::radians(270.0f), 4.71238898038469f);
    EXPECT_FLOAT_EQ(Geometry::radians(360.0f), 6.283185307179586f);
}

TEST(TestGeometry, look_at)
{
    Core::Vector3 pos = Core::Vector3(3.f, 1.0f, 1.0f);
    Core::Vector3 center = Core::Vector3(0.0f, 0.0f, 1.0f);
    Core::Vector3 up = Core::Vector3(0.0f, 1.0f, 0.0f);
    Core::Matrix4 r = Geometry::look_at(pos, center, up);

    glm::vec3 pos_glm = glm::vec3(3.f, 1.0f, 1.0f);
    glm::vec3 center_glm = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 up_glm = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 r_glm = glm::lookAt(pos_glm, center_glm, up_glm);

    std::cout << r << std::endl;
    std::cout << r_glm << std::endl;

    EXPECT_TRUE(Expect_Matrix_Equal(r, r_glm));
}

TEST(TestGeometry, translate)
{
    Core::Matrix4 m = Core::Matrix4::identity();
    m = Geometry::translate(m, Core::Vector3(1.0f, 2.0f, 3.0f));

    glm::mat4 m_glm = glm::mat4(1.0f);
    m_glm = glm::translate(m_glm, glm::vec3(1.0f, 2.0f, 3.0f));

    std::cout << m << std::endl;
    std::cout << m_glm << std::endl;

    EXPECT_TRUE(Expect_Matrix_Equal(m, m_glm));
}

TEST(TestGeometry, rotate)
{
    Core::Matrix4 m = Core::Matrix4::identity();
    m = Geometry::translate(m, Core::Vector3(1.0f, 2.0f, 3.0f));
    m = Geometry::rotate(m, Geometry::radians(90.0f), Core::Vector3(2.0f, 0.0f, 0.0f));

    glm::mat4 m_glm = glm::mat4(1.0f);
    m_glm = glm::translate(m_glm, glm::vec3(1.0f, 2.0f, 3.0f));
    m_glm = glm::rotate(m_glm, glm::radians(90.0f), glm::vec3(2.0f, 0.0f, 0.0f));

    std::cout << m << std::endl;
    std::cout << m_glm << std::endl;

    EXPECT_TRUE(Expect_Matrix_Equal(m, m_glm));
}

TEST(TestGeometry, scale)
{
    Core::Matrix4 m = Core::Matrix4::identity();
    m = Geometry::translate(m, Core::Vector3(1.0f, 2.0f, 3.0f));
    m = Geometry::rotate(m, Geometry::radians(90.0f), Core::Vector3(1.0f, 2.0f, 0.0f));
    m = Geometry::scale(m, Core::Vector3(1.0f, 2.0f, 3.0f));

    glm::mat4 m_glm = glm::mat4(1.0f);
    m_glm = glm::translate(m_glm, glm::vec3(1.0f, 2.0f, 3.0f));
    m_glm = glm::rotate(m_glm, glm::radians(90.0f), glm::vec3(1.0f, 2.0f, 0.0f));
    m_glm = glm::scale(m_glm, glm::vec3(1.0f, 2.0f, 3.0f));

    std::cout << m << std::endl;
    std::cout << m_glm << std::endl;

    EXPECT_TRUE(Expect_Matrix_Equal(m, m_glm));
}

TEST(TestGeometry, ortho)
{
    Core::Matrix4 m = Geometry::orthographic(0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 100.0f);

    glm::mat4 m_glm = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 100.0f);

    std::cout << m << std::endl;
    std::cout << m_glm << std::endl;

    EXPECT_TRUE(Expect_Matrix_Equal(m, m_glm));
}

TEST(TestGeometry, perspective)
{
    Core::Matrix4 m = Geometry::perspective(Geometry::radians(45.f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::mat4 m_glm = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    std::cout << m << std::endl;
    std::cout << m_glm << std::endl;

    EXPECT_TRUE(Expect_Matrix_Equal(m, m_glm));
}

TEST(TestGeometry, frustum)
{
    Core::Matrix4 m = Geometry::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

    glm::mat4 m_glm = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

    std::cout << m << std::endl;
    std::cout << m_glm << std::endl;

    EXPECT_TRUE(Expect_Matrix_Equal(m, m_glm));
}

TEST(TestGeometry, quat_look_at)
{
    Core::Vector3 direction = Core::Vector3(0.0f, 0.0f, -1.0f);
    Core::Vector3 up = Core::Vector3(0.0f, 2.0f, 2.0f);
    Core::Quaternion q = Geometry::quat_look_at(direction, up);

    glm::vec3 direction_glm = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up_glm = glm::vec3(0.0f, 2.0f, 2.0f);
    glm::quat q_glm = glm::quatLookAt(direction_glm, up_glm);

    std::cout << "q: " << q.w << ", " << q.x << ", " << q.y << ", " << q.z << std::endl;
    std::cout << "q_glm: " << q_glm.w << ", " << q_glm.x << ", " << q_glm.y << ", " << q_glm.z << std::endl;

    EXPECT_FLOAT_EQ(q.w, q_glm.w);
    EXPECT_FLOAT_EQ(q.x, q_glm.x);
    EXPECT_FLOAT_EQ(q.y, q_glm.y);
    EXPECT_FLOAT_EQ(q.z, q_glm.z);
}

TEST(TestGeometry, quat_matrix_look_at)
{
    Core::Vector3 center = Core::Vector3(0.0f, 0.0f, 0.0f);
    Core::Vector3 up = Core::Vector3(0.0f, 2.0f, 2.0f);
    Core::Vector3 pos = Core::Vector3(0.0f, 0.0f, 4.0f);
    Core::Vector3 direction = pos - center;
    Core::Quaternion q = Geometry::quat_look_at(direction, up);
    Core::Vector3 up_ = q * Core::Vector3(0.0f, 1.0f, 0.0f);
    Core::Matrix4 m1 = Geometry::look_at(pos, center, up_);


    Core::Matrix4 m2 = Geometry::look_at(pos, center, up);

    std::cout << "m1" << std::endl
              << m1 << std::endl;
    std::cout << "m2" << std::endl
              << m2 << std::endl;

    EXPECT_TRUE(m1 == m2);
}
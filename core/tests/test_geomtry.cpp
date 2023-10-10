#include "core.h"
#include "gtest/gtest.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "test_utils.h"

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
    Core::Vector3 center = Core::Vector3(0.0f, 0.0f, 0.0f);
    Core::Vector3 up = Core::Vector3(0.0f, 1.0f, 0.0f);
    Core::Matrix4 r = Geometry::look_at(pos, center, up);

    glm::vec3 pos_glm = glm::vec3(3.f, 1.0f, 1.0f);
    glm::vec3 center_glm = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up_glm = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 r_glm = glm::lookAt(pos_glm, center_glm, up_glm);

    EXPECT_FLOAT_EQ(r(0, 0), r_glm[0][0]);
    EXPECT_FLOAT_EQ(r(0, 1), r_glm[1][0]);
    EXPECT_FLOAT_EQ(r(0, 2), r_glm[2][0]);
    EXPECT_FLOAT_EQ(r(0, 3), r_glm[3][0]);

    EXPECT_FLOAT_EQ(r(1, 0), r_glm[0][1]);
    EXPECT_FLOAT_EQ(r(1, 1), r_glm[1][1]);
    EXPECT_FLOAT_EQ(r(1, 2), r_glm[2][1]);
    EXPECT_FLOAT_EQ(r(1, 3), r_glm[3][1]);

    EXPECT_FLOAT_EQ(r(2, 0), r_glm[0][2]);
    EXPECT_FLOAT_EQ(r(2, 1), r_glm[1][2]);
    EXPECT_FLOAT_EQ(r(2, 2), r_glm[2][2]);
    EXPECT_FLOAT_EQ(r(2, 3), r_glm[3][2]);

    EXPECT_FLOAT_EQ(r(3, 0), r_glm[0][3]);
    EXPECT_FLOAT_EQ(r(3, 1), r_glm[1][3]);
    EXPECT_FLOAT_EQ(r(3, 2), r_glm[2][3]);
    EXPECT_FLOAT_EQ(r(3, 3), r_glm[3][3]);
}

TEST(TestGeometry, translate)
{
    Core::Matrix4 m = Core::Matrix4::identity();
    m = Geometry::translate(m, Core::Vector3(1.0f, 2.0f, 3.0f));

    glm::mat4 m_glm = glm::mat4(1.0f);
    m_glm = glm::translate(m_glm, glm::vec3(1.0f, 2.0f, 3.0f));

    std::cout << m << std::endl;
    std::cout << m_glm << std::endl;

    EXPECT_TRUE(Expect_Matrix_Equal(m, glm::transpose(m_glm)));
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

    EXPECT_TRUE(Expect_Matrix_Equal(m, glm::transpose(m_glm)));
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
    std::cout << glm::transpose(m_glm) << std::endl;

    EXPECT_TRUE(Expect_Matrix_Equal(m, glm::transpose(m_glm)));
}
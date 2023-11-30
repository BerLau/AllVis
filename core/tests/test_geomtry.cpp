#include "gtest/gtest.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "test_utils.h"
#include <glm/gtx/quaternion.hpp>
#include "geometry/general.h"
#include "geometry/geometry3d.h"
#include "geometry/geometry2d.h"

TEST(TestGeometry, radians)
{
    EXPECT_FLOAT_EQ(Core::Geometry::radians(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(Core::Geometry::radians(90.0f), 1.5707963267948966f);
    EXPECT_FLOAT_EQ(Core::Geometry::radians(180.0f), 3.141592653589793f);
    EXPECT_FLOAT_EQ(Core::Geometry::radians(270.0f), 4.71238898038469f);
    EXPECT_FLOAT_EQ(Core::Geometry::radians(360.0f), 6.283185307179586f);
}

TEST(TestGeometry, look_at)
{
    Core::Vector3 pos = Core::Vector3(3.f, 1.0f, 1.0f);
    Core::Vector3 center = Core::Vector3(0.0f, 0.0f, 1.0f);
    Core::Vector3 up = Core::Vector3(0.0f, 1.0f, 0.0f);
    Core::Matrix4 r = Core::Geometry::look_at(pos, center, up);

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
    m = Core::Geometry::translate(m, Core::Vector3(1.0f, 2.0f, 3.0f));

    glm::mat4 m_glm = glm::mat4(1.0f);
    m_glm = glm::translate(m_glm, glm::vec3(1.0f, 2.0f, 3.0f));

    std::cout << m << std::endl;
    std::cout << m_glm << std::endl;

    EXPECT_TRUE(Expect_Matrix_Equal(m, m_glm));
}

TEST(TestGeometry, rotate)
{
    Core::Matrix4 m = Core::Matrix4::identity();
    m = Core::Geometry::translate(m, Core::Vector3(1.0f, 2.0f, 3.0f));
    m = Core::Geometry::rotate(m, Core::Geometry::radians(90.0f), Core::Vector3(2.0f, 0.0f, 0.0f));

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
    m = Core::Geometry::translate(m, Core::Vector3(1.0f, 2.0f, 3.0f));
    m = Core::Geometry::rotate(m, Core::Geometry::radians(90.0f), Core::Vector3(1.0f, 2.0f, 0.0f));
    m = Core::Geometry::scale(m, Core::Vector3(1.0f, 2.0f, 3.0f));

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
    Core::Matrix4 m = Core::Geometry::orthographic(0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 100.0f);

    glm::mat4 m_glm = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 100.0f);

    std::cout << m << std::endl;
    std::cout << m_glm << std::endl;

    EXPECT_TRUE(Expect_Matrix_Equal(m, m_glm));
}

TEST(TestGeometry, perspective)
{
    Core::Matrix4 m = Core::Geometry::perspective(Core::Geometry::radians(45.f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::mat4 m_glm = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    std::cout << m << std::endl;
    std::cout << m_glm << std::endl;

    EXPECT_TRUE(Expect_Matrix_Equal(m, m_glm));
}

TEST(TestGeometry, frustum)
{
    Core::Matrix4 m = Core::Geometry::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

    glm::mat4 m_glm = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

    std::cout << m << std::endl;
    std::cout << m_glm << std::endl;

    EXPECT_TRUE(Expect_Matrix_Equal(m, m_glm));
}

TEST(TestGeometry, quat_look_at)
{
    Core::Vector3 direction = Core::Vector3(0.0f, 0.0f, -1.0f);
    Core::Vector3 up = Core::Vector3(0.0f, 2.0f, 2.0f);
    Core::Quaternion q = Core::Geometry::quat_look_at(direction, up);

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
    Core::Quaternion q = Core::Geometry::quat_look_at(direction, up);
    Core::Vector3 up_ = q * Core::Vector3(0.0f, 1.0f, 0.0f);
    Core::Matrix4 m1 = Core::Geometry::look_at(pos, center, up_);

    Core::Matrix4 m2 = Core::Geometry::look_at(pos, center, up);

    std::cout << "m1" << std::endl
              << m1 << std::endl;
    std::cout << "m2" << std::endl
              << m2 << std::endl;

    EXPECT_TRUE(m1 == m2);
}

TEST(TestGeometry, AngleAxis)
{
    Core::Vector3 axis = Core::Vector3(0.0f, 1.0f, 0.0f);
    Core::Quaternion q = Core::Geometry::angle_axis(Core::Geometry::radians(90.0f), axis);

    glm::vec3 axis_glm = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::quat q_glm = glm::angleAxis(glm::radians(90.0f), axis_glm);

    std::cout << "q: " << q.w << ", " << q.x << ", " << q.y << ", " << q.z << std::endl;
    std::cout << "q_glm: " << q_glm.w << ", " << q_glm.x << ", " << q_glm.y << ", " << q_glm.z << std::endl;

    EXPECT_FLOAT_EQ(q.w, q_glm.w);
    EXPECT_FLOAT_EQ(q.x, q_glm.x);
    EXPECT_FLOAT_EQ(q.y, q_glm.y);
    EXPECT_FLOAT_EQ(q.z, q_glm.z);
}

TEST(TestGeometry, point_line_position)
{
    Core::Vector2 a(0.8, 4);
    Core::Vector2 b(3.7, 11.25);
    Core::Vector2 point = {2.5, 8.25};

    int r = Core::Geometry::point_line_position(point, a, b);
    EXPECT_EQ(r, 0);
    point.y() = 7.75;
    r = Core::Geometry::point_line_position(point, a, b);
    EXPECT_EQ(r, -1);
    point.x() = -2.5;
    point.y() = -0.55;
    r = Core::Geometry::point_line_position(point, a, b);
    EXPECT_EQ(r, 1);
}

TEST(TestGeometry, point_in_triangle)
{
    Core::Vector2 a(0.0f, 0.0f);
    Core::Vector2 b(1.0f, 0.0f);
    Core::Vector2 c(0.0f, 1.0f);
    Core::Vector2 point(0.5f, 0.5f);
    bool r = Core::Geometry::point_in_triangle(point, a, b, c);
    EXPECT_EQ(r, true);
    point.x() = 1.5f;
    r = Core::Geometry::point_in_triangle(point, a, b, c);
    EXPECT_EQ(r, false);
    point.x() = 0.5f;
    point.y() = 1.5f;
    r = Core::Geometry::point_in_triangle(point, a, b, c);
    EXPECT_EQ(r, false);


}
TEST(TestGeometry, point_line_distance)
{
    Core::Vector2 a(0.8, 4);
    Core::Vector2 b(3.7, 11.25);
    Core::Vector2 point = {2.5, 8.25};

    float r = Core::Geometry::distance_point_line(point, a, b);
    EXPECT_FLOAT_EQ(r, 0.0f);
}
#include <gtest/gtest.h>
#include "core.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "test_utils.h"

using namespace Core;

TEST(TestQuaternion, from_matrix4)
{
    Matrix4 m = {
        0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, -0.0f, 0.0f, 1.0f};
    Core::Quaternion q = Core::Quaternion::from_matrix(m);

    glm::mat4 m_glm = glm::mat4(
        0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, -0.0f, 0.0f, 1.0f);
    glm::quat q_glm = glm::quat_cast(m_glm);

    std::cout << "q: " << q.w << ", " << q.x << ", " << q.y << ", " << q.z << std::endl;
    std::cout << "q_glm: " << q_glm.w << ", " << q_glm.x << ", " << q_glm.y << ", " << q_glm.z << std::endl;

    EXPECT_TRUE(EQUAL_F(q.w, q_glm.w));
    EXPECT_TRUE(EQUAL_F(q.x, q_glm.x));
    EXPECT_TRUE(EQUAL_F(q.y, q_glm.y));
    EXPECT_TRUE(EQUAL_F(q.z, q_glm.z));
}

TEST(TestQuaternion, from_matrix3)
{
    Matrix3 m = {
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f};
    Core::Quaternion q = Core::Quaternion::from_matrix(m);

    glm::mat3 m_glm = glm::mat3(
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);
    glm::quat q_glm = glm::quat_cast(m_glm);

    std::cout << "q: " << q.w << ", " << q.x << ", " << q.y << ", " << q.z << std::endl;
    std::cout << "q_glm: " << q_glm.w << ", " << q_glm.x << ", " << q_glm.y << ", " << q_glm.z << std::endl;

    EXPECT_TRUE(EQUAL_F(q.w, q_glm.w));
    EXPECT_TRUE(EQUAL_F(q.x, q_glm.x));
    EXPECT_TRUE(EQUAL_F(q.y, q_glm.y));
    EXPECT_TRUE(EQUAL_F(q.z, q_glm.z));
}

TEST(TestQuaternion, from_axis_angle)
{
    Vector3 axis = Vector3(0.0f, 1.0f, 0.0f);
    float angle = 90.f;
    Quaternion q = Quaternion::from_axis_angle(axis, Geometry::radians(angle));

    glm::vec3 axis_glm = glm::vec3(0.0f, 1.0f, 0.0f);
    float angle_glm = 90.f;
    glm::quat q_glm = glm::angleAxis(glm::radians(angle_glm), axis_glm);

    std::cout << "q: " << q.w << ", " << q.x << ", " << q.y << ", " << q.z << std::endl;
    std::cout << "q_glm: " << q_glm.w << ", " << q_glm.x << ", " << q_glm.y << ", " << q_glm.z << std::endl;

    EXPECT_TRUE(EQUAL_F(q.w, q_glm.w));
    EXPECT_TRUE(EQUAL_F(q.x, q_glm.x));
    EXPECT_TRUE(EQUAL_F(q.y, q_glm.y));
    EXPECT_TRUE(EQUAL_F(q.z, q_glm.z));
}

TEST(TestQuaternion, from_euler_angle)
{
    float yaw = 120.f;
    float pitch = 30.f;
    float roll = 45.f;

    float yaw_rad = Geometry::radians(yaw);
    float pitch_rad = Geometry::radians(pitch);
    float roll_rad = Geometry::radians(roll);

    Quaternion q = Quaternion::from_euler_angle(pitch, yaw, roll);

    glm::quat q_glm = glm::quat(glm::vec3(pitch_rad, yaw_rad, roll_rad));

    std::cout << "q: " << q.w << ", " << q.x << ", " << q.y << ", " << q.z << std::endl;
    std::cout << "q_glm: " << q_glm.w << ", " << q_glm.x << ", " << q_glm.y << ", " << q_glm.z << std::endl;

    EXPECT_TRUE(EQUAL_F(q.w, q_glm.w));
    EXPECT_TRUE(EQUAL_F(q.x, q_glm.x));
    EXPECT_TRUE(EQUAL_F(q.y, q_glm.y));
    EXPECT_TRUE(EQUAL_F(q.z, q_glm.z));
}

TEST(TestQuaternion, to_matrix4)
{
    Quaternion q = Quaternion(0.7071068f, 1.2f, -2.0f, 0.7071068f);
    Matrix4 m = q.to_matrix4();

    glm::quat q_glm = glm::quat(0.7071068f, 1.2f, -2.0f, 0.7071068f);
    glm::mat4 m_glm = glm::mat4_cast(q_glm);

    std::cout << "m: " << std::endl
              << m << std::endl;
    std::cout << "m_glm: " << std::endl
              << m_glm << std::endl;

    EXPECT_TRUE(Expect_Matrix_Equal(m, m_glm));
}

TEST(TestQuaternion, to_matrix3)
{
    Quaternion q = Quaternion(0.7071068f, 1.2f, -2.0f, 0.7071068f);
    Matrix3 m = q.to_matrix3();

    glm::quat q_glm = glm::quat(0.7071068f, 1.2f, -2.0f, 0.7071068f);
    glm::mat3 m_glm = glm::mat3_cast(q_glm);

    std::cout << "m: " << std::endl
              << m << std::endl;
    std::cout << "m_glm: " << std::endl
              << m_glm << std::endl;

    EXPECT_FLOAT_EQ(m(0, 0), m_glm[0][0]);
    EXPECT_FLOAT_EQ(m(0, 1), m_glm[0][1]);
    EXPECT_FLOAT_EQ(m(0, 2), m_glm[0][2]);

    EXPECT_FLOAT_EQ(m(1, 0), m_glm[1][0]);
    EXPECT_FLOAT_EQ(m(1, 1), m_glm[1][1]);
    EXPECT_FLOAT_EQ(m(1, 2), m_glm[1][2]);

    EXPECT_FLOAT_EQ(m(2, 0), m_glm[2][0]);
    EXPECT_FLOAT_EQ(m(2, 1), m_glm[2][1]);
    EXPECT_FLOAT_EQ(m(2, 2), m_glm[2][2]);
}

TEST(TestQuaternion, to_axis_angle)
{
    Core::Vector3 axis = Core::Vector3(1.0f, -0.3f, 0.7f);
    float angle = Geometry::radians(65.f);

    Quaternion q = Quaternion::from_axis_angle(axis, angle);
    std::tuple<Vector3, float> axis_angle = q.to_axis_angle();
    Core::Vector3 axis_ = std::get<0>(axis_angle);
    float angle_ = std::get<1>(axis_angle);

    std::cout << "axis: " << axis << std::endl;
    std::cout << "angle: " << angle << std::endl;
    std::cout << "axis_: " << axis_ << std::endl;
    std::cout << "angle_: " << angle_ << std::endl;

    EXPECT_TRUE(EQUAL_F(axis.x(), axis_.x()));
    EXPECT_TRUE(EQUAL_F(axis.y(), axis_.y()));
    EXPECT_TRUE(EQUAL_F(axis.z(), axis_.z()));
    EXPECT_TRUE(EQUAL_F(angle, angle_));
}

TEST(TestQuaternion, to_euler_angle)
{
    float pitch = 30.f;
    float yaw = 120.f;
    float roll = 45.f;

    float pitch_rad = Geometry::radians(pitch);
    float yaw_rad = Geometry::radians(yaw);
    float roll_rad = Geometry::radians(roll);

    std::cout << "pitch: " << pitch_rad << std::endl;
    std::cout << "yaw: " << yaw_rad << std::endl;
    std::cout << "roll: " << roll_rad << std::endl;

    Quaternion q = Quaternion::from_euler_angle(pitch, yaw, roll);
    EulerAngle euler_angle = q.to_euler_angle();

    glm::quat q_glm = glm::quat(glm::vec3(pitch_rad, yaw_rad, roll_rad));
    glm::vec3 euler_angle_glm = glm::degrees(glm::eulerAngles(q_glm));

    float pitch_glm = euler_angle_glm.x;
    float yaw_glm = euler_angle_glm.y;
    float roll_glm = euler_angle_glm.z;

    std::cout << "pitch: " << euler_angle.pitch << std::endl;
    std::cout << "yaw: " << euler_angle.yaw << std::endl;
    std::cout << "roll: " << euler_angle.roll << std::endl;

    std::cout << "pitch_glm: " << pitch_glm << std::endl;
    std::cout << "yaw_glm: " << yaw_glm << std::endl;
    std::cout << "roll_glm: " << roll_glm << std::endl;

    EXPECT_TRUE(EQUAL_F(euler_angle.yaw, yaw_glm));
    EXPECT_TRUE(EQUAL_F(euler_angle.pitch, pitch_glm));
    EXPECT_TRUE(EQUAL_F(euler_angle.roll, roll_glm));
}

TEST(TestQuaternion, conjugate)
{
    Quaternion q = Quaternion(0.7071068f, 1.2f, -2.0f, 0.7071068f);
    Quaternion q_conj = q.conjugate();

    glm::quat q_glm = glm::quat(0.7071068f, 1.2f, -2.0f, 0.7071068f);
    glm::quat q_conj_glm = glm::conjugate(q_glm);

    std::cout << "q: " << q.w << ", " << q.x << ", " << q.y << ", " << q.z << std::endl;
    std::cout << "q_conj: " << q_conj.w << ", " << q_conj.x << ", " << q_conj.y << ", " << q_conj.z << std::endl;
    std::cout << "q_conj_glm: " << q_conj_glm.w << ", " << q_conj_glm.x << ", " << q_conj_glm.y << ", " << q_conj_glm.z << std::endl;

    EXPECT_TRUE(EQUAL_F(q_conj.w, q_conj_glm.w));
    EXPECT_TRUE(EQUAL_F(q_conj.x, q_conj_glm.x));
    EXPECT_TRUE(EQUAL_F(q_conj.y, q_conj_glm.y));
    EXPECT_TRUE(EQUAL_F(q_conj.z, q_conj_glm.z));
}

TEST(TestQuaternion, inverse)
{
    Quaternion q = Quaternion(0.7071068f, 1.2f, -2.0f, 0.7071068f);
    Quaternion q_inv = q.inverse();

    glm::quat q_glm = glm::quat(0.7071068f, 1.2f, -2.0f, 0.7071068f);
    glm::quat q_inv_glm = glm::inverse(q_glm);

    std::cout << "q: " << q.w << ", " << q.x << ", " << q.y << ", " << q.z << std::endl;
    std::cout << "q_inv: " << q_inv.w << ", " << q_inv.x << ", " << q_inv.y << ", " << q_inv.z << std::endl;
    std::cout << "q_inv_glm: " << q_inv_glm.w << ", " << q_inv_glm.x << ", " << q_inv_glm.y << ", " << q_inv_glm.z << std::endl;

    EXPECT_TRUE(EQUAL_F(q_inv.w, q_inv_glm.w));
    EXPECT_TRUE(EQUAL_F(q_inv.x, q_inv_glm.x));
    EXPECT_TRUE(EQUAL_F(q_inv.y, q_inv_glm.y));
    EXPECT_TRUE(EQUAL_F(q_inv.z, q_inv_glm.z));
}

TEST(TestQuaternion, dot)
{
    Quaternion q1 = Quaternion(0.7071068f, 1.2f, -2.0f, 0.7071068f);
    Quaternion q2 = Quaternion(0.7071068f, 1.2f, -2.0f, 0.7071068f);

    float dot = Quaternion::dot(q1, q2);
    float dot_glm = glm::dot(glm::quat(0.7071068f, 1.2f, -2.0f, 0.7071068f), glm::quat(0.7071068f, 1.2f, -2.0f, 0.7071068f));

    std::cout << "dot: " << dot << std::endl;
    std::cout << "dot_glm: " << dot_glm << std::endl;

    EXPECT_TRUE(EQUAL_F(dot, dot_glm));
}

TEST(TestQuaternion, identity)
{
    Quaternion q = Quaternion::identity();
    Quaternion q_glm = Quaternion(1, 0, 0, 0);

    std::cout << "q: " << q.w << ", " << q.x << ", " << q.y << ", " << q.z << std::endl;
    std::cout << "q_glm: " << q_glm.w << ", " << q_glm.x << ", " << q_glm.y << ", " << q_glm.z << std::endl;

    EXPECT_TRUE(EQUAL_F(q.w, q_glm.w));
    EXPECT_TRUE(EQUAL_F(q.x, q_glm.x));
    EXPECT_TRUE(EQUAL_F(q.y, q_glm.y));
    EXPECT_TRUE(EQUAL_F(q.z, q_glm.z));
}

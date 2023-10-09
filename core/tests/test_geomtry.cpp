#include "core.h"
#include "gtest/gtest.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    EXPECT_FLOAT_EQ(r(0,0), r_glm[0][0]);
    EXPECT_FLOAT_EQ(r(0,1), r_glm[1][0]);
    EXPECT_FLOAT_EQ(r(0,2), r_glm[2][0]);
    EXPECT_FLOAT_EQ(r(0,3), r_glm[3][0]);

    EXPECT_FLOAT_EQ(r(1,0), r_glm[0][1]);
    EXPECT_FLOAT_EQ(r(1,1), r_glm[1][1]);
    EXPECT_FLOAT_EQ(r(1,2), r_glm[2][1]);
    EXPECT_FLOAT_EQ(r(1,3), r_glm[3][1]);

    EXPECT_FLOAT_EQ(r(2,0), r_glm[0][2]);
    EXPECT_FLOAT_EQ(r(2,1), r_glm[1][2]);
    EXPECT_FLOAT_EQ(r(2,2), r_glm[2][2]);
    EXPECT_FLOAT_EQ(r(2,3), r_glm[3][2]);

    EXPECT_FLOAT_EQ(r(3,0), r_glm[0][3]);
    EXPECT_FLOAT_EQ(r(3,1), r_glm[1][3]);
    EXPECT_FLOAT_EQ(r(3,2), r_glm[2][3]);
    EXPECT_FLOAT_EQ(r(3,3), r_glm[3][3]);
}
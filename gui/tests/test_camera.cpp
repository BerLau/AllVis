#include <gtest/gtest.h>
#include <gui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

TEST(TestCamera, LookAt)
{
    Core::Vector3 pos = Core::Vector3(3.f, 1.0f, 1.0f);
    Core::Vector3 center = Core::Vector3(0.0f, 0.0f, 0.0f);
    Core::Vector3 up = Core::Vector3(0.0f, 1.0f, 0.0f);
    Core::Matrix4 r = Geometry::look_at(pos, center, up);

    Rendering::Camera camera(pos, center, up);

    Core::Matrix4 r2 = camera.get_view_matrix();

    std::cout << "r: " << r << std::endl;
    std::cout << "r2: " << r2 << std::endl;

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
}
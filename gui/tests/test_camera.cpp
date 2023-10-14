#include <gtest/gtest.h>
#include <gui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// bool Expect_Matrix_Equal(const Core::Matrix4 &m1, const glm::mat4 &m2)
// {
//     for (int i = 0; i < 16; i++)
//     {
//         for (int j = 0; j < 4; j++)
//         {
//             if (m1(i, j) != m2[i][j])
//             {
//                 std::cout << "m1[" << i << "][" << j << "] = " << m1(i, j) << std::endl;
//                 std::cout << "m2[" << i << "][" << j << "] = " << m2[i][j] << std::endl;
//                 return false;
//             }
//         }
//     }
//     return true;
// }

TEST(TestCamera, LookAt)
{
    Core::Vector3 pos = Core::Vector3(0.f, 2.0f, 4.0f);
    Core::Vector3 center = Core::Vector3(0.0f, 1.0f, 2.0f);
    Core::Vector3 up = Core::Vector3(0.0f, 2.0f, 0.0f);
    Core::Matrix4 r = Geometry::look_at(pos, center, up);

    Rendering::Camera camera(pos);
    camera.focus_on(center, up);

    Core::Matrix4 r2 = camera.get_view_matrix();

    std::cout << r << std::endl;
    std::cout << r2 << std::endl;

    EXPECT_TRUE(r == r2);
}
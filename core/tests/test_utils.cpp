#include "test_utils.h"
#include "tools.h"
#include "math/base.h"
#include "math/random.h"

std::ostream &operator<<(std::ostream &os, const glm::mat4 &m)
{
    os << "[" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << "]" << std::endl;
    os << "[" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << "]" << std::endl;
    os << "[" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << "]" << std::endl;
    os << "[" << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << "]" << std::endl;
    return os;
}

bool Expect_Matrix_Equal(const Core::Matrix4 &m1, const glm::mat4 &m2)
{
    bool rslt = true;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (Core::Math::equal(m1(i, j), m2[i][j]) == false)
            {
                std::cout << "m1(" << i << ", " << j << ") = " << m1(i, j) << ", m2(" << i << ", " << j << ") = " << m2[i][j] << std::endl;
                rslt = false;
            }
        }
    }
    return rslt;
}

bool Expect_Matrix_Equal(const Core::Matrix3 &m1, const glm::mat3 &m2)
{
    bool rslt = true;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (Core::Math::equal(m1(i, j), m2[i][j]) == false)
            {
                std::cout << "m1(" << i << ", " << j << ") = " << m1(i, j) << ", m2(" << i << ", " << j << ") = " << m2[i][j] << std::endl;
                rslt = false;
            }
        }
    }
    return rslt;
}

TEST(TestRandom, random_max_int)
{
    int max = 10;
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            int rslt = Core::Math::random(max);
            std::cout << "random number: " << rslt << std::endl;
            EXPECT_GE(rslt, 0);
            EXPECT_LT(rslt, max);
        }
    }
}

TEST(TestRandom, random_range_int)
{
    int min = 10;
    int max = 20;
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            int rslt = Core::Math::random(min, max);
            std::cout << "random number: " << rslt << std::endl;
            EXPECT_GE(rslt, min);
            EXPECT_LT(rslt, max);
        }
    }
}

TEST(TestRandom, random_max_float)
{
    float max = 1.5f;
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            float rslt = Core::Math::random(max);
            std::cout << "random number: " << rslt << std::endl;
            EXPECT_GE(rslt, 0.f);
            EXPECT_LT(rslt, max);
        }
    }
}

TEST(TestRandom, random_range_float)
{
    float min = -1.5f;
    float max = 1.0f;
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            float rslt = Core::Math::random(min, max);
            std::cout << "random number: " << rslt << std::endl;
            EXPECT_GE(rslt, min);
            EXPECT_LT(rslt, max);
        }
    }
}
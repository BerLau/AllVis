#include <gtest/gtest.h>
#include "cases_vector.h"

TEST_F(VectorTest, DEFAULT_CONSTRUCTION)
{
    using namespace Core;
    Vector v(3);
    EXPECT_FLOAT_EQ(v[0], 0);
    EXPECT_FLOAT_EQ(v[1], 0);
    EXPECT_FLOAT_EQ(v[2], 0);
}

TEST_F(VectorTest, COPY_CONSTRUCTION_LVALUE)
{
    using namespace Core;
    Vector v1 = Vector::ones(3);
    Vector v(v1);
    EXPECT_FLOAT_EQ(v[0], 1);
    EXPECT_FLOAT_EQ(v[1], 1);
    EXPECT_FLOAT_EQ(v[2], 1);
}

TEST_F(VectorTest, COPY_CONSTRUCTION_RVALUE)
{
    using namespace Core;
    Vector v(Vector::ones(3));
    EXPECT_FLOAT_EQ(v[0], 1);
    EXPECT_FLOAT_EQ(v[1], 1);
    EXPECT_FLOAT_EQ(v[2], 1);
}

TEST_F(VectorTest, COPY_ASSIGNMENT_LVALUE)
{
    using namespace Core;
    Vector v = Vector::ones(3);
    Vector rslt = TestCopyAssignment(v);
    EXPECT_FLOAT_EQ(rslt[0], 1);
    EXPECT_FLOAT_EQ(rslt[1], 1);
    EXPECT_FLOAT_EQ(rslt[2], 1);

    EXPECT_FLOAT_EQ(v[0], 1);
    EXPECT_FLOAT_EQ(v[1], 1);
    EXPECT_FLOAT_EQ(v[2], 1);
}

TEST_F(VectorTest, MOVE_ASSIGNMENT)
{
    using namespace Core;
    Vector v = Vector::ones(3);
    Vector rslt = TestMoveAssignment(std::move(v));

    EXPECT_FLOAT_EQ(rslt[0], 1);
    EXPECT_FLOAT_EQ(rslt[1], 1);
    EXPECT_FLOAT_EQ(rslt[2], 1);

    EXPECT_EQ(v.data(), nullptr);
}

TEST_F(VectorTest, ADDITION)
{
    using namespace Core;
    Vector v1 = Vector::ones(3);
    Vector v2 = Vector::ones(3);
    Vector v = v1 + v2;
    EXPECT_FLOAT_EQ(v[0], 2);
    EXPECT_FLOAT_EQ(v[1], 2);
    EXPECT_FLOAT_EQ(v[2], 2);
}

TEST_F(VectorTest, SUBTRACTION)
{
    using namespace Core;
    Vector v1 = Vector::ones(3);
    Vector v2 = Vector::ones(3);
    Vector v = v1 - v2;
    EXPECT_FLOAT_EQ(v[0], 0);
    EXPECT_FLOAT_EQ(v[1], 0);
    EXPECT_FLOAT_EQ(v[2], 0);
}

TEST_F(VectorTest, SCALE)
{
    using namespace Core;
    Vector v1 = Vector::ones(3);
    Vector v = v1 * 2.5;
    EXPECT_FLOAT_EQ(v[0], 2.5);
    EXPECT_FLOAT_EQ(v[1], 2.5);
    EXPECT_FLOAT_EQ(v[2], 2.5);
}

TEST_F(VectorTest, DOT_PRODUCT)
{
    using namespace Core;
    Vector v1 = Vector::ones(3);
    Vector v2 = Vector::ones(3);
    float rslt = Vector::dot(v1, v2);
    EXPECT_FLOAT_EQ(rslt, 3);
}

TEST_F(VectorTest, NORM)
{
    using namespace Core;
    Vector v1 = Vector::ones(3);
    float rslt = v1.norm();
    float rslt2 = v1.length();
    float rslt3 = v1.magnitude();
    float expected = std::sqrt(3);
    EXPECT_FLOAT_EQ(rslt, expected);
    EXPECT_FLOAT_EQ(rslt2, expected);
    EXPECT_FLOAT_EQ(rslt3, expected);
}

TEST_F(VectorTest, NORMALIZE)
{
    using namespace Core;
    Vector v1 = Vector::ones(3);
    Vector v = Geometry::normalize(v1);
    float expected = std::sqrt(3);
    EXPECT_FLOAT_EQ(v[0], 1 / expected);
    EXPECT_FLOAT_EQ(v[1], 1 / expected);
    EXPECT_FLOAT_EQ(v[2], 1 / expected);
}

TEST_F(VectorTest, CROSS_PRODUCT)
{
    using namespace Core;
    Vector3 v1(1, 0, 0);
    Vector3 v2(0, 1, 0);
    Vector3 v = Vector3::cross(v1, v2);
    EXPECT_FLOAT_EQ(v[0], 0);
    EXPECT_FLOAT_EQ(v[1], 0);
    EXPECT_FLOAT_EQ(v[2], 1);
}

TEST_F(VectorTest, VECTOR3_CONSTRUCTION)
{
    using namespace Core;
    Vector3 v(1, 2, 3);
    EXPECT_FLOAT_EQ(v[0], 1);
    EXPECT_FLOAT_EQ(v[1], 2);
    EXPECT_FLOAT_EQ(v[2], 3);
}

TEST_F(VectorTest, VECTOR3_COPY_CONSTRUCTION_LVALUE)
{
    using namespace Core;
    Vector3 v1(1, 2, 3);
    Vector3 v(v1);
    EXPECT_FLOAT_EQ(v[0], 1);
    EXPECT_FLOAT_EQ(v[1], 2);
    EXPECT_FLOAT_EQ(v[2], 3);
}

TEST_F(VectorTest, VECTOR3_COPY_CONSTRUCTION_RVALUE)
{
    using namespace Core;
    Vector3 v(Vector3(1, 2, 3));
    EXPECT_FLOAT_EQ(v[0], 1);
    EXPECT_FLOAT_EQ(v[1], 2);
    EXPECT_FLOAT_EQ(v[2], 3);
}

TEST_F(VectorTest, VECTOR3_ADDITION)
{
    using namespace Core;
    Vector3 v1(1, 2, 3);
    Vector3 v2(1, 2, 3);
    Vector3 v = v1 + v2;
    EXPECT_FLOAT_EQ(v[0], 2);
    EXPECT_FLOAT_EQ(v[1], 4);
    EXPECT_FLOAT_EQ(v[2], 6);
}

TEST_F(VectorTest, VECTOR3_SUBTRACTION)
{
    using namespace Core;
    Vector3 v1(1, 2, 3);
    Vector3 v2(1, 2, 3);
    Vector3 v = v1 - v2;
    EXPECT_FLOAT_EQ(v[0], 0);
    EXPECT_FLOAT_EQ(v[1], 0);
    EXPECT_FLOAT_EQ(v[2], 0);
}

TEST_F(VectorTest, VECTOR3_SCALE)
{
    using namespace Core;
    Vector3 v1(1, 2, 3);
    Vector3 v = v1 * 2.5;
    EXPECT_FLOAT_EQ(v[0], 2.5);
    EXPECT_FLOAT_EQ(v[1], 5);
    EXPECT_FLOAT_EQ(v[2], 7.5);
}

TEST_F(VectorTest, VECTOR3_DOT_PRODUCT)
{
    using namespace Core;
    Vector3 v1(1, 2, 3);
    Vector3 v2(1, 2, 3);
    float rslt = Vector3::dot(v1, v2);
    EXPECT_FLOAT_EQ(rslt, 14);
}

TEST_F(VectorTest, VECTOR3_CROSS_PRODUCT)
{
    using namespace Core;
    Vector3 v1(1, 0, 0);
    Vector3 v2(0, 1, 0);
    Vector3 v = Vector3::cross(v1, v2);
    EXPECT_FLOAT_EQ(v[0], 0);
    EXPECT_FLOAT_EQ(v[1], 0);
    EXPECT_FLOAT_EQ(v[2], 1);
}

TEST_F(VectorTest, VECTOR3_NORM)
{
    using namespace Core;
    Vector3 v1(1, 2, 3);
    float rslt = v1.norm();
    float rslt2 = v1.length();
    float rslt3 = v1.magnitude();
    float expected = std::sqrt(14);
    EXPECT_FLOAT_EQ(rslt, expected);
    EXPECT_FLOAT_EQ(rslt2, expected);
    EXPECT_FLOAT_EQ(rslt3, expected);
}

TEST_F(VectorTest, VECTOR_MATRIX_MULTIPLICATION)
{
    using namespace Core;
    Vector v1 = Vector::ones(4);
    Matrix m = MatrixS::identity(4);
    Vector v = v1 * m;
    EXPECT_EQ(v.size(), 4);
    EXPECT_FLOAT_EQ(v[0], 1);
    EXPECT_FLOAT_EQ(v[1], 1);
    EXPECT_FLOAT_EQ(v[2], 1);
    EXPECT_FLOAT_EQ(v[3], 1);
}


#include "case_matrix.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Core
{
    TEST(MatrixTest, Constructor)
    {
        Matrix m(2, 3);
        EXPECT_EQ(m.rows(), 2);
        EXPECT_EQ(m.cols(), 3);
        EXPECT_EQ(m.data()[0], 0);
        EXPECT_EQ(m.data()[1], 0);
        EXPECT_EQ(m.data()[2], 0);
        EXPECT_EQ(m.data()[3], 0);
        EXPECT_EQ(m.data()[4], 0);
        EXPECT_EQ(m.data()[5], 0);
    }

    TEST(MatrixTest, CopyConstructor)
    {
        Matrix m(2, 3);
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;
        m(1, 0) = 4;
        m(1, 1) = 5;
        m(1, 2) = 6;

        Matrix m2(m);
        EXPECT_EQ(m2.rows(), 2);
        EXPECT_EQ(m2.cols(), 3);
        EXPECT_EQ(m2.data()[0], 1);
        EXPECT_EQ(m2.data()[1], 2);
        EXPECT_EQ(m2.data()[2], 3);
        EXPECT_EQ(m2.data()[3], 4);
        EXPECT_EQ(m2.data()[4], 5);
        EXPECT_EQ(m2.data()[5], 6);
    }

    TEST(MatrixTest, CopyAssignment)
    {
        Matrix m(2, 3);
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;
        m(1, 0) = 4;
        m(1, 1) = 5;
        m(1, 2) = 6;

        Matrix m2 = m;
        EXPECT_EQ(m2.rows(), 2);
        EXPECT_EQ(m2.cols(), 3);
        EXPECT_EQ(m2.data()[0], 1);
        EXPECT_EQ(m2.data()[1], 2);
        EXPECT_EQ(m2.data()[2], 3);
        EXPECT_EQ(m2.data()[3], 4);
        EXPECT_EQ(m2.data()[4], 5);
        EXPECT_EQ(m2.data()[5], 6);
    }

    TEST(MatrixTest, MoveConstructor)
    {
        Matrix m(2, 3);
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;
        m(1, 0) = 4;
        m(1, 1) = 5;
        m(1, 2) = 6;

        Matrix m2(std::move(m));
        EXPECT_EQ(m2.rows(), 2);
        EXPECT_EQ(m2.cols(), 3);
        EXPECT_EQ(m2.data()[0], 1);
        EXPECT_EQ(m2.data()[1], 2);
        EXPECT_EQ(m2.data()[2], 3);
        EXPECT_EQ(m2.data()[3], 4);
        EXPECT_EQ(m2.data()[4], 5);
        EXPECT_EQ(m2.data()[5], 6);

        EXPECT_EQ(m.data(), nullptr);
    }

    TEST(MatrixTest, MoveAssignment)
    {
        Matrix m(2, 3);
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;
        m(1, 0) = 4;
        m(1, 1) = 5;
        m(1, 2) = 6;

        Matrix m2 = std::move(m);
        EXPECT_EQ(m2.rows(), 2);
        EXPECT_EQ(m2.cols(), 3);
        EXPECT_EQ(m2.data()[0], 1);
        EXPECT_EQ(m2.data()[1], 2);
        EXPECT_EQ(m2.data()[2], 3);
        EXPECT_EQ(m2.data()[3], 4);
        EXPECT_EQ(m2.data()[4], 5);
        EXPECT_EQ(m2.data()[5], 6);

        EXPECT_EQ(m.data(), nullptr);
    }

    TEST(MatrixTest, Destructor)
    {
        Matrix *m = new Matrix(2, 3);
        m->~Matrix();
        EXPECT_EQ(m->data(), nullptr);
    }

    TEST(MatrixTest, Equal)
    {
        Matrix m1(2, 3);
        m1.fill(1);
        m1 = m1 * 2.f;

        Matrix m2(2, 3);
        m2.fill(2);

        EXPECT_TRUE(m1 == m2);
    }

    TEST(MatrixTest, Fill)
    {
        Matrix m(2, 3);
        m.fill(1);
        EXPECT_EQ(m.rows(), 2);
        EXPECT_EQ(m.cols(), 3);
        EXPECT_EQ(m.data()[0], 1);
        EXPECT_EQ(m.data()[1], 1);
        EXPECT_EQ(m.data()[2], 1);
        EXPECT_EQ(m.data()[3], 1);
        EXPECT_EQ(m.data()[4], 1);
        EXPECT_EQ(m.data()[5], 1);
    }

    TEST(MatrixTest, Ones)
    {
        Matrix m = Matrix::ones(2, 3);
        EXPECT_EQ(m.rows(), 2);
        EXPECT_EQ(m.cols(), 3);
        EXPECT_EQ(m.data()[0], 1);
        EXPECT_EQ(m.data()[1], 1);
        EXPECT_EQ(m.data()[2], 1);
        EXPECT_EQ(m.data()[3], 1);
        EXPECT_EQ(m.data()[4], 1);
        EXPECT_EQ(m.data()[5], 1);
    }

    TEST(MatrixTest, Zeros)
    {
        Matrix m = Matrix::zeros(2, 3);
        EXPECT_EQ(m.rows(), 2);
        EXPECT_EQ(m.cols(), 3);
        EXPECT_EQ(m.data()[0], 0);
        EXPECT_EQ(m.data()[1], 0);
        EXPECT_EQ(m.data()[2], 0);
        EXPECT_EQ(m.data()[3], 0);
        EXPECT_EQ(m.data()[4], 0);
        EXPECT_EQ(m.data()[5], 0);
    }

    TEST(MatrixTest, Identity)
    {
        MatrixS m = MatrixS::identity(3);
        EXPECT_EQ(m.rows(), 3);
        EXPECT_EQ(m.cols(), 3);
        EXPECT_EQ(m.data()[0], 1);
        EXPECT_EQ(m.data()[1], 0);
        EXPECT_EQ(m.data()[2], 0);
        EXPECT_EQ(m.data()[3], 0);
        EXPECT_EQ(m.data()[4], 1);
        EXPECT_EQ(m.data()[5], 0);
        EXPECT_EQ(m.data()[6], 0);
        EXPECT_EQ(m.data()[7], 0);
        EXPECT_EQ(m.data()[8], 1);
    }

    TEST(MatrixTest, Transpose)
    {
        Matrix m(2, 3);
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;
        m(1, 0) = 4;
        m(1, 1) = 5;
        m(1, 2) = 6;

        Matrix m2 = m.transpose();
        EXPECT_EQ(m2.rows(), 3);
        EXPECT_EQ(m2.cols(), 2);
        EXPECT_EQ(m2.data()[0], 1);
        EXPECT_EQ(m2.data()[1], 4);
        EXPECT_EQ(m2.data()[2], 2);
        EXPECT_EQ(m2.data()[3], 5);
        EXPECT_EQ(m2.data()[4], 3);
        EXPECT_EQ(m2.data()[5], 6);
    }

    TEST(MatrixTest, Addition)
    {
        Matrix m(2, 3);
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;
        m(1, 0) = 4;
        m(1, 1) = 5;
        m(1, 2) = 6;

        Matrix m2(2, 3);
        m2(0, 0) = 1;
        m2(0, 1) = 2;
        m2(0, 2) = 3;
        m2(1, 0) = 4;
        m2(1, 1) = 5;
        m2(1, 2) = 6;

        Matrix m3 = m + m2;
        EXPECT_EQ(m3.rows(), 2);
        EXPECT_EQ(m3.cols(), 3);
        EXPECT_EQ(m3.data()[0], 2);
        EXPECT_EQ(m3.data()[1], 4);
        EXPECT_EQ(m3.data()[2], 6);
        EXPECT_EQ(m3.data()[3], 8);
        EXPECT_EQ(m3.data()[4], 10);
        EXPECT_EQ(m3.data()[5], 12);
    }

    TEST(MatrixTest, Subtraction)
    {
        Matrix m(2, 3);
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;
        m(1, 0) = 4;
        m(1, 1) = 5;
        m(1, 2) = 6;

        Matrix m2(2, 3);
        m2(0, 0) = 1;
        m2(0, 1) = 2;
        m2(0, 2) = 3;
        m2(1, 0) = 4;
        m2(1, 1) = 5;
        m2(1, 2) = 6;

        Matrix m3 = m - m2;
        EXPECT_EQ(m3.rows(), 2);
        EXPECT_EQ(m3.cols(), 3);
        EXPECT_EQ(m3.data()[0], 0);
        EXPECT_EQ(m3.data()[1], 0);
        EXPECT_EQ(m3.data()[2], 0);
        EXPECT_EQ(m3.data()[3], 0);
        EXPECT_EQ(m3.data()[4], 0);
        EXPECT_EQ(m3.data()[5], 0);
    }

    TEST(MatrixTest, ScalarMultiplication)
    {
        Matrix m(2, 3);
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;
        m(1, 0) = 4;
        m(1, 1) = 5;
        m(1, 2) = 6;

        Matrix m2 = m * 2;
        EXPECT_EQ(m2.rows(), 2);
        EXPECT_EQ(m2.cols(), 3);
        EXPECT_EQ(m2.data()[0], 2);
        EXPECT_EQ(m2.data()[1], 4);
        EXPECT_EQ(m2.data()[2], 6);
        EXPECT_EQ(m2.data()[3], 8);
        EXPECT_EQ(m2.data()[4], 10);
        EXPECT_EQ(m2.data()[5], 12);
    }

    TEST(MatrixTest, MatrixMultiplication)
    {
        Matrix m(2, 3);
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;
        m(1, 0) = 4;
        m(1, 1) = 5;
        m(1, 2) = 6;

        Matrix m2(3, 2);
        m2(0, 0) = 1;
        m2(0, 1) = 2;
        m2(1, 0) = 3;
        m2(1, 1) = 4;
        m2(2, 0) = 5;
        m2(2, 1) = 6;

        Matrix m3 = m * m2;
        EXPECT_EQ(m3.rows(), 2);
        EXPECT_EQ(m3.cols(), 2);
        EXPECT_EQ(m3.data()[0], 22);
        EXPECT_EQ(m3.data()[1], 28);
        EXPECT_EQ(m3.data()[2], 49);
        EXPECT_EQ(m3.data()[3], 64);
    }

    TEST(MatrixTest, MatrixMultiplication2)
    {
        Matrix m(2, 3);
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;
        m(1, 0) = 4;
        m(1, 1) = 5;
        m(1, 2) = 6;

        Matrix m2(3, 2);
        m2(0, 0) = 1;
        m2(0, 1) = 2;
        m2(1, 0) = 3;
        m2(1, 1) = 4;
        m2(2, 0) = 5;
        m2(2, 1) = 6;

        Matrix m3 = m2 * m;
        EXPECT_EQ(m3.rows(), 3);
        EXPECT_EQ(m3.cols(), 3);
        EXPECT_EQ(m3.data()[0], 9);
        EXPECT_EQ(m3.data()[1], 12);
        EXPECT_EQ(m3.data()[2], 15);
        EXPECT_EQ(m3.data()[3], 19);
        EXPECT_EQ(m3.data()[4], 26);
        EXPECT_EQ(m3.data()[5], 33);
        EXPECT_EQ(m3.data()[6], 29);
        EXPECT_EQ(m3.data()[7], 40);
        EXPECT_EQ(m3.data()[8], 51);
    }

    TEST(MatrixTest, MatrixMultiplication3)
    {
        Matrix m1(2, 3);
        m1(0, 0) = 1;
        m1(0, 1) = 2;
        m1(0, 2) = 3;
        m1(1, 0) = 4;
        m1(1, 1) = 5;
        m1(1, 2) = 6;

        Matrix m2(3, 2);
        m2(0, 0) = 1;
        m2(0, 1) = 2;
        m2(1, 0) = 3;
        m2(1, 1) = 4;
        m2(2, 0) = 5;
        m2(2, 1) = 6;

        Matrix m3(2, 2);
        m3(0, 0) = 1;
        m3(0, 1) = 2;
        m3(1, 0) = 3;
        m3(1, 1) = 4;

        Matrix m = m1 * m2 * m3;

        EXPECT_EQ(m.rows(), 2);
        EXPECT_EQ(m.cols(), 2);
        EXPECT_EQ(m.data()[0], 106);
        EXPECT_EQ(m.data()[1], 156);
        EXPECT_EQ(m.data()[2], 241);
        EXPECT_EQ(m.data()[3], 354);
    }

    TEST(MatrixTest, MatrixSwapRows)
    {
        Matrix m = MatrixS::identity(3);
        m.swap_rows(0, 1);
        EXPECT_FLOAT_EQ(m(0, 0), 0);
        EXPECT_FLOAT_EQ(m(0, 1), 1);
        EXPECT_FLOAT_EQ(m(0, 2), 0);
        EXPECT_FLOAT_EQ(m(1, 0), 1);
        EXPECT_FLOAT_EQ(m(1, 1), 0);
        EXPECT_FLOAT_EQ(m(1, 2), 0);
        EXPECT_FLOAT_EQ(m(2, 0), 0);
        EXPECT_FLOAT_EQ(m(2, 1), 0);
        EXPECT_FLOAT_EQ(m(2, 2), 1);
    }

    TEST(MatrixTest, MatrixScaleRow)
    {
        Matrix m = MatrixS::identity(3);
        m.scale_row(1, 2);
        EXPECT_FLOAT_EQ(m(0, 0), 1);
        EXPECT_FLOAT_EQ(m(0, 1), 0);
        EXPECT_FLOAT_EQ(m(0, 2), 0);
        EXPECT_FLOAT_EQ(m(1, 0), 0);
        EXPECT_FLOAT_EQ(m(1, 1), 2);
        EXPECT_FLOAT_EQ(m(1, 2), 0);
        EXPECT_FLOAT_EQ(m(2, 0), 0);
        EXPECT_FLOAT_EQ(m(2, 1), 0);
        EXPECT_FLOAT_EQ(m(2, 2), 1);
    }

    TEST(MatrixTest, MatrixAddRow)
    {
        Matrix m = MatrixS::identity(3);
        m.add_row(0, 1, 2);
        EXPECT_FLOAT_EQ(m(0, 0), 1);
        EXPECT_FLOAT_EQ(m(0, 1), 2);
        EXPECT_FLOAT_EQ(m(0, 2), 0);
        EXPECT_FLOAT_EQ(m(1, 0), 0);
        EXPECT_FLOAT_EQ(m(1, 1), 1);
        EXPECT_FLOAT_EQ(m(1, 2), 0);
        EXPECT_FLOAT_EQ(m(2, 0), 0);
        EXPECT_FLOAT_EQ(m(2, 1), 0);
        EXPECT_FLOAT_EQ(m(2, 2), 1);
    }

    TEST(MatrixTest, MatrixSubmatrix)
    {
        Matrix m = MatrixS::identity(4);
        Matrix m2 = m.submatrix(0, 0);
        auto [rows, cols] = m2.shape();
        EXPECT_EQ(rows, 3);
        EXPECT_EQ(cols, 3);
        EXPECT_FLOAT_EQ(m2(0, 0), 1);
        EXPECT_FLOAT_EQ(m2(0, 1), 0);
        EXPECT_FLOAT_EQ(m2(0, 2), 0);
        EXPECT_FLOAT_EQ(m2(1, 0), 0);
        EXPECT_FLOAT_EQ(m2(1, 1), 1);
        EXPECT_FLOAT_EQ(m2(1, 2), 0);
        EXPECT_FLOAT_EQ(m2(2, 0), 0);
        EXPECT_FLOAT_EQ(m2(2, 1), 0);
        EXPECT_FLOAT_EQ(m2(2, 2), 1);
    }

    TEST(MatrixTest, MatrixVectorMultiplication)
    {
        Matrix m = MatrixS::identity(4);
        Vector v = Vector::ones(4);
        Vector v2 = m * v;
        EXPECT_EQ(v2.size(), 4);
        EXPECT_FLOAT_EQ(v2[0], 1);
        EXPECT_FLOAT_EQ(v2[1], 1);
        EXPECT_FLOAT_EQ(v2[2], 1);
        EXPECT_FLOAT_EQ(v2[3], 1);
    }

    // test look_at
    TEST(MatrixTest, LookAt)
    {
        Vector3 eye = {0.1f, 0.5f, 1.0f};
        Vector3 center = {0.0f, -1.0f, 0.0f};
        Vector3 up = {0.0f, 1.0f, 0.0f};

        Matrix4 m = Geometry::look_at(eye, center, up);

        glm::mat4 glm_m = glm::lookAt(glm::vec3(eye[0], eye[1], eye[2]), glm::vec3(center[0], center[1], center[2]), glm::vec3(up[0], up[1], up[2]));

        std::cout << m << std::endl;
        std::cout << glm::to_string(glm_m) << std::endl;

        EXPECT_TRUE(EQUAL_F(m(0, 0), glm_m[0][0]));
        EXPECT_TRUE(EQUAL_F(m(0, 1), glm_m[1][0]));
        EXPECT_TRUE(EQUAL_F(m(0, 2), glm_m[2][0]));
        EXPECT_TRUE(EQUAL_F(m(0, 3), glm_m[3][0]));

        EXPECT_TRUE(EQUAL_F(m(1, 0), glm_m[0][1]));
        EXPECT_TRUE(EQUAL_F(m(1, 1), glm_m[1][1]));
        EXPECT_TRUE(EQUAL_F(m(1, 2), glm_m[2][1]));
        EXPECT_TRUE(EQUAL_F(m(1, 3), glm_m[3][1]));

        EXPECT_TRUE(EQUAL_F(m(2, 0), glm_m[0][2]));
        EXPECT_TRUE(EQUAL_F(m(2, 1), glm_m[1][2]));
        EXPECT_TRUE(EQUAL_F(m(2, 2), glm_m[2][2]));
        EXPECT_TRUE(EQUAL_F(m(2, 3), glm_m[3][2]));

        EXPECT_TRUE(EQUAL_F(m(3, 0), glm_m[0][3]));
        EXPECT_TRUE(EQUAL_F(m(3, 1), glm_m[1][3]));
        EXPECT_TRUE(EQUAL_F(m(3, 2), glm_m[2][3]));
        EXPECT_TRUE(EQUAL_F(m(3, 3), glm_m[3][3]));
    }

    // test perspective
    TEST(MatrixTest, Perspective)
    {
        Matrix4 view_a = Geometry::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
        glm::mat4 view_b = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

        std::cout << view_a << std::endl;
        std::cout << glm::to_string(view_b) << std::endl;

        EXPECT_TRUE(EQUAL_F(view_a(0, 0), view_b[0][0]));
        EXPECT_TRUE(EQUAL_F(view_a(0, 1), view_b[1][0]));
        EXPECT_TRUE(EQUAL_F(view_a(0, 2), view_b[2][0]));
        EXPECT_TRUE(EQUAL_F(view_a(0, 3), view_b[3][0]));

        EXPECT_TRUE(EQUAL_F(view_a(1, 0), view_b[0][1]));
        EXPECT_TRUE(EQUAL_F(view_a(1, 1), view_b[1][1]));
        EXPECT_TRUE(EQUAL_F(view_a(1, 2), view_b[2][1]));
        EXPECT_TRUE(EQUAL_F(view_a(1, 3), view_b[3][1]));

        EXPECT_TRUE(EQUAL_F(view_a(2, 0), view_b[0][2]));
        EXPECT_TRUE(EQUAL_F(view_a(2, 1), view_b[1][2]));
        EXPECT_TRUE(EQUAL_F(view_a(2, 2), view_b[2][2]));
        EXPECT_TRUE(EQUAL_F(view_a(2, 3), view_b[3][2]));

        EXPECT_TRUE(EQUAL_F(view_a(3, 0), view_b[0][3]));
        EXPECT_TRUE(EQUAL_F(view_a(3, 1), view_b[1][3]));
        EXPECT_TRUE(EQUAL_F(view_a(3, 2), view_b[2][3]));
        EXPECT_TRUE(EQUAL_F(view_a(3, 3), view_b[3][3]));
    }

    // test orthographic
    TEST(MatrixTest, Orthographic)
    {
        Matrix4 view_a = Geometry::orthographic(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
        glm::mat4 view_b = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

        std::cout << view_a << std::endl;
        std::cout << glm::to_string(view_b) << std::endl;

        EXPECT_TRUE(EQUAL_F(view_a(0, 0), view_b[0][0]));
        EXPECT_TRUE(EQUAL_F(view_a(0, 1), view_b[1][0]));
        EXPECT_TRUE(EQUAL_F(view_a(0, 2), view_b[2][0]));
        EXPECT_TRUE(EQUAL_F(view_a(0, 3), view_b[3][0]));

        EXPECT_TRUE(EQUAL_F(view_a(1, 0), view_b[0][1]));
        EXPECT_TRUE(EQUAL_F(view_a(1, 1), view_b[1][1]));
        EXPECT_TRUE(EQUAL_F(view_a(1, 2), view_b[2][1]));
        EXPECT_TRUE(EQUAL_F(view_a(1, 3), view_b[3][1]));

        EXPECT_TRUE(EQUAL_F(view_a(2, 0), view_b[0][2]));
        EXPECT_TRUE(EQUAL_F(view_a(2, 1), view_b[1][2]));
        EXPECT_TRUE(EQUAL_F(view_a(2, 2), view_b[2][2]));
        EXPECT_TRUE(EQUAL_F(view_a(2, 3), view_b[3][2]));

        EXPECT_TRUE(EQUAL_F(view_a(3, 0), view_b[0][3]));
        EXPECT_TRUE(EQUAL_F(view_a(3, 1), view_b[1][3]));
        EXPECT_TRUE(EQUAL_F(view_a(3, 2), view_b[2][3]));
        EXPECT_TRUE(EQUAL_F(view_a(3, 3), view_b[3][3]));
    }

} // namespace Core

#include "case_matrix.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "test_utils.h"
namespace Core
{
    TEST(TestMatrix, Constructor)
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

    TEST(TestMatrix, CopyConstructor)
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

    TEST(TestMatrix, CopyAssignment)
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

    TEST(TestMatrix, MoveConstructor)
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

    TEST(TestMatrix, MoveAssignment)
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

    TEST(TestMatrix, Destructor)
    {
        Matrix *m = new Matrix(2, 3);
        m->~Matrix();
        EXPECT_EQ(m->data(), nullptr);
    }

    TEST(TestMatrix, Equal)
    {
        Matrix m1(2, 3);
        m1.fill(1);
        m1 = m1 * 2.f;

        Matrix m2(2, 3);
        m2.fill(2);

        EXPECT_TRUE(m1 == m2);
    }

    TEST(TestMatrix, Fill)
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

    TEST(TestMatrix, Ones)
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

    TEST(TestMatrix, Zeros)
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

    TEST(TestMatrix, Identity)
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

    TEST(TestMatrix, Transpose)
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

    TEST(TestMatrix, Addition)
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

    TEST(TestMatrix, Subtraction)
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

    TEST(TestMatrix, ScalarMultiplication)
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

    TEST(TestMatrix, MatrixMultiplication)
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

    TEST(TestMatrix, MatrixMultiplication2)
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

    TEST(TestMatrix, MatrixMultiplication3)
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

    TEST(TestMatrix, MatrixSwapRows)
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

    TEST(TestMatrix, MatrixScaleRow)
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

    TEST(TestMatrix, MatrixAddRow)
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

    TEST(TestMatrix, MatrixSubmatrix)
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

    TEST(TestMatrix, MatrixVectorMultiplication)
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
    TEST(TestMatrix, LookAt)
    {
        Vector3 eye = {0.1f, 0.5f, 1.0f};
        Vector3 center = {0.0f, -1.0f, 0.0f};
        Vector3 up = {0.0f, 1.0f, 0.0f};

        Matrix4 m = Geometry::look_at(eye, center, up);

        glm::mat4 glm_m = glm::lookAt(glm::vec3(eye[0], eye[1], eye[2]), glm::vec3(center[0], center[1], center[2]), glm::vec3(up[0], up[1], up[2]));

        std::cout << m << std::endl;
        std::cout << glm::to_string(glm_m) << std::endl;

        EXPECT_TRUE(Expect_Matrix_Equal(m, glm::transpose(glm_m)));
    }

    // test perspective
    TEST(TestMatrix, Perspective)
    {
        Matrix4 view_a = Geometry::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
        glm::mat4 view_b = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

        std::cout << view_a << std::endl;
        std::cout << glm::to_string(view_b) << std::endl;

        EXPECT_TRUE(Expect_Matrix_Equal(view_a, glm::transpose(view_b)));
    }

    // test orthographic
    TEST(TestMatrix, Orthographic)
    {
        Matrix4 view_a = Geometry::orthographic(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
        glm::mat4 view_b = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

        std::cout << view_a << std::endl;
        std::cout << glm::to_string(view_b) << std::endl;

        EXPECT_TRUE(Expect_Matrix_Equal(view_a, glm::transpose(view_b)));
    }

    // test inverse
    TEST(TestMatrix, inverse)
    {
        Matrix4 m1 = {
            1, 2, 3, 4,
            0, 1, 2, 3,
            0, 0, 1, 2,
            0, 0, 0, 1};

        auto inv_m1 = m1.inverse();

        glm::mat4 glm_m1 = {
            {1, 2, 3, 4},
            {0, 1, 2, 3},
            {0, 0, 1, 2},
            {0, 0, 0, 1}};

        glm::mat4 glm_inv_m1 = glm::inverse(glm_m1);

        std::cout << "INVERSE: " << std::endl
                  << inv_m1 << std::endl;
        std::cout << "INVERSE GLM: " << std::endl
                  << glm_inv_m1 << std::endl;

        EXPECT_TRUE(Expect_Matrix_Equal(inv_m1, glm::transpose(glm_inv_m1)));
    }

    // test transpose
    TEST(TestMatrix, transpose)
    {
        float v[][4] = {
            {1, 2, 3, 4},
            {0, 1, 2, 3},
            {0, 0, 1, 2},
            {0, 0, 0, 1}};

        Matrix4 m;
        glm::mat4 glm_m;

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                m(i, j) = v[i][j];
                glm_m[i][j] = v[i][j];
            }
        }
        std::cout << "m: " << std::endl
                  << m << std::endl;
        std::cout << "glm: " << std::endl
                  << glm_m << std::endl;

        EXPECT_TRUE(Expect_Matrix_Equal(m, glm_m));
    }

    TEST(TestMatrix, normal_matrix)
    {
        auto m1 = Matrix4::identity();
        m1 = Geometry::translate(m1, {1, 2, 3});
        m1 = Geometry::rotate(m1, glm::radians(45.0f), {1, 0, 0});
        m1 = Geometry::scale(m1, {1, 2, 3});

        auto normal_m1 = m1.inverse().transpose();

        glm::mat4 glm_m1 = glm::mat4(1.0f);
        glm_m1 = glm::translate(glm_m1, glm::vec3(1, 2, 3));
        glm_m1 = glm::rotate(glm_m1, glm::radians(45.0f), glm::vec3(1, 0, 0));
        glm_m1 = glm::scale(glm_m1, glm::vec3(1, 2, 3));

        glm::mat4 glm_normal_m1 = glm::transpose(glm::inverse(glm_m1));

        std::cout << "NORMAL: " << std::endl
                  << m1 << std::endl;
        std::cout << "NORMAL_GLM: " << std::endl
                  << glm_m1 << std::endl;

        EXPECT_TRUE(Expect_Matrix_Equal(normal_m1, glm_normal_m1));
    }

} // namespace Core

#include "case_matrix.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "test_utils.h"
#include <glm/gtc/type_ptr.hpp>
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
        std::cout << m << std::endl;
        Vector v = Vector::ones(4);
        std::cout << v << std::endl;
        Vector v2 = m * v;
        std::cout << v2 << std::endl;
        EXPECT_EQ(v2.size(), 4);
        EXPECT_FLOAT_EQ(v2[0], 1);
        EXPECT_FLOAT_EQ(v2[1], 1);
        EXPECT_FLOAT_EQ(v2[2], 1);
        EXPECT_FLOAT_EQ(v2[3], 1);
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
            1, 2, 3, 4,
            0, 1, 2, 3,
            0, 0, 1, 2,
            0, 0, 0, 1};

        glm::mat4 glm_inv_m1 = glm::inverse(glm_m1);

        std::cout << "INVERSE: " << std::endl
                  << inv_m1 << std::endl;
        std::cout << "INVERSE GLM: " << std::endl
                  << glm_inv_m1 << std::endl;

        EXPECT_TRUE(Expect_Matrix_Equal(inv_m1, glm_inv_m1));
    }

    // test transpose
    TEST(TestMatrix, transpose)
    {
        float v[16] = {
            1, 2, 3, 4,
            0, 1, 2, 3,
            0, 0, 1, 2,
            0, 0, 0, 1};

        Matrix4 m(v, true);

        Matrix4 mt = m.transpose();
        glm::mat4 glm_m = glm::make_mat4(v);

        auto glmt = glm::transpose(glm_m);

        std::cout << "m: " << std::endl
                  << mt << std::endl;
        std::cout << "glm: " << std::endl
                  << glmt << std::endl;

        EXPECT_TRUE(Expect_Matrix_Equal(mt, glmt));
    }

    TEST(TestMatrix, normal_matrix)
    {
        auto m1 = Matrix4::identity();
        m1 = Geometry::translate(m1, {1, 2, 3});
        m1 = Geometry::rotate(m1, glm::radians(45.0f), {1, 0, 0});
        m1 = Geometry::scale(m1, {1, 2, 3});

        Matrix3 normal_mat = m1.inverse().transpose();

        glm::mat4 glm_m1 = glm::mat4(1.0f);
        glm_m1 = glm::translate(glm_m1, glm::vec3(1, 2, 3));
        glm_m1 = glm::rotate(glm_m1, glm::radians(45.0f), glm::vec3(1, 0, 0));
        glm_m1 = glm::scale(glm_m1, glm::vec3(1, 2, 3));

        glm::mat3 glm_normal_mat = glm::transpose(glm::inverse(glm_m1));

        std::cout << "NORMAL: " << std::endl
                  << normal_mat << std::endl;
        std::cout << "NORMAL_GLM: " << std::endl
                  << glm_normal_mat << std::endl;

        EXPECT_TRUE(Expect_Matrix_Equal(normal_mat, glm_normal_mat));
    }

    TEST(TestMatrix, construction)
    {
        glm::vec4 col_0 = {1, 2, 3, 4};
        glm::vec4 col_1 = {5, 6, 7, 8};
        glm::vec4 col_2 = {9, 10, 11, 12};
        glm::vec4 col_3 = {13, 14, 15, 16};

        glm::mat4 m(col_0, col_1, col_2, col_3);

        float *values = glm::value_ptr(m);

        for (int i = 0; i < 16; i++)
        {
            if (i % 4 == 0)
                std::cout << std::endl;

            std::cout << values[i] << " ";
        }

        std::cout << "m: " << std::endl
                  << m << std::endl;
    }

} // namespace Core

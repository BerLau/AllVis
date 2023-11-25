#include <gtest/gtest.h>
#include <cmath>
#include <cstdio>
#include <string>
#include "test_utils.h"

TEST(MathTest, Sign)
{
    EXPECT_EQ(1, Arithmetics::sign(1));
    EXPECT_EQ(-1, Arithmetics::sign(-1));
    EXPECT_EQ(0, Arithmetics::sign(0));
}

TEST(MathTest, Sqrt)
{
    double a = 3;
    double r1 = std::sqrt(a);
    double r2 = Arithmetics::sqrt(a);
    EXPECT_FLOAT_EQ(r1, r2);
}

TEST(MathTest, Pow)
{
    double x = 2.7;
    int n = 3;
    double r1 = std::pow(x, n);
    double r2 = Arithmetics::pow(x, n);
    EXPECT_FLOAT_EQ(r1, r2);
    n = -3;
    r1 = std::pow(x, n);
    r2 = Arithmetics::pow(x, n);
    EXPECT_FLOAT_EQ(r1, r2);
    n = 0;
    r1 = std::pow(x, n);
    r2 = Arithmetics::pow(x, n);
    EXPECT_FLOAT_EQ(r1, r2);
}
#include <gtest/gtest.h>
#include "tools.h"

TEST(Tools, split_str)
{
    std::string str = "1,2,;3,4,;5,6,7,;8,9,;10;";
    std::string delim = ";";
    std::vector<std::string> result =Core:: Tools::split_str(str, delim);
    ASSERT_EQ(result.size(), 6);
    ASSERT_EQ(result[0], "1,2,");
    ASSERT_EQ(result[1], "3,4,");
    ASSERT_EQ(result[2], "5,6,7,");
    ASSERT_EQ(result[3], "8,9,");
    ASSERT_EQ(result[4], "10");
    ASSERT_EQ(result[5], "");
}

TEST(Tools, reverse_str)
{
    std::string s = "abcdefghijklmn";
    std::string r = "nmlkjihgfedcba";
   Core:: Tools::reverse_str(r.data(),s.length());
    EXPECT_TRUE(s == r);
}
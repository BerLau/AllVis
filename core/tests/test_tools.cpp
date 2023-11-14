#include <gtest/gtest.h>
#include "core.h"

TEST(Tools, split_str)
{
    std::string str = "1,2,;3,4,;5,6,7,;8,9,;10;";
    std::string delim = ";";
    std::vector<std::string> result = Tools::split_str(str, delim);
    ASSERT_EQ(result.size(), 6);
    ASSERT_EQ(result[0], "1,2,");
    ASSERT_EQ(result[1], "3,4,");
    ASSERT_EQ(result[2], "5,6,7,");
    ASSERT_EQ(result[3], "8,9,");
    ASSERT_EQ(result[4], "10");
    ASSERT_EQ(result[5], "");
}
#include <gtest/gtest.h>
#include <string>
#include "max_subsequence.h"

using namespace std;

TEST(NormalCase, SameLength)
{
    string str1("abcdef"), str2("defabc");
    string res = get_max_subsequence(str1, str2);
    ASSERT_TRUE((res == "abc") || (res == "def"));
}

TEST(NormalCase, DifferentLength)
{
    string str1("abcdefg"), str2("defgabc");
    string res = get_max_subsequence(str1, str2);
    ASSERT_EQ(res, "defg");
}

TEST(NormalCase, Sequence)
{
    string str1("axbycd"), str2("abcd");
    string res = get_max_subsequence(str1, str2);
    ASSERT_EQ(res, "abcd");
}

TEST(CornerCase, OneEmpty)
{
    string str1("abcdef"), str2;
    string res = get_max_subsequence(str1, str2);
    ASSERT_TRUE(res.empty());
}

TEST(CornerCase, BothEmpty)
{
    string str1, str2;
    string res = get_max_subsequence(str1, str2);
    ASSERT_TRUE(res.empty());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

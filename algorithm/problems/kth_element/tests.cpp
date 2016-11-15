#include <gtest/gtest.h>
#include <vector>
#include <ctime>
#include "kth_element.h"
#include <iostream>
using namespace std;

TEST(NormalCase, OneElement)
{
    vector<int> a = {1};
    int value = 0;
    bool res = false;

    res = kth_element(a, 1, value);
    ASSERT_TRUE(res && (value == 1));
}

TEST(NormalCase, Sorted)
{
    vector<int> a = {1, 2, 3, 4, 5};
    int value = 0;
    bool res = false;

    res = kth_element(a, 1, value);
    ASSERT_TRUE(res && (value == 1));

    res = kth_element(a, 3, value);
    cout << res << endl << value << endl;
    ASSERT_TRUE(res && (value == 3));

    res = kth_element(a, 5, value);
    ASSERT_TRUE(res && (value == 5));
}

TEST(NormalCase, Unsorted)
{
    vector<int> a = {3, 1, 4, 2, 5};
    int value = 0;
    bool res = false;

    res = kth_element(a, 1, value);
    ASSERT_TRUE(res && (value == 1));

    res = kth_element(a, 3, value);
    ASSERT_TRUE(res && (value == 3));

    res = kth_element(a, 5, value);
    ASSERT_TRUE(res && (value == 5));
}

TEST(ErrorCase, Notfound)
{
    vector<int> a = {3, 1, 4, 2, 5};
    int value = 0;
    bool res = true;

    res = kth_element(a, 0, value);
    ASSERT_FALSE(res);

    res = kth_element(a, 6, value);
    ASSERT_FALSE(res);

    a.clear();
    res = kth_element(a, 1, value);
    ASSERT_FALSE(res);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <algorithm>
#include "trie.h"

using namespace std;

TEST(Add, Once)
{
    string str = {"abc"};
    vector<string> match = {str};

    Trie t;
    t.Add(str);
    vector<string> res;
    t.GetContents(res);
    auto p = t.Find(str);

    ASSERT_EQ(p.first->value, 'b');
    ASSERT_EQ(p.second->value, 'c');
    ASSERT_TRUE(std::equal(res.begin(), res.end(), match.begin()));
}

TEST(Add, Twice)
{
    string str = {"abc"};
    vector<string> match = {str, str};

    Trie t;
    t.Add(str);
    t.Add(str);
    vector<string> res;
    t.GetContents(res);

    ASSERT_TRUE(std::equal(res.begin(), res.end(), match.begin()));
}

TEST(Add, Prefix)
{
    string prefix = {"abc"};
    string str = {"abcdef"};
    vector<string> match = {prefix, str};

    Trie t;
    t.Add(prefix);
    t.Add(str);
    vector<string> res;
    t.GetContents(res);

    ASSERT_TRUE(std::equal(res.begin(), res.end(), match.begin()));
}

TEST(Add, Branch)
{
    string prefix = {"abc"};
    string branch1 = {"abcdef"};
    string branch2 = {"abcghi"};
    vector<string> match = {prefix, branch1, branch2};

    Trie t;
    t.Add(prefix);
    t.Add(branch1);
    t.Add(branch2);
    vector<string> res;
    t.GetContents(res);

    ASSERT_TRUE(std::equal(res.begin(), res.end(), match.begin()));
}

TEST(Remove, Empty)
{
    string str = {"abc"};
    Trie t;
    vector<string> res;
    t.GetContents(res);
    auto p = t.Find(str);
    ASSERT_TRUE(res.empty());
    ASSERT_FALSE(t.Remove(str));
    ASSERT_TRUE(!p.first && !p.second);
}

TEST(Remove, Prefix)
{
    string prefix = {"abc"};
    string str = {"abcdef"};
    vector<string> match = {str};

    Trie t;
    t.Add(prefix);
    t.Add(str);
    t.Remove(prefix);
    vector<string> res;
    t.GetContents(res);

    ASSERT_TRUE(std::equal(res.begin(), res.end(), match.begin()));
}

TEST(Remove, Suffix)
{
    string prefix = {"abc"};
    string str = {"abcdef"};
    vector<string> match = {prefix};

    Trie t;
    t.Add(prefix);
    t.Add(str);
    t.Remove(str);
    vector<string> res;
    t.GetContents(res);

    ASSERT_TRUE(std::equal(res.begin(), res.end(), match.begin()));
}

TEST(Remove, Once)
{
    string str = {"abc"};
    vector<string> match = {str};

    Trie t;
    t.Add(str);
    t.Add(str);
    t.Remove(str);
    vector<string> res;
    t.GetContents(res);

    ASSERT_TRUE(std::equal(res.begin(), res.end(), match.begin()));
}

TEST(Remove, All)
{
    string prefix = {"abc"};
    string str = {"abcdef"};
    vector<string> match;

    Trie t;
    t.Add(prefix);
    t.Add(prefix);
    t.Add(str);
    t.Remove(str);
    t.Remove(prefix);
    t.Remove(prefix);
    vector<string> res;
    t.GetContents(res);

    ASSERT_TRUE(std::equal(res.begin(), res.end(), match.begin()));
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

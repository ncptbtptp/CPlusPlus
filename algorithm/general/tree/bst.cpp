#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <iostream>
using namespace std;


struct tNode
{
    tNode(int d):
        data(d),
        left(NULL),
        right(NULL)
    {}
    int data;
    tNode *left;
    tNode *right;
};


class BST
{
public:
    BST(): m_root(NULL)
    {}

    void InOrderTraverse(vector<int>& result);

    tNode *Insert(int data);
    // Return true if found and successfully removed
    bool Remove(int data);
private:
    void InOrderTraversalHelper(tNode *node, vector<int>& result);
    bool Find(int data, tNode *&parent, tNode *&found);
    tNode *m_root;
};


void BST::InOrderTraversalHelper(tNode *node, vector<int>& result)
{
    if (!node) return;
    InOrderTraversalHelper(node->left, result);
    result.push_back(node->data);
    InOrderTraversalHelper(node->right, result);
}


bool BST::Find(int data, tNode *&parent, tNode *&found)
{
    tNode *cur = m_root;
    tNode *curParent = NULL;

    while (cur)
    {
        if (data == cur->data)
        {
            parent = curParent;
            found = cur;
            return true;
        }
        
        if (data < cur->data)
        {
            curParent = cur;
            cur = cur->left;
        }
        else
        {
            curParent = cur;
            cur = cur->right;
        }
    }
    return false;
}


void BST::InOrderTraverse(vector<int>& result)
{
    result.clear();
    InOrderTraversalHelper(m_root, result);
}


tNode* BST::Insert(int data)
{
    if (!m_root)
    {
        m_root = new tNode(data);
        return m_root;
    }

    // Use one pointer instead of parent-child two pointers
    // New node must be a leaf.
    tNode *ptr = m_root;
    while (true)
    {
        if (data <= ptr->data)
        {
            if (!ptr->left)
            {
                ptr->left = new tNode(data);
                return ptr->left;
            }
            ptr = ptr->left;
        }
        else
        {
            if (!ptr->right)
            {
                ptr->right = new tNode(data);
                return ptr->right;
            }
            ptr = ptr->right;
        }
    }
}


bool BST::Remove(int data)
{
    if (!m_root) return false;

    tNode *found = NULL, *parent = NULL;
    // O(h)
    if (!Find(data, parent, found))
        return false;

    if (!found->left)
    {
        if (found == m_root)
            m_root = found->right;
        else if (data < parent->data)
            parent->left = found->right;
        else
            parent->right = found->right;
    }
    else if (!found->right)
    {
        if (found == m_root)
            m_root = found->left;
        else if (data < parent->data)
            parent->left = found->left;
        else
            parent->right = found->left;
    }
    else
    {
        // Find next node in in-order sequence
        tNode *next = found->right, *nextParent = found;
        while (next->left)
        {
            nextParent = next;
            next = next->left;
        }

        // Two cases: see CLRS
        if (next == found->right)
        {
            next->left = found->left;
        }
        else
        {
            // next must not have left child, and must be the left child of nextParent.
            nextParent->left = next->right;
            next->left = found->left;
            next->right = found->right;
        }

        if (found == m_root)
        {
            m_root = next;
        }
        else
        {
            if (data < parent->data)
                parent->left = next;
            else
                parent->right = next;
        }
    }

    delete found;
    return true;
}


TEST(BSTTest, InsertTest)
{
    vector<vector<int>> inputs = {{1, 2, 3, 4, 5, 6}, {6, 3, 4, 2, 7, 8}};
    vector<vector<int>> expected = {{1, 2, 3, 4, 5, 6}, {2, 3, 4, 6, 7, 8}};

    for (size_t i = 0; i < inputs.size(); ++i)
    {
        BST tree;
        for (auto data : inputs[i])
        {
            tree.Insert(data);
        }
        vector<int> res;
        tree.InOrderTraverse(res);
        EXPECT_THAT(expected[i], ::testing::ContainerEq(res));
    }
}


TEST(BSTTest, RemoveTest)
{
    vector<vector<int>> inputs = {
    {1},                    // Single node
    {2, 1, 3},              // Leaf
    {5, 4, 3, 2, 6},        // Single child, not-root
    {5, 6},                 // Single child, root
    {5, 2, 1, 3, 6},        // Both childs, not-root 
    {5, 2, 6},              // Both childs, root
    {8, 4, 3, 7, 5, 6, 9},  // Both childs, not-right-child, not-root
    {4, 3, 7, 5, 6},        // Both childs, not-right-child, root
    };
    vector<int> removes = {1, 1, 3, 5, 2, 5, 4, 4};
    vector<vector<int>> expected = {{}, {2, 3}, {2, 4, 5, 6}, {6}, {1, 3, 5, 6}, {2, 6}, {3, 5, 6, 7, 8, 9}, {3, 5, 6, 7}};

    for (size_t i = 0; i < inputs.size(); ++i)
    {
        BST tree;
        for (auto data : inputs[i])
        {
            tree.Insert(data);
        }
        tree.Remove(removes[i]);
        vector<int> res;
        tree.InOrderTraverse(res);
        EXPECT_THAT(expected[i], ::testing::ContainerEq(res));
    }
}


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



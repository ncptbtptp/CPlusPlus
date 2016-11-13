#include "trie.h"
#include <gtest/gtest.h>
#include <stack>
#include <algorithm>
using namespace std;

void Trie::Add(const string &str)
{
    Node *cur = &m_root;
    // Loop invariant:
    // (1) @cur points to node whose value is already set
    // (2) @cur->next should be updated if new children
    for (size_t i = 0; i < str.length(); ++i) {
        Node * &next_p = cur->next[str[i] - 'a'];
        if (next_p == nullptr) {
            next_p = new Node(str[i]);
            cur = next_p;
        }
        cur = next_p;
    }

    // When loop ends, @cur points to the leaf node (last char in string)
    cur->existed = true;
    ++cur->count;
}

// Another way to implement Remove() is to first find() to test existence,
// and then remove the chain in a top-down fasion. It's O(len(key)) either way.
bool Trie::Remove(const string &str)
{
    stack<Node*> pre;
    Node *p = nullptr, *q = &m_root;
    for (size_t i = 0; i < str.length(); ++i) {
        p = q;
        q = p->next[str[i] - 'a'];
        // str[i] is not found!
        if (q == nullptr) {
            return false;
        }
        pre.push(p);
    }

    pre.push(q);

    // Remove nodes
    char ch = '0';
    while (!pre.empty()) {
        Node* cur = pre.top();
        pre.pop();
        // Which child is removed?
        if (ch != '0') {
            cur->next[ch - 'a'] = nullptr;
        }

        if (--cur->count > 0) {
            break;
        }

        ch = cur->value;
        delete cur;
    }

    return true;
}

std::pair<Node*, Node*> Trie::Find(const string &str)
{
    Node *p = nullptr;
    Node *q = &m_root;

    // Loop invariant:
    // @q: points to the current character of str
    // @p: points to the its parent
    for (size_t i = 0; i < str.length(); ++i) {
        p = q;
        q = p->next[str[i] - 'a'];
        if (q == nullptr) {
            return std::make_pair(nullptr, nullptr);
        }
    }

    return std::make_pair(p, q);
}

void Trie::GetContents(vector<string> &res) const
{
    GetHelper(&m_root, string(), res);
}

// DFS
void Trie::GetHelper(const Node *start, string pre, vector<string> &res) const
{
    // Don't include root's value!
    if (start->value != '0') {
        pre += start->value;
    }

    if (start->existed) {
        // There can be multiple instances of this string stored in Trie
        for (size_t i = 0; i < start->count; ++i) {
            res.push_back(pre);
        }
    }

    for (auto next : start->next) {
        if (next != nullptr) {
            GetHelper(next, pre, res);
        }
    }
}

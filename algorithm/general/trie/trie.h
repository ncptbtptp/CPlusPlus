#include <string>
#include <tuple> // std::pair, std::make_pair()
#include <vector>

    struct Node
    {
        bool existed {false};
        char value;
        Node *next [26] {};
        size_t count {0};       // E.g. handle mutliple instances of "abc", "abc"

        Node (char val = '0'): value(val)
        {}
    };
// Assume we only store low-case characters in trie
class Trie
{
public:
    void Add(const std::string &str);

    // Remove() is not trivial since we may need to delete all our way
    // back to the root!
    // Return: true if successfully deleted; false if not found
    bool Remove(const std::string &str);

    // Return pair of (parent_ptr, std::string_ptr)
    // Return (nullptr, nullptr) if not found
    std::pair<Node*, Node*> Find(const std::string &str);

    // Return all existed std::strings
    void GetContents(std::vector<std::string> &res) const;

private:
    void GetHelper(const Node *start, std::string pre, std::vector<std::string> &res) const;

    // Root of trie is "empty": doesn't contain any value
    Node m_root;
};

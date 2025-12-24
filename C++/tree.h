#ifndef TREE_H
#define TREE_H

#include <iosfwd>
#include <string>

class Tree {
private:
    struct Node {
        int value{0};
        Node* left{nullptr};
        Node* right{nullptr};
        explicit Node(int v) : value(v) {}
    };

    Node* root_{nullptr};

    static void destroy(Node* node);
    static void insert_bst(Node*& node, int value);
    static bool find_bst(const Node* node, int value);
    static void preorder(const Node* node, std::string& out);

    static bool is_full_impl(const Node* node);

    static void serialize_bin_node(std::ostream& os, const Node* node);
    static Node* deserialize_bin_node(std::istream& is);

    static void serialize_text_node(std::ostream& os, const Node* node);
    static Node* deserialize_text_node(std::istream& is);

public:
    std::string name;

    explicit Tree(std::string name);
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;
    ~Tree();

    void insert(int value);
    bool find(int value) const;
    std::string read() const;
    bool is_full() const;

    void serialize_binary(std::ostream& os) const;
    void deserialize_binary(std::istream& is);
    void serialize_text(std::ostream& os) const;
    void deserialize_text(std::istream& is);
};

#endif

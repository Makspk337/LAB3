#include "tree.h"

#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

Tree::Tree(std::string name) : name(std::move(name)) {}

Tree::~Tree() { destroy(root_); }

void Tree::destroy(Node* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

void Tree::insert_bst(Node*& node, int value) {
    if (!node) {
        node = new Node(value);
        return;
    }
    if (value < node->value) {
        insert_bst(node->left, value);
    } else if (value > node->value) {
        insert_bst(node->right, value);
    }
}

bool Tree::find_bst(const Node* node, int value) {
    if (!node) return false;
    if (value == node->value) return true;
    if (value < node->value) return find_bst(node->left, value);
    return find_bst(node->right, value);
}

void Tree::preorder(const Node* node, std::string& out) {
    if (!node) return;
    if (!out.empty()) out += ' ';
    out += std::to_string(node->value);
    preorder(node->left, out);
    preorder(node->right, out);
}

bool Tree::is_full_impl(const Node* node) {
    if (!node) return true;
    const bool has_left = node->left != nullptr;
    const bool has_right = node->right != nullptr;
    if (has_left != has_right) {
        return false;
    }
    return is_full_impl(node->left) && is_full_impl(node->right);
}

void Tree::insert(int value) { insert_bst(root_, value); }

bool Tree::find(int value) const { return find_bst(root_, value); }

std::string Tree::read() const {
    std::string out;
    preorder(root_, out);
    return out;
}

bool Tree::is_full() const { return is_full_impl(root_); }

void Tree::serialize_bin_node(std::ostream& os, const Node* node) {
    const unsigned char present = node ? 1 : 0;
    os.write(reinterpret_cast<const char*>(&present), sizeof(present));
    if (!node) return;
    os.write(reinterpret_cast<const char*>(&node->value), sizeof(node->value));
    serialize_bin_node(os, node->left);
    serialize_bin_node(os, node->right);
}

Tree::Node* Tree::deserialize_bin_node(std::istream& is) {
    unsigned char present = 0;
    is.read(reinterpret_cast<char*>(&present), sizeof(present));
    if (!present) return nullptr;
    int v = 0;
    is.read(reinterpret_cast<char*>(&v), sizeof(v));
    auto* node = new Node(v);
    node->left = deserialize_bin_node(is);
    node->right = deserialize_bin_node(is);
    return node;
}

void Tree::serialize_binary(std::ostream& os) const { serialize_bin_node(os, root_); }

void Tree::deserialize_binary(std::istream& is) {
    destroy(root_);
    root_ = deserialize_bin_node(is);
}

void Tree::serialize_text_node(std::ostream& os, const Node* node) {
    if (!node) {
        os << "# ";
        return;
    }
    os << node->value << ' ';
    serialize_text_node(os, node->left);
    serialize_text_node(os, node->right);
}

Tree::Node* Tree::deserialize_text_node(std::istream& is) {
    std::string tok;
    if (!(is >> tok)) return nullptr;
    if (tok == "#") return nullptr;
    int v = std::stoi(tok);
    auto* node = new Node(v);
    node->left = deserialize_text_node(is);
    node->right = deserialize_text_node(is);
    return node;
}

void Tree::serialize_text(std::ostream& os) const { serialize_text_node(os, root_); }

void Tree::deserialize_text(std::istream& is) {
    destroy(root_);
    root_ = deserialize_text_node(is);
}

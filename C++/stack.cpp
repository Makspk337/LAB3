#include "stack.h"

#include <istream>
#include <ostream>
#include <utility>
#include <vector>

namespace {
void write_string_bin(std::ostream& os, const std::string& s) {
    const std::uint32_t n = static_cast<std::uint32_t>(s.size());
    os.write(reinterpret_cast<const char*>(&n), sizeof(n));
    os.write(s.data(), static_cast<std::streamsize>(n));
}
std::string read_string_bin(std::istream& is) {
    std::uint32_t n = 0;
    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    std::string s(n, '\0');
    if (n > 0) {
        is.read(&s[0], static_cast<std::streamsize>(n));
    }
    return s;
}
}

Stack::Stack(std::string name) : name(std::move(name)) {}

Stack::~Stack() { clear(); }

void Stack::clear() {
    Node* cur = top_;
    while (cur) {
        Node* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    top_ = nullptr;
}

void Stack::push(const std::string& data) {
    auto* n = new Node(data);
    n->next = top_;
    top_ = n;
}

std::string Stack::pop() {
    if (!top_) {
        return "";
    }
    Node* del = top_;
    std::string out = del->data;
    top_ = top_->next;
    delete del;
    return out;
}

std::string Stack::peek() const { return top_ ? top_->data : ""; }

bool Stack::is_empty() const { return top_ == nullptr; }

std::string Stack::read() const {
    std::string out;
    bool first = true;
    for (Node* cur = top_; cur; cur = cur->next) {
        if (!first) {
            out += ' ';
        }
        first = false;
        out += cur->data;
    }
    return out;
}

void Stack::serialize_binary(std::ostream& os) const {
    std::vector<std::string> values;
    for (Node* cur = top_; cur; cur = cur->next) {
        values.push_back(cur->data);
    }
    const std::uint32_t sz = static_cast<std::uint32_t>(values.size());
    os.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    for (const auto& v : values) {
        write_string_bin(os, v);
    }
}

void Stack::deserialize_binary(std::istream& is) {
    clear();
    std::uint32_t sz = 0;
    is.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    std::vector<std::string> values;
    values.reserve(sz);
    for (std::uint32_t i = 0; i < sz; ++i) {
        values.push_back(read_string_bin(is));
    }
    for (std::size_t i = values.size(); i-- > 0;) {
        push(values[i]);
    }
}

void Stack::serialize_text(std::ostream& os) const {
    std::vector<std::string> values;
    for (Node* cur = top_; cur; cur = cur->next) {
        values.push_back(cur->data);
    }
    os << values.size() << "\n";
    for (const auto& v : values) {
        os << v << "\n";
    }
}

void Stack::deserialize_text(std::istream& is) {
    clear();
    int sz = 0;
    if (!(is >> sz)) {
        return;
    }
    std::string dummy;
    std::getline(is, dummy);
    std::vector<std::string> values;
    values.reserve(static_cast<std::size_t>(sz));
    for (int i = 0; i < sz; ++i) {
        std::string line;
        std::getline(is, line);
        values.push_back(line);
    }
    for (std::size_t i = values.size(); i-- > 0;) {
        push(values[i]);
    }
}

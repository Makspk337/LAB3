#include "forward_list.h"

#include <istream>
#include <ostream>
#include <sstream>
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

Forward_List::Forward_List(std::string name) : name(std::move(name)) {}

Forward_List::~Forward_List() { clear(); }

void Forward_List::clear() {
    Node* cur = head_;
    while (cur) {
        Node* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head_ = nullptr;
}

void Forward_List::push_head(const std::string& value) {
    auto* n = new Node(value);
    n->next = head_;
    head_ = n;
}

void Forward_List::push_tail(const std::string& value) {
    auto* n = new Node(value);
    if (!head_) {
        head_ = n;
        return;
    }
    Node* cur = head_;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = n;
}

void Forward_List::push_before(const std::string& before, const std::string& value) {
    if (!head_) {
        return;
    }
    if (head_->value == before) {
        push_head(value);
        return;
    }
    Node* prev = head_;
    Node* cur = head_->next;
    while (cur) {
        if (cur->value == before) {
            auto* n = new Node(value);
            prev->next = n;
            n->next = cur;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void Forward_List::push_after(const std::string& after, const std::string& value) {
    Node* cur = head_;
    while (cur) {
        if (cur->value == after) {
            auto* n = new Node(value);
            n->next = cur->next;
            cur->next = n;
            return;
        }
        cur = cur->next;
    }
}

void Forward_List::delete_head() {
    if (!head_) {
        return;
    }
    Node* tmp = head_;
    head_ = head_->next;
    delete tmp;
}

void Forward_List::delete_tail() {
    if (!head_) {
        return;
    }
    if (!head_->next) {
        delete head_;
        head_ = nullptr;
        return;
    }
    Node* prev = head_;
    Node* cur = head_->next;
    while (cur->next) {
        prev = cur;
        cur = cur->next;
    }
    prev->next = nullptr;
    delete cur;
}

void Forward_List::delete_before(const std::string& value) {
    if (!head_ || !head_->next) {
        return;
    }
    if (head_->next->value == value) {
        delete_head();
        return;
    }
    Node* prevprev = head_;
    Node* prev = head_->next;
    Node* cur = prev->next;
    while (cur) {
        if (cur->value == value) {
            prevprev->next = cur;
            delete prev;
            return;
        }
        prevprev = prev;
        prev = cur;
        cur = cur->next;
    }
}

void Forward_List::delete_after(const std::string& value) {
    Node* cur = head_;
    while (cur && cur->next) {
        if (cur->value == value) {
            Node* del = cur->next;
            cur->next = del->next;
            delete del;
            return;
        }
        cur = cur->next;
    }
}

void Forward_List::delete_value(const std::string& value) {
    if (!head_) {
        return;
    }
    if (head_->value == value) {
        delete_head();
        return;
    }
    Node* prev = head_;
    Node* cur = head_->next;
    while (cur) {
        if (cur->value == value) {
            prev->next = cur->next;
            delete cur;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

bool Forward_List::find(const std::string& value) const {
    Node* cur = head_;
    while (cur) {
        if (cur->value == value) {
            return true;
        }
        cur = cur->next;
    }
    return false;
}

std::string Forward_List::read() const {
    std::string out;
    Node* cur = head_;
    bool first = true;
    while (cur) {
        if (!first) {
            out += ' ';
        }
        first = false;
        out += cur->value;
        cur = cur->next;
    }
    return out;
}

std::string Forward_List::read_reverse() const {
    std::vector<std::string> values;
    for (Node* cur = head_; cur; cur = cur->next) {
        values.push_back(cur->value);
    }
    std::string out;
    for (std::size_t i = values.size(); i-- > 0;) {
        out += values[i];
        if (i != 0) {
            out += ' ';
        }
    }
    return out;
}

void Forward_List::serialize_binary(std::ostream& os) const {
    std::vector<std::string> values;
    for (Node* cur = head_; cur; cur = cur->next) {
        values.push_back(cur->value);
    }
    const std::uint32_t sz = static_cast<std::uint32_t>(values.size());
    os.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    for (const auto& v : values) {
        write_string_bin(os, v);
    }
}

void Forward_List::deserialize_binary(std::istream& is) {
    clear();
    std::uint32_t sz = 0;
    is.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    for (std::uint32_t i = 0; i < sz; ++i) {
        push_tail(read_string_bin(is));
    }
}

void Forward_List::serialize_text(std::ostream& os) const {
    std::vector<std::string> values;
    for (Node* cur = head_; cur; cur = cur->next) {
        values.push_back(cur->value);
    }
    os << values.size() << "\n";
    for (const auto& v : values) {
        os << v << "\n";
    }
}

void Forward_List::deserialize_text(std::istream& is) {
    clear();
    int sz = 0;
    if (!(is >> sz)) {
        return;
    }
    std::string dummy;
    std::getline(is, dummy);
    for (int i = 0; i < sz; ++i) {
        std::string line;
        std::getline(is, line);
        push_tail(line);
    }
}

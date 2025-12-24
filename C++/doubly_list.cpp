#include "doubly_list.h"

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

DoublyList::DoublyList(std::string name) : name(std::move(name)) {}

DoublyList::~DoublyList() { clear(); }

void DoublyList::clear() {
    Node* cur = head_;
    while (cur) {
        Node* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head_ = nullptr;
    tail_ = nullptr;
}

void DoublyList::push_head(const std::string& value) {
    auto* n = new Node(value);
    n->next = head_;
    if (head_) {
        head_->prev = n;
    } else {
        tail_ = n;
    }
    head_ = n;
}

void DoublyList::push_tail(const std::string& value) {
    auto* n = new Node(value);
    n->prev = tail_;
    if (tail_) {
        tail_->next = n;
    } else {
        head_ = n;
    }
    tail_ = n;
}

void DoublyList::push_before(const std::string& before, const std::string& value) {
    Node* cur = head_;
    while (cur) {
        if (cur->value == before) {
            if (cur == head_) {
                push_head(value);
                return;
            }
            auto* n = new Node(value);
            n->prev = cur->prev;
            n->next = cur;
            cur->prev->next = n;
            cur->prev = n;
            return;
        }
        cur = cur->next;
    }
}

void DoublyList::push_after(const std::string& after, const std::string& value) {
    Node* cur = head_;
    while (cur) {
        if (cur->value == after) {
            if (cur == tail_) {
                push_tail(value);
                return;
            }
            auto* n = new Node(value);
            n->next = cur->next;
            n->prev = cur;
            cur->next->prev = n;
            cur->next = n;
            return;
        }
        cur = cur->next;
    }
}

void DoublyList::delete_head() {
    if (!head_) {
        return;
    }
    Node* del = head_;
    head_ = head_->next;
    if (head_) {
        head_->prev = nullptr;
    } else {
        tail_ = nullptr;
    }
    delete del;
}

void DoublyList::delete_tail() {
    if (!tail_) {
        return;
    }
    Node* del = tail_;
    tail_ = tail_->prev;
    if (tail_) {
        tail_->next = nullptr;
    } else {
        head_ = nullptr;
    }
    delete del;
}

void DoublyList::delete_before(const std::string& value) {
    Node* cur = head_;
    while (cur) {
        if (cur->value == value) {
            Node* target = cur->prev;
            if (!target) {
                return;
            }
            if (target == head_) {
                delete_head();
                return;
            }
            target->prev->next = cur;
            cur->prev = target->prev;
            delete target;
            return;
        }
        cur = cur->next;
    }
}

void DoublyList::delete_after(const std::string& value) {
    Node* cur = head_;
    while (cur) {
        if (cur->value == value) {
            Node* target = cur->next;
            if (!target) {
                return;
            }
            if (target == tail_) {
                delete_tail();
                return;
            }
            cur->next = target->next;
            target->next->prev = cur;
            delete target;
            return;
        }
        cur = cur->next;
    }
}

void DoublyList::delete_value(const std::string& value) {
    Node* cur = head_;
    while (cur) {
        if (cur->value == value) {
            if (cur == head_) {
                delete_head();
                return;
            }
            if (cur == tail_) {
                delete_tail();
                return;
            }
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            delete cur;
            return;
        }
        cur = cur->next;
    }
}

bool DoublyList::find(const std::string& value) const {
    for (Node* cur = head_; cur; cur = cur->next) {
        if (cur->value == value) {
            return true;
        }
    }
    return false;
}

std::string DoublyList::read() const {
    std::string out;
    bool first = true;
    for (Node* cur = head_; cur; cur = cur->next) {
        if (!first) {
            out += ' ';
        }
        first = false;
        out += cur->value;
    }
    return out;
}

std::string DoublyList::read_reverse() const {
    std::string out;
    bool first = true;
    for (Node* cur = tail_; cur; cur = cur->prev) {
        if (!first) {
            out += ' ';
        }
        first = false;
        out += cur->value;
    }
    return out;
}

void DoublyList::serialize_binary(std::ostream& os) const {
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

void DoublyList::deserialize_binary(std::istream& is) {
    clear();
    std::uint32_t sz = 0;
    is.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    for (std::uint32_t i = 0; i < sz; ++i) {
        push_tail(read_string_bin(is));
    }
}

void DoublyList::serialize_text(std::ostream& os) const {
    std::vector<std::string> values;
    for (Node* cur = head_; cur; cur = cur->next) {
        values.push_back(cur->value);
    }
    os << values.size() << "\n";
    for (const auto& v : values) {
        os << v << "\n";
    }
}

void DoublyList::deserialize_text(std::istream& is) {
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

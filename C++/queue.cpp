#include "queue.h"

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

Queue::Queue(std::string name) : name(std::move(name)) {}

Queue::~Queue() { clear(); }

void Queue::clear() {
    Node* cur = head_;
    while (cur) {
        Node* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head_ = nullptr;
    tail_ = nullptr;
}

void Queue::push(const std::string& data) {
    auto* n = new Node(data);
    if (!tail_) {
        head_ = tail_ = n;
        return;
    }
    tail_->next = n;
    tail_ = n;
}

std::string Queue::pop() {
    if (!head_) {
        return "";
    }
    Node* del = head_;
    std::string out = del->data;
    head_ = head_->next;
    if (!head_) {
        tail_ = nullptr;
    }
    delete del;
    return out;
}

std::string Queue::peek() const { return head_ ? head_->data : ""; }

bool Queue::is_empty() const { return head_ == nullptr; }

std::string Queue::read() const {
    std::string out;
    bool first = true;
    for (Node* cur = head_; cur; cur = cur->next) {
        if (!first) {
            out += ' ';
        }
        first = false;
        out += cur->data;
    }
    return out;
}

void Queue::serialize_binary(std::ostream& os) const {
    std::vector<std::string> values;
    for (Node* cur = head_; cur; cur = cur->next) {
        values.push_back(cur->data);
    }
    const std::uint32_t sz = static_cast<std::uint32_t>(values.size());
    os.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    for (const auto& v : values) {
        write_string_bin(os, v);
    }
}

void Queue::deserialize_binary(std::istream& is) {
    clear();
    std::uint32_t sz = 0;
    is.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    for (std::uint32_t i = 0; i < sz; ++i) {
        push(read_string_bin(is));
    }
}

void Queue::serialize_text(std::ostream& os) const {
    std::vector<std::string> values;
    for (Node* cur = head_; cur; cur = cur->next) {
        values.push_back(cur->data);
    }
    os << values.size() << "\n";
    for (const auto& v : values) {
        os << v << "\n";
    }
}

void Queue::deserialize_text(std::istream& is) {
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
        push(line);
    }
}

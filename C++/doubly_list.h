#ifndef DOUBLY_LIST_H
#define DOUBLY_LIST_H

#include <iosfwd>
#include <string>

class DoublyList {
private:
    struct Node {
        std::string value;
        Node* prev{nullptr};
        Node* next{nullptr};
        explicit Node(std::string v) : value(std::move(v)) {}
    };

    Node* head_{nullptr};
    Node* tail_{nullptr};

    void clear();

public:
    std::string name;

    explicit DoublyList(std::string name);
    DoublyList(const DoublyList&) = delete;
    DoublyList& operator=(const DoublyList&) = delete;
    ~DoublyList();

    void push_head(const std::string& value);
    void push_tail(const std::string& value);
    void push_before(const std::string& before, const std::string& value);
    void push_after(const std::string& after, const std::string& value);

    void delete_head();
    void delete_tail();
    void delete_before(const std::string& value);
    void delete_after(const std::string& value);
    void delete_value(const std::string& value);

    bool find(const std::string& value) const;
    std::string read() const;
    std::string read_reverse() const;

    void serialize_binary(std::ostream& os) const;
    void deserialize_binary(std::istream& is);
    void serialize_text(std::ostream& os) const;
    void deserialize_text(std::istream& is);
};

#endif

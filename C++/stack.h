#ifndef STACK_H
#define STACK_H

#include <iosfwd>
#include <string>

class Stack {
private:
    struct Node {
        std::string data;
        Node* next{nullptr};
        explicit Node(std::string d) : data(std::move(d)) {}
    };

    Node* top_{nullptr};

    void clear();

public:
    std::string name;

    explicit Stack(std::string name);
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;
    ~Stack();

    void push(const std::string& data);
    std::string pop();
    std::string peek() const;
    bool is_empty() const;
    std::string read() const;

    void serialize_binary(std::ostream& os) const;
    void deserialize_binary(std::istream& is);
    void serialize_text(std::ostream& os) const;
    void deserialize_text(std::istream& is);
};

#endif

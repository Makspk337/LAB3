#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <iosfwd>
#include <string>

class Forward_List {
private:
    struct Node {
        std::string value;
        Node* next{nullptr};
        explicit Node(std::string v) : value(std::move(v)) {}
    };

    Node* head_{nullptr};

    void clear();

public:
    std::string name;

    explicit Forward_List(std::string name);
    Forward_List(const Forward_List&) = delete;
    Forward_List& operator=(const Forward_List&) = delete;
    Forward_List(Forward_List&&) = delete;
    Forward_List& operator=(Forward_List&&) = delete;
    ~Forward_List();

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

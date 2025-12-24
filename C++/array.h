#ifndef ARRAY_H
#define ARRAY_H

#include <iosfwd>
#include <string>

class Array {
private:
    std::string* data_{nullptr};
    int size_{0};
    int capacity_{0};

    void ensure_capacity();

public:
    std::string name;

    explicit Array(std::string name, int initial_capacity = 8);
    Array(const Array& other);
    Array& operator=(const Array& other);
    Array(Array&& other) noexcept;
    Array& operator=(Array&& other) noexcept;
    ~Array();

    void push_back(const std::string& value);
    void insert(int index, const std::string& value);
    std::string get(int index) const;
    void remove(int index);
    void replace(int index, const std::string& value);
    int length() const;
    std::string read() const;

    void serialize_binary(std::ostream& os) const;
    void deserialize_binary(std::istream& is);

    void serialize_text(std::ostream& os) const;
    void deserialize_text(std::istream& is);
};

#endif

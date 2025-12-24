#include "array.h"

#include <istream>
#include <ostream>
#include <stdexcept>
#include <utility>

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

Array::Array(std::string name, int initial_capacity) : name(std::move(name)) {
    if (initial_capacity < 1) {
        initial_capacity = 1;
    }
    capacity_ = initial_capacity;
    data_ = new std::string[capacity_];
}

Array::Array(const Array& other) : name(other.name), size_(other.size_), capacity_(other.capacity_) {
    data_ = new std::string[capacity_];
    for (int i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

Array& Array::operator=(const Array& other) {
    if (this == &other) {
        return *this;
    }
    Array tmp(other);
    *this = std::move(tmp);
    return *this;
}

Array::Array(Array&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_), name(std::move(other.name)) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

Array& Array::operator=(Array&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    delete[] data_;
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    name = std::move(other.name);

    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    return *this;
}

Array::~Array() { delete[] data_; }

void Array::ensure_capacity() {
    if (size_ < capacity_) {
        return;
    }
    capacity_ = (capacity_ == 0) ? 1 : capacity_ * 2;
    auto* new_data = new std::string[capacity_];
    for (int i = 0; i < size_; ++i) {
        new_data[i] = data_[i];
    }
    delete[] data_;
    data_ = new_data;
}

void Array::push_back(const std::string& value) {
    ensure_capacity();
    data_[size_++] = value;
}

void Array::insert(int index, const std::string& value) {
    if (index < 0 || index > size_) {
        return;
    }
    ensure_capacity();
    for (int i = size_; i > index; --i) {
        data_[i] = data_[i - 1];
    }
    data_[index] = value;
    ++size_;
}

std::string Array::get(int index) const {
    if (index < 0 || index >= size_) {
        return "";
    }
    return data_[index];
}

void Array::remove(int index) {
    if (index < 0 || index >= size_) {
        return;
    }
    for (int i = index; i < size_ - 1; ++i) {
        data_[i] = data_[i + 1];
    }
    --size_;
}

void Array::replace(int index, const std::string& value) {
    if (index < 0 || index >= size_) {
        return;
    }
    data_[index] = value;
}

int Array::length() const { return size_; }

std::string Array::read() const {
    std::string out;
    for (int i = 0; i < size_; ++i) {
        if (i) {
            out += ' ';
        }
        out += data_[i];
    }
    return out;
}

void Array::serialize_binary(std::ostream& os) const {
    const std::uint32_t sz = static_cast<std::uint32_t>(size_);
    os.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    for (int i = 0; i < size_; ++i) {
        write_string_bin(os, data_[i]);
    }
}

void Array::deserialize_binary(std::istream& is) {
    std::uint32_t sz = 0;
    is.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    size_ = 0;
    capacity_ = static_cast<int>(sz) > 0 ? static_cast<int>(sz) : 1;
    delete[] data_;
    data_ = new std::string[capacity_];
    for (std::uint32_t i = 0; i < sz; ++i) {
        push_back(read_string_bin(is));
    }
}

void Array::serialize_text(std::ostream& os) const {
    os << size_ << "\n";
    for (int i = 0; i < size_; ++i) {
        os << data_[i] << "\n";
    }
}

void Array::deserialize_text(std::istream& is) {
    int sz = 0;
    if (!(is >> sz)) {
        return;
    }
    std::string dummy;
    std::getline(is, dummy);
    size_ = 0;
    capacity_ = (sz > 0) ? sz : 1;
    delete[] data_;
    data_ = new std::string[capacity_];
    for (int i = 0; i < sz; ++i) {
        std::string line;
        std::getline(is, line);
        push_back(line);
    }
}

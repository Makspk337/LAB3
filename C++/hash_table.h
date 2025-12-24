#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iosfwd>
#include <utility>
#include <vector>

class HashTable {
private:
    enum class SlotState : unsigned char { Empty = 0, Filled = 1, Deleted = 2 };

    int* keys_{nullptr};
    int* values_{nullptr};
    SlotState* states_{nullptr};
    int capacity_{0};
    int count_{0};

    int hash(int key) const;
    void rehash(int new_capacity);
    int next_pow2(int n) const;

public:
    explicit HashTable(int capacity = 8);
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    ~HashTable();

    void insert(int key, int value);
    int get(int key) const;
    bool contains(int key) const;
    void remove(int key);

    int size() const { return count_; }

    std::vector<std::pair<int, int>> items() const;

    void serialize_binary(std::ostream& os) const;
    void deserialize_binary(std::istream& is);
    void serialize_text(std::ostream& os) const;
    void deserialize_text(std::istream& is);
};

class CuckooHashTable {
private:
    int* keys1_{nullptr};
    int* vals1_{nullptr};
    unsigned char* used1_{nullptr};

    int* keys2_{nullptr};
    int* vals2_{nullptr};
    unsigned char* used2_{nullptr};

    int capacity_{0};
    int count_{0};

    int hash1(int key) const;
    int hash2(int key) const;
    void rehash(int new_capacity);

public:
    explicit CuckooHashTable(int capacity = 8);
    CuckooHashTable(const CuckooHashTable&) = delete;
    CuckooHashTable& operator=(const CuckooHashTable&) = delete;
    ~CuckooHashTable();

    void insert(int key, int value);
    int get(int key) const;
    bool contains(int key) const;
    void remove(int key);

    int size() const { return count_; }

    std::vector<std::pair<int, int>> items() const;

    void serialize_binary(std::ostream& os) const;
    void deserialize_binary(std::istream& is);
    void serialize_text(std::ostream& os) const;
    void deserialize_text(std::istream& is);
};

#endif

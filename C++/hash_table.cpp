#include "hash_table.h"

#include <algorithm>
#include <cstdint>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {
constexpr double kMaxLoad = 0.6;

int clamp_min_pow2(int n) {
    if (n < 2) return 2;
    return n;
}
}


HashTable::HashTable(int capacity) {
    capacity_ = next_pow2(clamp_min_pow2(capacity));
    keys_ = new int[capacity_];
    values_ = new int[capacity_];
    states_ = new SlotState[capacity_];
    std::fill(states_, states_ + capacity_, SlotState::Empty);
}

HashTable::~HashTable() {
    delete[] keys_;
    delete[] values_;
    delete[] states_;
}

int HashTable::next_pow2(int n) const {
    int p = 1;
    while (p < n) p <<= 1;
    return p;
}

int HashTable::hash(int key) const {
    std::uint32_t x = static_cast<std::uint32_t>(key);
    x *= 2654435761u;
    return static_cast<int>(x & static_cast<std::uint32_t>(capacity_ - 1));
}

void HashTable::rehash(int new_capacity) {
    new_capacity = next_pow2(clamp_min_pow2(new_capacity));
    int* old_keys = keys_;
    int* old_vals = values_;
    SlotState* old_states = states_;
    int old_cap = capacity_;

    keys_ = new int[new_capacity];
    values_ = new int[new_capacity];
    states_ = new SlotState[new_capacity];
    capacity_ = new_capacity;
    count_ = 0;
    std::fill(states_, states_ + capacity_, SlotState::Empty);

    for (int i = 0; i < old_cap; ++i) {
        if (old_states[i] == SlotState::Filled) {
            insert(old_keys[i], old_vals[i]);
        }
    }

    delete[] old_keys;
    delete[] old_vals;
    delete[] old_states;
}

void HashTable::insert(int key, int value) {
    if (static_cast<double>(count_ + 1) / static_cast<double>(capacity_) > kMaxLoad) {
        rehash(capacity_ * 2);
    }

    int idx = hash(key);
    int first_deleted = -1;

    for (int probe = 0; probe < capacity_; ++probe) {
        const int pos = (idx + probe) & (capacity_ - 1);

        if (states_[pos] == SlotState::Filled && keys_[pos] == key) {
            values_[pos] = value;
            return;
        }
        if (states_[pos] == SlotState::Deleted && first_deleted == -1) {
            first_deleted = pos;
        }
        if (states_[pos] == SlotState::Empty) {
            const int target = (first_deleted != -1) ? first_deleted : pos;
            keys_[target] = key;
            values_[target] = value;
            states_[target] = SlotState::Filled;
            ++count_;
            return;
        }
    }

    rehash(capacity_ * 2);
    insert(key, value);
}

bool HashTable::contains(int key) const { return get(key) != -1; }

int HashTable::get(int key) const {
    int idx = hash(key);
    for (int probe = 0; probe < capacity_; ++probe) {
        const int pos = (idx + probe) & (capacity_ - 1);
        if (states_[pos] == SlotState::Empty) {
            return -1;
        }
        if (states_[pos] == SlotState::Filled && keys_[pos] == key) {
            return values_[pos];
        }
    }
    return -1;
}

void HashTable::remove(int key) {
    int idx = hash(key);
    for (int probe = 0; probe < capacity_; ++probe) {
        const int pos = (idx + probe) & (capacity_ - 1);
        if (states_[pos] == SlotState::Empty) {
            return;
        }
        if (states_[pos] == SlotState::Filled && keys_[pos] == key) {
            states_[pos] = SlotState::Deleted;
            --count_;
            return;
        }
    }
}

std::vector<std::pair<int, int>> HashTable::items() const {
    std::vector<std::pair<int, int>> out;
    out.reserve(static_cast<std::size_t>(count_));
    for (int i = 0; i < capacity_; ++i) {
        if (states_[i] == SlotState::Filled) {
            out.emplace_back(keys_[i], values_[i]);
        }
    }
    return out;
}

void HashTable::serialize_binary(std::ostream& os) const {
    auto it = items();
    const std::uint32_t sz = static_cast<std::uint32_t>(it.size());
    os.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    for (auto [k, v] : it) {
        os.write(reinterpret_cast<const char*>(&k), sizeof(k));
        os.write(reinterpret_cast<const char*>(&v), sizeof(v));
    }
}

void HashTable::deserialize_binary(std::istream& is) {
    std::uint32_t sz = 0;
    is.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    rehash(next_pow2(static_cast<int>(sz * 2 + 8)));
    for (std::uint32_t i = 0; i < sz; ++i) {
        int k = 0;
        int v = 0;
        is.read(reinterpret_cast<char*>(&k), sizeof(k));
        is.read(reinterpret_cast<char*>(&v), sizeof(v));
        insert(k, v);
    }
}

void HashTable::serialize_text(std::ostream& os) const {
    auto it = items();
    os << it.size() << "\n";
    for (auto [k, v] : it) {
        os << k << " " << v << "\n";
    }
}

void HashTable::deserialize_text(std::istream& is) {
    std::size_t sz = 0;
    if (!(is >> sz)) return;
    rehash(next_pow2(static_cast<int>(sz * 2 + 8)));
    for (std::size_t i = 0; i < sz; ++i) {
        int k = 0;
        int v = 0;
        is >> k >> v;
        insert(k, v);
    }
}


CuckooHashTable::CuckooHashTable(int capacity) {
    capacity_ = 1;
    while (capacity_ < capacity) capacity_ <<= 1;
    if (capacity_ < 2) capacity_ = 2;

    keys1_ = new int[capacity_];
    vals1_ = new int[capacity_];
    used1_ = new unsigned char[capacity_]();

    keys2_ = new int[capacity_];
    vals2_ = new int[capacity_];
    used2_ = new unsigned char[capacity_]();
}

CuckooHashTable::~CuckooHashTable() {
    delete[] keys1_;
    delete[] vals1_;
    delete[] used1_;
    delete[] keys2_;
    delete[] vals2_;
    delete[] used2_;
}

int CuckooHashTable::hash1(int key) const {
    std::uint32_t x = static_cast<std::uint32_t>(key);
    x *= 2654435761u;
    return static_cast<int>(x & static_cast<std::uint32_t>(capacity_ - 1));
}
int CuckooHashTable::hash2(int key) const {
    std::uint32_t x = static_cast<std::uint32_t>(key);
    x ^= x >> 16;
    x *= 2246822519u;
    x ^= x >> 13;
    return static_cast<int>(x & static_cast<std::uint32_t>(capacity_ - 1));
}

void CuckooHashTable::rehash(int new_capacity) {
    auto current = items();
    delete[] keys1_;
    delete[] vals1_;
    delete[] used1_;
    delete[] keys2_;
    delete[] vals2_;
    delete[] used2_;

    capacity_ = 1;
    while (capacity_ < new_capacity) capacity_ <<= 1;
    if (capacity_ < 2) capacity_ = 2;

    keys1_ = new int[capacity_];
    vals1_ = new int[capacity_];
    used1_ = new unsigned char[capacity_]();

    keys2_ = new int[capacity_];
    vals2_ = new int[capacity_];
    used2_ = new unsigned char[capacity_]();

    count_ = 0;
    for (auto [k, v] : current) {
        insert(k, v);
    }
}

bool CuckooHashTable::contains(int key) const { return get(key) != -1; }

int CuckooHashTable::get(int key) const {
    const int i1 = hash1(key);
    if (used1_[i1] && keys1_[i1] == key) return vals1_[i1];

    const int i2 = hash2(key);
    if (used2_[i2] && keys2_[i2] == key) return vals2_[i2];

    return -1;
}

void CuckooHashTable::insert(int key, int value) {
    if (contains(key)) {
        const int i1 = hash1(key);
        if (used1_[i1] && keys1_[i1] == key) {
            vals1_[i1] = value;
            return;
        }
        const int i2 = hash2(key);
        if (used2_[i2] && keys2_[i2] == key) {
            vals2_[i2] = value;
            return;
        }
    }

    if (static_cast<double>(count_ + 1) / static_cast<double>(capacity_) > 0.45) {
        rehash(capacity_ * 2);
    }

    int cur_k = key;
    int cur_v = value;

    constexpr int kMaxKicks = 32;
    for (int kick = 0; kick < kMaxKicks; ++kick) {
        int i1 = hash1(cur_k);
        if (!used1_[i1]) {
            used1_[i1] = 1;
            keys1_[i1] = cur_k;
            vals1_[i1] = cur_v;
            ++count_;
            return;
        }
        std::swap(cur_k, keys1_[i1]);
        std::swap(cur_v, vals1_[i1]);

        int i2 = hash2(cur_k);
        if (!used2_[i2]) {
            used2_[i2] = 1;
            keys2_[i2] = cur_k;
            vals2_[i2] = cur_v;
            ++count_;
            return;
        }
        std::swap(cur_k, keys2_[i2]);
        std::swap(cur_v, vals2_[i2]);
    }

    rehash(capacity_ * 2);
    insert(cur_k, cur_v);
}

void CuckooHashTable::remove(int key) {
    const int i1 = hash1(key);
    if (used1_[i1] && keys1_[i1] == key) {
        used1_[i1] = 0;
        --count_;
        return;
    }
    const int i2 = hash2(key);
    if (used2_[i2] && keys2_[i2] == key) {
        used2_[i2] = 0;
        --count_;
        return;
    }
}

std::vector<std::pair<int, int>> CuckooHashTable::items() const {
    std::vector<std::pair<int, int>> out;
    out.reserve(static_cast<std::size_t>(count_));
    for (int i = 0; i < capacity_; ++i) {
        if (used1_[i]) out.emplace_back(keys1_[i], vals1_[i]);
    }
    for (int i = 0; i < capacity_; ++i) {
        if (used2_[i]) out.emplace_back(keys2_[i], vals2_[i]);
    }
    return out;
}

void CuckooHashTable::serialize_binary(std::ostream& os) const {
    auto it = items();
    const std::uint32_t sz = static_cast<std::uint32_t>(it.size());
    os.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
    for (auto [k, v] : it) {
        os.write(reinterpret_cast<const char*>(&k), sizeof(k));
        os.write(reinterpret_cast<const char*>(&v), sizeof(v));
    }
}

void CuckooHashTable::deserialize_binary(std::istream& is) {
    std::uint32_t sz = 0;
    is.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    rehash(static_cast<int>(sz * 4 + 8));
    for (std::uint32_t i = 0; i < sz; ++i) {
        int k = 0;
        int v = 0;
        is.read(reinterpret_cast<char*>(&k), sizeof(k));
        is.read(reinterpret_cast<char*>(&v), sizeof(v));
        insert(k, v);
    }
}

void CuckooHashTable::serialize_text(std::ostream& os) const {
    auto it = items();
    os << it.size() << "\n";
    for (auto [k, v] : it) {
        os << k << " " << v << "\n";
    }
}

void CuckooHashTable::deserialize_text(std::istream& is) {
    std::size_t sz = 0;
    if (!(is >> sz)) return;
    rehash(static_cast<int>(sz * 4 + 8));
    for (std::size_t i = 0; i < sz; ++i) {
        int k = 0;
        int v = 0;
        is >> k >> v;
        insert(k, v);
    }
}

#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include "hash_table.h"

TEST(HashTableTest, Constructor) {
    HashTable ht;
    EXPECT_EQ(ht.size(), 0);
}

TEST(HashTableTest, InsertAndGet) {
    HashTable ht;
    
    ht.insert(1, 100);
    ht.insert(2, 200);
    ht.insert(3, 300);
    
    EXPECT_EQ(ht.get(1), 100);
    EXPECT_EQ(ht.get(2), 200);
    EXPECT_EQ(ht.get(3), 300);
    EXPECT_EQ(ht.get(4), -1);
}

TEST(HashTableTest, Contains) {
    HashTable ht;
    
    ht.insert(1, 100);
    ht.insert(2, 200);
    
    EXPECT_TRUE(ht.contains(1));
    EXPECT_TRUE(ht.contains(2));
    EXPECT_FALSE(ht.contains(3));
}

TEST(HashTableTest, UpdateValue) {
    HashTable ht;
    
    ht.insert(1, 100);
    EXPECT_EQ(ht.get(1), 100);
    
    ht.insert(1, 200);
    EXPECT_EQ(ht.get(1), 200);
}

TEST(HashTableTest, Remove) {
    HashTable ht;
    
    ht.insert(1, 100);
    ht.insert(2, 200);
    ht.insert(3, 300);
    
    EXPECT_EQ(ht.size(), 3);
    EXPECT_TRUE(ht.contains(2));
    
    ht.remove(2);
    EXPECT_EQ(ht.size(), 2);
    EXPECT_FALSE(ht.contains(2));
    EXPECT_EQ(ht.get(2), -1);
    
    ht.remove(999);
    EXPECT_EQ(ht.size(), 2);
}

TEST(HashTableTest, Rehash) {
    HashTable ht(2);
    
    for (int i = 0; i < 10; ++i) {
        ht.insert(i, i * 100);
    }
    
    EXPECT_EQ(ht.size(), 10);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(ht.get(i), i * 100);
    }
}

TEST(HashTableTest, Items) {
    HashTable ht;
    
    ht.insert(1, 100);
    ht.insert(2, 200);
    ht.insert(3, 300);
    
    auto items = ht.items();
    EXPECT_EQ(items.size(), 3);
}

TEST(HashTableTest, SerializeBinary) {
    HashTable ht;
    
    ht.insert(1, 100);
    ht.insert(2, 200);
    ht.insert(3, 300);
    
    std::stringstream ss;
    ht.serialize_binary(ss);
    
    HashTable ht2;
    ht2.deserialize_binary(ss);
    
    EXPECT_EQ(ht2.size(), 3);
    EXPECT_EQ(ht2.get(1), 100);
    EXPECT_EQ(ht2.get(2), 200);
    EXPECT_EQ(ht2.get(3), 300);
}

TEST(HashTableTest, SerializeText) {
    HashTable ht;
    
    ht.insert(1, 100);
    ht.insert(2, 200);
    ht.insert(3, 300);
    
    std::stringstream ss;
    ht.serialize_text(ss);
    
    HashTable ht2;
    ht2.deserialize_text(ss);
    
    EXPECT_EQ(ht2.size(), 3);
    EXPECT_EQ(ht2.get(1), 100);
    EXPECT_EQ(ht2.get(2), 200);
    EXPECT_EQ(ht2.get(3), 300);
}

TEST(CuckooHashTableTest, Constructor) {
    CuckooHashTable cht;
    EXPECT_EQ(cht.size(), 0);
}

TEST(CuckooHashTableTest, InsertAndGet) {
    CuckooHashTable cht;
    
    cht.insert(1, 100);
    cht.insert(2, 200);
    cht.insert(3, 300);
    
    EXPECT_EQ(cht.get(1), 100);
    EXPECT_EQ(cht.get(2), 200);
    EXPECT_EQ(cht.get(3), 300);
    EXPECT_EQ(cht.get(4), -1);
}

TEST(CuckooHashTableTest, Contains) {
    CuckooHashTable cht;
    
    cht.insert(1, 100);
    cht.insert(2, 200);
    
    EXPECT_TRUE(cht.contains(1));
    EXPECT_TRUE(cht.contains(2));
    EXPECT_FALSE(cht.contains(3));
}

TEST(CuckooHashTableTest, UpdateValue) {
    CuckooHashTable cht;
    
    cht.insert(1, 100);
    EXPECT_EQ(cht.get(1), 100);
    
    cht.insert(1, 200);
    EXPECT_EQ(cht.get(1), 200);
}

TEST(CuckooHashTableTest, Remove) {
    CuckooHashTable cht;
    
    cht.insert(1, 100);
    cht.insert(2, 200);
    cht.insert(3, 300);
    
    EXPECT_EQ(cht.size(), 3);
    EXPECT_TRUE(cht.contains(2));
    
    cht.remove(2);
    EXPECT_EQ(cht.size(), 2);
    EXPECT_FALSE(cht.contains(2));
    EXPECT_EQ(cht.get(2), -1);
}

TEST(CuckooHashTableTest, SerializeBinary) {
    CuckooHashTable cht;
    
    cht.insert(1, 100);
    cht.insert(2, 200);
    cht.insert(3, 300);
    
    std::stringstream ss;
    cht.serialize_binary(ss);
    
    CuckooHashTable cht2;
    cht2.deserialize_binary(ss);
    
    EXPECT_EQ(cht2.size(), 3);
    EXPECT_EQ(cht2.get(1), 100);
    EXPECT_EQ(cht2.get(2), 200);
    EXPECT_EQ(cht2.get(3), 300);
}

TEST(CuckooHashTableTest, SerializeText) {
    CuckooHashTable cht;
    
    cht.insert(1, 100);
    cht.insert(2, 200);
    cht.insert(3, 300);
    
    std::stringstream ss;
    cht.serialize_text(ss);
    
    CuckooHashTable cht2;
    cht2.deserialize_text(ss);
    
    EXPECT_EQ(cht2.size(), 3);
    EXPECT_EQ(cht2.get(1), 100);
    EXPECT_EQ(cht2.get(2), 200);
    EXPECT_EQ(cht2.get(3), 300);
}

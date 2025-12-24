#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include "array.h"

TEST(ArrayTest, ConstructorAndBasic) {
    Array arr("test_array");
    EXPECT_EQ(arr.length(), 0);
    EXPECT_EQ(arr.read(), "");
}

TEST(ArrayTest, PushBackAndGet) {
    Array arr("test_array");
    
    arr.push_back("first");
    arr.push_back("second");
    arr.push_back("third");
    
    EXPECT_EQ(arr.length(), 3);
    EXPECT_EQ(arr.get(0), "first");
    EXPECT_EQ(arr.get(1), "second");
    EXPECT_EQ(arr.get(2), "third");
    EXPECT_EQ(arr.get(5), "");
}

TEST(ArrayTest, Read) {
    Array arr("test_array");
    
    arr.push_back("one");
    arr.push_back("two");
    arr.push_back("three");
    
    EXPECT_EQ(arr.read(), "one two three");
}

TEST(ArrayTest, Insert) {
    Array arr("test_array");
    
    arr.push_back("one");
    arr.push_back("three");
    arr.insert(1, "two");
    
    EXPECT_EQ(arr.length(), 3);
    EXPECT_EQ(arr.read(), "one two three");
    
    arr.insert(0, "zero");
    EXPECT_EQ(arr.read(), "zero one two three");
    
    arr.insert(4, "four");
    EXPECT_EQ(arr.read(), "zero one two three four");
}

TEST(ArrayTest, Remove) {
    Array arr("test_array");
    
    arr.push_back("one");
    arr.push_back("two");
    arr.push_back("three");
    
    arr.remove(1);
    EXPECT_EQ(arr.length(), 2);
    EXPECT_EQ(arr.read(), "one three");
    
    arr.remove(0);
    EXPECT_EQ(arr.length(), 1);
    EXPECT_EQ(arr.read(), "three");
    
    arr.remove(0);
    EXPECT_EQ(arr.length(), 0);
    EXPECT_EQ(arr.read(), "");
}

TEST(ArrayTest, Replace) {
    Array arr("test_array");
    
    arr.push_back("old1");
    arr.push_back("old2");
    arr.replace(0, "new1");
    arr.replace(1, "new2");
    
    EXPECT_EQ(arr.get(0), "new1");
    EXPECT_EQ(arr.get(1), "new2");
    EXPECT_EQ(arr.read(), "new1 new2");
}

TEST(ArrayTest, CopyConstructor) {
    Array arr1("arr1");
    arr1.push_back("a");
    arr1.push_back("b");
    
    Array arr2 = arr1;
    EXPECT_EQ(arr2.length(), 2);
    EXPECT_EQ(arr2.read(), "a b");
    
    arr1.push_back("c");
    EXPECT_EQ(arr1.length(), 3);
    EXPECT_EQ(arr2.length(), 2);
}

TEST(ArrayTest, MoveConstructor) {
    Array arr1("arr1");
    arr1.push_back("a");
    arr1.push_back("b");
    
    Array arr2 = std::move(arr1);
    EXPECT_EQ(arr2.length(), 2);
    EXPECT_EQ(arr2.read(), "a b");
    EXPECT_EQ(arr1.length(), 0);
}

TEST(ArrayTest, CopyAssignment) {
    Array arr1("arr1");
    arr1.push_back("a");
    arr1.push_back("b");
    
    Array arr2("arr2");
    arr2 = arr1;
    
    EXPECT_EQ(arr2.length(), 2);
    EXPECT_EQ(arr2.read(), "a b");
}

TEST(ArrayTest, MoveAssignment) {
    Array arr1("arr1");
    arr1.push_back("a");
    arr1.push_back("b");
    
    Array arr2("arr2");
    arr2 = std::move(arr1);
    
    EXPECT_EQ(arr2.length(), 2);
    EXPECT_EQ(arr2.read(), "a b");
    EXPECT_EQ(arr1.length(), 0);
}

TEST(ArrayTest, SerializeBinary) {
    Array arr("test_array");
    arr.push_back("hello");
    arr.push_back("world");
    arr.push_back("test");
    
    std::stringstream ss;
    arr.serialize_binary(ss);
    
    Array arr2("test_array2");
    arr2.deserialize_binary(ss);
    
    EXPECT_EQ(arr2.length(), 3);
    EXPECT_EQ(arr2.read(), "hello world test");
}

TEST(ArrayTest, SerializeText) {
    Array arr("test_array");
    arr.push_back("hello");
    arr.push_back("world");
    arr.push_back("test");
    
    std::stringstream ss;
    arr.serialize_text(ss);
    
    Array arr2("test_array2");
    arr2.deserialize_text(ss);
    
    EXPECT_EQ(arr2.length(), 3);
    EXPECT_EQ(arr2.read(), "hello world test");
}

TEST(ArrayTest, EmptySerialize) {
    Array arr("test_array");
    
    std::stringstream ss;
    arr.serialize_binary(ss);
    
    Array arr2("test_array2");
    arr2.deserialize_binary(ss);
    
    EXPECT_EQ(arr2.length(), 0);
    EXPECT_EQ(arr2.read(), "");
}

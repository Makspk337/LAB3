#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include "stack.h"

TEST(StackTest, Constructor) {
    Stack st("test_stack");
    EXPECT_TRUE(st.is_empty());
    EXPECT_EQ(st.peek(), "");
    EXPECT_EQ(st.read(), "");
}

TEST(StackTest, PushAndPeek) {
    Stack st("test_stack");
    
    st.push("first");
    EXPECT_FALSE(st.is_empty());
    EXPECT_EQ(st.peek(), "first");
    
    st.push("second");
    EXPECT_EQ(st.peek(), "second");
    
    st.push("third");
    EXPECT_EQ(st.peek(), "third");
}

TEST(StackTest, Pop) {
    Stack st("test_stack");
    
    st.push("first");
    st.push("second");
    st.push("third");
    
    EXPECT_EQ(st.pop(), "third");
    EXPECT_EQ(st.pop(), "second");
    EXPECT_EQ(st.pop(), "first");
    EXPECT_EQ(st.pop(), "");
    EXPECT_TRUE(st.is_empty());
}

TEST(StackTest, Read) {
    Stack st("test_stack");
    
    st.push("one");
    st.push("two");
    st.push("three");
    
    EXPECT_EQ(st.read(), "three two one");
    
    st.pop();
    EXPECT_EQ(st.read(), "two one");
}

TEST(StackTest, SerializeBinary) {
    Stack st("test_stack");
    
    st.push("hello");
    st.push("world");
    st.push("test");
    
    std::stringstream ss;
    st.serialize_binary(ss);
    
    Stack st2("test_stack2");
    st2.deserialize_binary(ss);
    
    EXPECT_EQ(st2.read(), "test world hello");
    EXPECT_EQ(st2.pop(), "test");
    EXPECT_EQ(st2.pop(), "world");
    EXPECT_EQ(st2.pop(), "hello");
    EXPECT_TRUE(st2.is_empty());
}

TEST(StackTest, SerializeText) {
    Stack st("test_stack");
    
    st.push("hello");
    st.push("world");
    st.push("test");
    
    std::stringstream ss;
    st.serialize_text(ss);
    
    Stack st2("test_stack2");
    st2.deserialize_text(ss);
    
    EXPECT_EQ(st2.read(), "test world hello");
    EXPECT_EQ(st2.pop(), "test");
    EXPECT_EQ(st2.pop(), "world");
    EXPECT_EQ(st2.pop(), "hello");
    EXPECT_TRUE(st2.is_empty());
}

TEST(StackTest, EmptySerialize) {
    Stack st("test_stack");
    
    std::stringstream ss;
    st.serialize_binary(ss);
    
    Stack st2("test_stack2");
    st2.deserialize_binary(ss);
    
    EXPECT_TRUE(st2.is_empty());
    EXPECT_EQ(st2.read(), "");
}

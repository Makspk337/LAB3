#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include "queue.h"

TEST(QueueTest, Constructor) {
    Queue q("test_queue");
    EXPECT_TRUE(q.is_empty());
    EXPECT_EQ(q.peek(), "");
    EXPECT_EQ(q.read(), "");
}

TEST(QueueTest, PushAndPeek) {
    Queue q("test_queue");
    
    q.push("first");
    EXPECT_FALSE(q.is_empty());
    EXPECT_EQ(q.peek(), "first");
    
    q.push("second");
    EXPECT_EQ(q.peek(), "first");
    
    q.push("third");
    EXPECT_EQ(q.peek(), "first");
}

TEST(QueueTest, Pop) {
    Queue q("test_queue");
    
    q.push("first");
    q.push("second");
    q.push("third");
    
    EXPECT_EQ(q.pop(), "first");
    EXPECT_EQ(q.pop(), "second");
    EXPECT_EQ(q.pop(), "third");
    EXPECT_EQ(q.pop(), "");
    EXPECT_TRUE(q.is_empty());
}

TEST(QueueTest, Read) {
    Queue q("test_queue");
    
    q.push("one");
    q.push("two");
    q.push("three");
    
    EXPECT_EQ(q.read(), "one two three");
    
    q.pop();
    EXPECT_EQ(q.read(), "two three");
}

TEST(QueueTest, SerializeBinary) {
    Queue q("test_queue");
    
    q.push("hello");
    q.push("world");
    q.push("test");
    
    std::stringstream ss;
    q.serialize_binary(ss);
    
    Queue q2("test_queue2");
    q2.deserialize_binary(ss);
    
    EXPECT_EQ(q2.read(), "hello world test");
    EXPECT_EQ(q2.pop(), "hello");
    EXPECT_EQ(q2.pop(), "world");
    EXPECT_EQ(q2.pop(), "test");
    EXPECT_TRUE(q2.is_empty());
}

TEST(QueueTest, SerializeText) {
    Queue q("test_queue");
    
    q.push("hello");
    q.push("world");
    q.push("test");
    
    std::stringstream ss;
    q.serialize_text(ss);
    
    Queue q2("test_queue2");
    q2.deserialize_text(ss);
    
    EXPECT_EQ(q2.read(), "hello world test");
    EXPECT_EQ(q2.pop(), "hello");
    EXPECT_EQ(q2.pop(), "world");
    EXPECT_EQ(q2.pop(), "test");
    EXPECT_TRUE(q2.is_empty());
}

TEST(QueueTest, EmptySerialize) {
    Queue q("test_queue");
    
    std::stringstream ss;
    q.serialize_binary(ss);
    
    Queue q2("test_queue2");
    q2.deserialize_binary(ss);
    
    EXPECT_TRUE(q2.is_empty());
    EXPECT_EQ(q2.read(), "");
}

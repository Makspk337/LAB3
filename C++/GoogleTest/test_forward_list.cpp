#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include "forward_list.h"

TEST(ForwardListTest, Constructor) {
    Forward_List list("test_list");
    EXPECT_EQ(list.read(), "");
    EXPECT_EQ(list.read_reverse(), "");
}

TEST(ForwardListTest, PushHead) {
    Forward_List list("test_list");
    
    list.push_head("third");
    list.push_head("second");
    list.push_head("first");
    
    EXPECT_EQ(list.read(), "first second third");
    EXPECT_EQ(list.read_reverse(), "third second first");
}

TEST(ForwardListTest, PushTail) {
    Forward_List list("test_list");
    
    list.push_tail("first");
    list.push_tail("second");
    list.push_tail("third");
    
    EXPECT_EQ(list.read(), "first second third");
    EXPECT_EQ(list.read_reverse(), "third second first");
}

TEST(ForwardListTest, PushBefore) {
    Forward_List list("test_list");
    
    list.push_tail("one");
    list.push_tail("three");
    list.push_before("three", "two");
    
    EXPECT_EQ(list.read(), "one two three");
    
    list.push_before("one", "zero");
    EXPECT_EQ(list.read(), "zero one two three");
}

TEST(ForwardListTest, PushAfter) {
    Forward_List list("test_list");
    
    list.push_tail("one");
    list.push_tail("three");
    list.push_after("one", "two");
    
    EXPECT_EQ(list.read(), "one two three");
    
    list.push_after("three", "four");
    EXPECT_EQ(list.read(), "one two three four");
}

TEST(ForwardListTest, DeleteHead) {
    Forward_List list("test_list");
    
    list.push_tail("one");
    list.push_tail("two");
    list.push_tail("three");
    
    list.delete_head();
    EXPECT_EQ(list.read(), "two three");
    
    list.delete_head();
    EXPECT_EQ(list.read(), "three");
    
    list.delete_head();
    EXPECT_EQ(list.read(), "");
    
    list.delete_head();
    EXPECT_EQ(list.read(), "");
}

TEST(ForwardListTest, DeleteTail) {
    Forward_List list("test_list");
    
    list.push_tail("one");
    list.push_tail("two");
    list.push_tail("three");
    
    list.delete_tail();
    EXPECT_EQ(list.read(), "one two");
    
    list.delete_tail();
    EXPECT_EQ(list.read(), "one");
    
    list.delete_tail();
    EXPECT_EQ(list.read(), "");
    
    list.delete_tail();
    EXPECT_EQ(list.read(), "");
}

TEST(ForwardListTest, DeleteBefore) {
    Forward_List list("test_list");
    
    list.push_tail("one");
    list.push_tail("two");
    list.push_tail("three");
    
    list.delete_before("three");
    EXPECT_EQ(list.read(), "one three");
    
    list.delete_before("three");
    EXPECT_EQ(list.read(), "three");
}

TEST(ForwardListTest, DeleteAfter) {
    Forward_List list("test_list");
    
    list.push_tail("one");
    list.push_tail("two");
    list.push_tail("three");
    
    list.delete_after("one");
    EXPECT_EQ(list.read(), "one three");
    
    list.delete_after("one");
    EXPECT_EQ(list.read(), "one");
}

TEST(ForwardListTest, DeleteValue) {
    Forward_List list("test_list");
    
    list.push_tail("one");
    list.push_tail("two");
    list.push_tail("three");
    
    list.delete_value("two");
    EXPECT_EQ(list.read(), "one three");
    
    list.delete_value("one");
    EXPECT_EQ(list.read(), "three");
    
    list.delete_value("three");
    EXPECT_EQ(list.read(), "");
}

TEST(ForwardListTest, Find) {
    Forward_List list("test_list");
    
    list.push_tail("one");
    list.push_tail("two");
    list.push_tail("three");
    
    EXPECT_TRUE(list.find("one"));
    EXPECT_TRUE(list.find("two"));
    EXPECT_TRUE(list.find("three"));
    EXPECT_FALSE(list.find("four"));
    EXPECT_FALSE(list.find(""));
}

TEST(ForwardListTest, SerializeBinary) {
    Forward_List list("test_list");
    
    list.push_tail("hello");
    list.push_tail("world");
    list.push_tail("test");
    
    std::stringstream ss;
    list.serialize_binary(ss);
    
    Forward_List list2("test_list2");
    list2.deserialize_binary(ss);
    
    EXPECT_EQ(list2.read(), "hello world test");
}

TEST(ForwardListTest, SerializeText) {
    Forward_List list("test_list");
    
    list.push_tail("hello");
    list.push_tail("world");
    list.push_tail("test");
    
    std::stringstream ss;
    list.serialize_text(ss);
    
    Forward_List list2("test_list2");
    list2.deserialize_text(ss);
    
    EXPECT_EQ(list2.read(), "hello world test");
}

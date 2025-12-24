#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include "tree.h"

TEST(TreeTest, Constructor) {
    Tree t("test_tree");
    EXPECT_EQ(t.read(), "");
    EXPECT_FALSE(t.find(1));
}

TEST(TreeTest, InsertAndFind) {
    Tree t("test_tree");
    
    t.insert(50);
    t.insert(30);
    t.insert(70);
    t.insert(20);
    t.insert(40);
    t.insert(60);
    t.insert(80);
    
    EXPECT_TRUE(t.find(50));
    EXPECT_TRUE(t.find(30));
    EXPECT_TRUE(t.find(70));
    EXPECT_TRUE(t.find(20));
    EXPECT_TRUE(t.find(40));
    EXPECT_TRUE(t.find(60));
    EXPECT_TRUE(t.find(80));
    EXPECT_FALSE(t.find(10));
    EXPECT_FALSE(t.find(90));
}

TEST(TreeTest, ReadPreorder) {
    Tree t("test_tree");
    
    t.insert(50);
    t.insert(30);
    t.insert(70);
    t.insert(20);
    t.insert(40);
    t.insert(60);
    t.insert(80);
    
    std::string result = t.read();
    EXPECT_EQ(result, "50 30 20 40 70 60 80");
}

TEST(TreeTest, IsFull) {
    Tree t1("test_tree1");
    
    t1.insert(50);
    t1.insert(30);
    t1.insert(70);
    EXPECT_TRUE(t1.is_full());
    
    Tree t2("test_tree2");
    t2.insert(50);
    t2.insert(30);
    t2.insert(70);
    t2.insert(20);
    EXPECT_FALSE(t2.is_full());
    
    Tree t3("test_tree3");
    t3.insert(50);
    t3.insert(30);
    t3.insert(70);
    t3.insert(20);
    t3.insert(40);
    t3.insert(60);
    t3.insert(80);
    EXPECT_TRUE(t3.is_full());
}

TEST(TreeTest, SerializeBinary) {
    Tree t("test_tree");
    
    t.insert(50);
    t.insert(30);
    t.insert(70);
    t.insert(20);
    t.insert(40);
    t.insert(60);
    t.insert(80);
    
    std::stringstream ss;
    t.serialize_binary(ss);
    
    Tree t2("test_tree2");
    t2.deserialize_binary(ss);
    
    EXPECT_EQ(t2.read(), "50 30 20 40 70 60 80");
    EXPECT_TRUE(t2.find(50));
    EXPECT_TRUE(t2.find(30));
    EXPECT_TRUE(t2.find(70));
    EXPECT_TRUE(t2.find(20));
    EXPECT_TRUE(t2.find(40));
    EXPECT_TRUE(t2.find(60));
    EXPECT_TRUE(t2.find(80));
}

TEST(TreeTest, SerializeText) {
    Tree t("test_tree");
    
    t.insert(50);
    t.insert(30);
    t.insert(70);
    t.insert(20);
    t.insert(40);
    
    std::stringstream ss;
    t.serialize_text(ss);
    
    Tree t2("test_tree2");
    t2.deserialize_text(ss);
    
    EXPECT_EQ(t2.read(), "50 30 20 40 70");
    EXPECT_TRUE(t2.find(50));
    EXPECT_TRUE(t2.find(30));
    EXPECT_TRUE(t2.find(70));
    EXPECT_TRUE(t2.find(20));
    EXPECT_TRUE(t2.find(40));
}

TEST(TreeTest, EmptyTree) {
    Tree t("test_tree");
    
    EXPECT_EQ(t.read(), "");
    EXPECT_FALSE(t.find(1));
    EXPECT_TRUE(t.is_full());
    
    std::stringstream ss_bin;
    t.serialize_binary(ss_bin);
    
    Tree t2("test_tree2");
    t2.deserialize_binary(ss_bin);
    EXPECT_EQ(t2.read(), "");
    
    std::stringstream ss_txt;
    t.serialize_text(ss_txt);
    
    Tree t3("test_tree3");
    t3.deserialize_text(ss_txt);
    EXPECT_EQ(t3.read(), "");
}

TEST(TreeTest, SingleNode) {
    Tree t("test_tree");
    t.insert(42);
    
    EXPECT_EQ(t.read(), "42");
    EXPECT_TRUE(t.find(42));
    EXPECT_FALSE(t.find(43));
    EXPECT_TRUE(t.is_full());
    
    std::stringstream ss;
    t.serialize_text(ss);
    
    Tree t2("test_tree2");
    t2.deserialize_text(ss);
    
    EXPECT_EQ(t2.read(), "42");
    EXPECT_TRUE(t2.find(42));
}

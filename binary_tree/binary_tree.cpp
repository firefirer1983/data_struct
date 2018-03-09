#include <assert.h>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

using std::cout;
using std::endl;

class BTNode
{
public:
  BTNode(int key)  : key_(key), left_(nullptr), right_(nullptr) {};
  
  ~BTNode() {
    left_ = nullptr;
    right_ = nullptr;
  };
  int key_;
  BTNode *left_;
  BTNode *right_;
};

class BTree
{
public:
  BTree(BTNode *node)  : root_(node){};
  
  ~BTree() {
    DeleteTree(root_);
  };
  
  void DeleteTree(BTNode *root) {
    BTNode *node = root;
    if(node) {
      DeleteTree(node->left_);
      DeleteTree(node->right_);
      delete node;
    }
  }

  void InsertNode(int key, BTNode *node) {
    if(key < node->key_) {
      if(node->left_) {
        InsertNode(key, node->left_);
        return ;
      }
	  node->left_ = new BTNode(key);
	  cout << key << " < " << node->key_ << endl; 
    } else {
      if(node->right_) {
	    InsertNode(key, node->right_);
        return ;
      }
      node->right_ = new BTNode(key);
	  cout << key << " > " << node->key_ << endl; 
    }
  }

  void Traverse(BTNode *node) {
    if(node) {
      cout << node->key_ << endl;
      Traverse(node->left_);
      Traverse(node->right_);
    }
  }

  BTNode *Search(int key, BTNode *node) {
    if(node){
      if(node->key_ == key) {
        cout << "Hit key: " << key << endl;
        return node;
      } else {
        BTNode *res = Search(key, node->left_); 
        if(res)
          return res;
        res = Search(key, node->right_); 
        if(res)
          return res;
      }
    } 
    return nullptr;
  }
  
  BTNode *root_;
};

class BTree_GTest : public ::testing::Test {

protected:
  BTree *tree_;
  int leafs_key_[5] = {10, 6, 11, 5, 13};
  virtual void SetUp(){
	  tree_ = new BTree(new BTNode(leafs_key_[0]));
	  for(unsigned int i = 1; i < (sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
		tree_->InsertNode(leafs_key_[i], tree_->root_);
	  }
  }
  
  virtual void TearDown(){
    delete tree_;
  }
};

TEST_F(BTree_GTest, BTreeInsert_GTest){
  tree_->InsertNode(8, tree_->root_);
  tree_->Traverse(tree_->root_);
}

TEST_F(BTree_GTest, BTreeSearch_GTest){

  for(unsigned i = 0; i < (sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
    EXPECT_TRUE(tree_->Search(leafs_key_[i], tree_->root_));
  }

  EXPECT_FALSE(tree_->Search(100, tree_->root_));
  EXPECT_FALSE(tree_->Search(3, tree_->root_));
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


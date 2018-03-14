#include <assert.h>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

using std::cout;
using std::endl;

class BSTNode
{
public:
  BSTNode(int key)  : key_(key), left_(nullptr), right_(nullptr) {};
  
  ~BSTNode() {
    left_ = nullptr;
    right_ = nullptr;
  };
  int key_;
  BSTNode *left_;
  BSTNode *right_;
};

class BST
{
public:
  BST(BSTNode *node)  : root_(node){};
  
  ~BST() {
    DeleteTree(root_);
  };
  
  void DeleteTree(BSTNode *root) {
    BSTNode *node = root;
    if(node) {
      DeleteTree(node->left_);
      DeleteTree(node->right_);
      delete node;
    }
  }

  bool Delete(int key) {
    _Delete(key, root_);
  }

  bool _Delete(int key, BSTNode *parent) {
    if(!parent) {
	  return false;
	}
	if(key == parent->key_) {
	  if(!parent->left_ && !parent->right_) { // no left, no right child
	  	delete parent;
	  } else if(parent->left_ && !parent->right_) { // only left child
	  } else if(!parent->left_) && parent->right_) { // only right child
	  } else { // both left and right child
	  }
	  return true;
	} else if(key < parent->key_) {
	  return _Delete(key, parent->left_);
	} else {
	  return _Delete(key, parent->right_);
	}
  }
  
  void Insert(int key) {
    _Insert(key, root_);
  }

  void _Insert(int key, BSTNode *parent) {
	if(!parent) {
	  parent = new BSTNode(key);
	} else {
	  if(key < parent->key_) {
	  	_Insert(key, parent->left_);
	  } else {
        _Insert(key, parent->right_);
	  }
	}
  }

  BSTNode *Search(int key) {
    return _Search(key, root_);
  }
  
  BSTNode *_Search(int key, BSTNode *parent) {
    BSTNode *target = nullptr;
    if(parent) {
      if(parent->key_ == key) {
        target = parent;
      } else if(key < parent->key_) {
        target = _Search(key, parent->left_);
      } else {
        target = _Search(key, parent->right_);
      }
    }
	return target;
  }
  
  void Insert(int key, BSTNode *node) {
  }
  
  void Traverse() {
  }

  
  BSTNode *root_;
};

class BST_GTest : public ::testing::Test {

protected:
  BST *tree_;
  int leafs_key_[15] = {55, 23, 91, 20, 25, 80, 120, 15, 21, 24, 28, 70, 85, 110, 150};
  virtual void SetUp(){
	  tree_ = new BST(new BSTNode(leafs_key_[0]));
	  for(unsigned int i = 1; i < (sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
		tree_->Insert(leafs_key_[i], tree_->root_);
	  }
	  cout << endl;
	  tree_->Traverse();
  }
  
  virtual void TearDown(){
    delete tree_;
  }
};

TEST_F(BST_GTest, BSTInsert_GTest){
  tree_->Insert(8);
  cout << endl;
  tree_->Insert(89);
  cout << endl;
  tree_->Traverse();
}

TEST_F(BST_GTest, BSTSearch_GTest){

  for(unsigned i = 0; i < (sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
    EXPECT_TRUE(tree_->Search(leafs_key_[i]));
  }

  EXPECT_EQ(nullptr, tree_->Search(100));
  tree_->Insert(100);
  cout << endl;
  EXPECT_NE(nullptr, tree_->Search(100));
  EXPECT_EQ(true, tree_->Delete(100));
  EXPECT_EQ(nullptr, tree_->Search(100));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(110));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(91));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(20));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(55));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(21));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(24));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(28));
  tree_->Traverse();
  
  EXPECT_EQ(true, tree_->Delete(15));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(120));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(70));
  tree_->Traverse();
  
  EXPECT_EQ(true, tree_->Delete(150));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(23));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(25));
  tree_->Traverse();
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


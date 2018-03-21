#include <assert.h>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

using std::cout;
using std::endl;

class AVLNode
{
public:
  AVLNode(int key)  : key_(key), left_(nullptr), right_(nullptr) {};
  
  ~AVLNode() {
    left_ = nullptr;
    right_ = nullptr;
  };
  int key_;
  int height_;
  AVLNode *left_;
  AVLNode *right_;
};

class AVL
{
public:
  AVL() :root_(nullptr){};
  
  ~AVL() {
    Delete(root_);
  };

  void Delete(AVLNode *node) {
    if(node){
      Delete(node->left_);
      Delete(node->right_);
      delete node;
    }
  }

  AVLNode *insert(int key, AVLNode *parent) {
    AVLNode *root = nullptr;
    if(!parent) {
	  root = new AVLNode(key);
    } else if(key < parent->key_) {
      root = insert(key, parent->left_);
	  if(root)
        parent->left_ = root;
    } else {
      root = insert(key, parent->right_);
      if(root)
	    parent->right_ = root;
    }
	return root;
  }
  void Insert(int key) {
    cout << "==> " << key << endl;
    AVLNode *ret = nullptr;
    if(root_ == nullptr) {
      root_ = new AVLNode(key);
    } else {
      ret = _Insert(key, root_);
      if(ret) {
        if(ret && key < root_->key_) {
          root_->left_ = ret;
        } else if(ret) {
		  cout << "root_ right: " << key << endl;
		  root_->right_ = ret;
        }
      }
    }
  }
  AVLNode *_Insert(int key, AVLNode *parent) {
    AVLNode *ret = nullptr;
    if(!parent) {
      ret = new AVLNode(key);
    } else if(key < parent->key_) {
      ret = _Insert(key, parent->left_);
      if(ret){
        parent->left_ = ret;
      	cout << "(" << parent->left_->key_ << ")" << "<-" << parent->key_ << endl;
      } else {
		  parent = _Balance(parent);
      }
      
	  ret =nullptr;
	  
    } else if(key >= parent->key_) {
      ret = _Insert(key, parent->right_);
      if(ret) {
	  	parent->right_ = ret;
	  	cout << parent->key_ << "->" << "(" << parent->right_->key_ << ")" <<  endl;
	  } else {
		  parent = _Balance(parent);
      }
	  ret = nullptr;
    }
    return ret;
  }

  int _Diff(AVLNode *parent) {
    int right = _Height(parent->right_);
    int left = _Height(parent->left_);
    int diff = left - right;
    cout << parent->key_ << ": " << left << " vs " << right << " diff: " << diff << endl;
    return diff;
  }
  
  AVLNode *_LeftRotate(AVLNode *node) { // right rotation once!
    cout << "leftRotate" << endl;
    AVLNode *root = node->right_;
    node->right_ = root->left_;
    root->left_ = node;
    return root;
  }
  
  AVLNode *_RightRotate(AVLNode *node) { // right rotation once!
    cout << "_RightRotate" << endl;
    AVLNode *root = node->left_;
    node->left_ = root->right_;
    root->right_ = node;
    return root;
  }

  
  AVLNode *_LRRotate(AVLNode *node) { // right rotation once!
    cout << "_LRRotate" << endl;
	node->left_ = _LeftRotate(node->left_);
	_RightRotate(node);
    return node;
	
  }
  
  AVLNode *_RLRotate(AVLNode *node) { // right rotation once!
    cout << "_RLRotate" << endl;
	node->right_ = _RightRotate(node->right_);
	_LeftRotate(node);
    return node;
  }

  AVLNode *_Balance(AVLNode *parent) {
  	cout << "balance: " << parent->key_ << endl;
    int diff = _Diff(parent);
    cout << "diff: " << diff << endl;
    if(diff > 1) {
      if(_Diff(parent->left_) > 0) {
        parent = _RightRotate(parent);
      } else {
		parent = _RLRotate(parent);
      }
    } else if(diff < -1) {
      if(_Diff(parent->right_) < 0) {
        parent = _LeftRotate(parent);
      } else {
		parent = _LRRotate(parent);
      }
    }
    return parent;
  }

  void Traverse() {
    _Traverse(root_);
    cout << endl;
  }
  
  void _Traverse(AVLNode *node) {
    if(node) {
      cout << node->key_ << " ";
      _Traverse(node->left_);
      _Traverse(node->right_);
    }
  }
  
  int _MAX(int x, int y) {
    return (x>y?x:y);
  }
  
  int _MIN(int x, int y) {
    return (x>y?y:x);
  }

  int _Height(AVLNode *node) {
    int left_height = 0;
    int right_height = 0;
    int height = 0;
    if(node) {
      if(node->left_) {
        left_height = _Height(node->left_);
      }
      if(node->right_) {
        right_height = _Height(node->right_);
      }
      height = _MAX(left_height, right_height) + 1;
	  cout << left_height << " <-- [" << node->key_ << "] --> "  << right_height << endl;
    }
    return  height;
  }
  AVLNode *root_;
};

class AVL_GTest : public ::testing::Test {

protected:
  AVL *tree_;
//  int leafs_key_[15] = {55, 23, 91, 20, 25, 80, 120, 15, 21, 24, 28, 70, 85, 110, 150};
  int leafs_key_[3] = {50, 30, 10};
  virtual void SetUp(){
  tree_ = new AVL();
#if 0
    tree_ = new AVL(new AVLNode(leafs_key_[0]));
    for(unsigned int i = 1; i < (sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
      tree_->Insert(leafs_key_[i]);
      tree->Height();
    }
    cout << endl;
    tree_->Traverse();
#endif	  
  }
  
  virtual void TearDown(){
    delete tree_;
  }
};

#if 0
TEST_F(AVL_GTest, AVLInsert_GTest){
  tree_->Insert(8);
  cout << endl;
  tree_->Insert(89);
  cout << endl;
  tree_->Traverse();
}
#endif
TEST_F(AVL_GTest, AVLInsertBalance_GTest){
#if 0
  for(unsigned int i = 0; i < (sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
    tree_->Insert(leafs_key_[i]);
  }
#endif
  tree_->root_ = tree_->insert(50, tree_->root_);
  tree_->root_ = tree_->insert(30, tree_->root_);
  tree_->root_ = tree_->insert(10, tree_->root_);
  tree_->Traverse();
  
  cout << endl;
}

#if 0
TEST_F(AVL_GTest, AVLSearch_GTest){

  for(unsigned i = 0; i < (sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
    EXPECT_TRUE(tree_->Search(leafs_key_[i]));
  }

  EXPECT_EQ(nullptr, tree_->Search(100));
  tree_->Insert(100, tree_->root_);
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
#endif
int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


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

  AVLNode *Insert(int key, AVLNode *parent) {
    AVLNode *root = nullptr;
    if(!parent) {
      root = new AVLNode(key);
    } else if(key < parent->key_) {
      if(parent->left_) {
        parent->left_ = Insert(key, parent->left_);
      } else {
        parent->left_ = new AVLNode(key);
        cout<< "[" << key << "]" << "->"<< "[" << parent->key_ << "]" <<  endl;
      }
    } else {
      if(parent->right_) {
        parent->right_ = Insert(key, parent->right_);
      } else {
        parent->right_ = new AVLNode(key);
        cout<<"[" << parent->key_ <<"]" << "<-" << "[" << key << "]" << endl;
      }
    }
    
    if(parent) {
	  if(_Diff(parent) > 1) {
	    cout << "Unbalance: " << _Height(parent->left_) << " << [" <<parent->key_ << "] >> " << _Height(parent->right_) << " -> diff: " << _Diff(parent) << endl;
	    if(_Diff(parent->left_) > 0) {
	      root = _RightRotate(parent);
	    } else {
	      root = _LRRotate(parent);
	    }
		cout << _Height(parent->left_) << " << [" <<parent->key_ << "] >> " << _Height(parent->right_) << " -> diff: " << _Diff(parent) << endl;
	  } else if(_Diff(parent) < -1) {
	    cout << "Unbalance: " << _Height(parent->left_) << " << [" <<parent->key_ << "] >> " << _Height(parent->right_) << " -> diff: " << _Diff(parent) << endl;
	    if(_Diff(parent->right_) < 0) {
	      root = _LeftRotate(parent);
	    } else {
	      root = _RLRotate(parent);
	    }
		cout << _Height(parent->left_) << " << [" <<parent->key_ << "] >> " << _Height(parent->right_) << " -> diff: " << _Diff(parent) << endl;
	  } else {
	    root = parent;
	  }
	}
	return root;
  }

  int _Diff(AVLNode *parent) {
    int right = _Height(parent->right_);
    int left = _Height(parent->left_);
    int diff = left - right;
//    cout << parent->key_ << ": " << left << " vs " << right << " diff: " << diff << endl;
    return diff;
  }
  
  AVLNode *_LeftRotate(AVLNode *node) { // right rotation once!
    cout << "_LeftRotate["<<node->key_<<"]" << endl;
    AVLNode *root = node->right_;
    node->right_ = root->left_;
    root->left_ = node;
    return root;
  }
  
  AVLNode *_RightRotate(AVLNode *node) { // right rotation once!
    cout << "_RightRotate["<<node->key_<<"]" << endl;
    AVLNode *root = node->left_;
    node->left_ = root->right_;
    root->right_ = node;
    return root;
  }

  
  AVLNode *_LRRotate(AVLNode *node) { // right rotation once!
    cout << "_LRRotate["<<node->key_<<"]" << endl;
	node->left_ = _LeftRotate(node->left_);
    return _RightRotate(node);
	
  }
  
  AVLNode *_RLRotate(AVLNode *node) { // right rotation once!
    cout << "_RLRotate["<<node->key_<<"]" << endl;
	node->right_ = _RightRotate(node->right_);
    return _LeftRotate(node);
  }

  void Traverse() {
    _Traverse(root_);
    cout << endl;
  }
  
  void _Traverse(AVLNode *node) {
    if(node) {
      cout << node->key_ << " " << " h:" << _Height(node) << " diff: " << _Diff(node) << endl;
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
    if(!node) {
      height = 0;
    } else {
      left_height = _Height(node->left_);
      right_height = _Height(node->right_);
      height = _MAX(left_height, right_height) + 1;
    }
    return height;
  }
  AVLNode *root_;
};

class AVL_GTest : public ::testing::Test {

protected:
  AVL *tree_;
//  int leafs_key_[15] = {55, 23, 91, 20, 25, 80, 120, 15, 21, 24, 28, 70, 85, 110, 150};
//  int leafs_key_[3] = {30, 10, 20};
  int leafs_key_[11] = {20, 10, 40, 30, 50, 60, 70, 160, 150, 25, 28};

  virtual void SetUp(){
  tree_ = new AVL();
  }
  
  virtual void TearDown(){
    delete tree_;
  }
};

TEST_F(AVL_GTest, AVLInsertBalance_GTest){
  for(unsigned int i = 0; i < (sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
	tree_->root_ = tree_->Insert(leafs_key_[i], tree_->root_);
  }
  tree_->Traverse();
  cout << endl;
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


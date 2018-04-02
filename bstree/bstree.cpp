#include <assert.h>
#include <iostream>
#include <string>
#include <queue>

#include <gtest/gtest.h>

using std::cout;
using std::cin;
using std::endl;

using std::queue;

#define MAX(x, y) (x>y?x:y)

#define OFSSET_START 8
#define LEVEL_OFFSET 4

//#define SHOW_TREE
#ifdef SHOW_TREE
#define OFFSET_UNIT " "
#define NEW_LINE endl
#else
#define OFFSET_UNIT ""
#define NEW_LINE "";
#endif

inline void OFFSET(int n) {
	for(int i = 0; i < n*LEVEL_OFFSET; i++) {
		cout << OFFSET_UNIT;
	}
}

inline void START() {
	for(int i = 0; i < OFSSET_START; i++) {
		cout << OFFSET_UNIT;
	}
}

inline void NETLINE() {
	cout << NEW_LINE;
}

class Node
{
public:
  Node(int key)  : key_(key), left_(nullptr), right_(nullptr) {};
  
  ~Node() {
    left_ = nullptr;
    right_ = nullptr;
    key_ = 0;
    ofs_ = 0;
  };
  int key_;
  Node *left_;
  Node *right_;
	int ofs_; // Just for showing the tree structure use
};

class BSTree
{
public:
  BSTree():root_(nullptr){};
  
  ~BSTree(){
		if(root_) {
			_Delete(root_);
			root_ = nullptr;
		}
	};
  
  void Insert(int key) {
    if(!root_) {
		  root_ = new Node(key);
		  return;
    }
		root_ = _Insert(key, root_);
    return;
  }
  
  bool Delete(int key) {
    if(!root_) {
      cout << "Delete failed on a empty tree" << endl;
      return false;
    }
    Node *del_node = _Delete(key, root_, nullptr);
    if(del_node)
      return true;
    else
      return false;
  }

  void TraverseInOrder() {
    cout << "In Order:" << endl;
    _InTravse(root_);
    cout << endl;
  }
  
  void TraversePreOrder() {
    cout << "Pre Order:" << endl;
    _PreTravse(root_);
    cout << endl;
  }
  
  void TraversePostOrder() {
    cout << "Post Order:" << endl;
    _PostTravse(root_);
    cout << endl;
  }
	
  void TraverseTopBottom() {
    cout << "Top Bottom:" << endl;
    int height_pre = Height(root_);
    int height_cur = height_pre;
    root_->ofs_ = height_cur;
    node_queue_.empty();
    node_queue_.push(root_);
    START();
    while(node_queue_.size()) {
      Node *cur = node_queue_.front();
      height_cur = Height(cur);
      //cout << cur->key_ << ": " << height_cur << endl;
      if(cur && cur->left_) {
        cur->left_->ofs_ = cur->ofs_ - 1;
        node_queue_.push(cur->left_);
      }
      if(cur && cur->right_) {
        cur->right_->ofs_ = cur->ofs_ + 1;
        node_queue_.push(cur->right_);
      }
      cout << cur->key_ << " ";
      //cout << "[" << cur->ofs_ << "]" << " ";
      OFFSET(cur->ofs_);
      node_queue_.pop();
      if(height_pre != height_cur) {
        NETLINE();
        START();
        height_pre = height_cur;
      }
    }
    cout << endl;
  }

  int Height(Node *parent) {
    int height = 0;
    int height_left = 0;
    int height_right = 0;
  
    if(!parent) {
      height = 1;
    } else {
      height_left = Height(parent->left_);
      height_right = Height(parent->right_);
      height = MAX(height_left, height_right) + 1;
    }
    return height;
  }
	
private:
  Node *_Insert(int key, Node *parent) {
    if(key == parent->key_) {
    
    } else if(key < parent->key_) {
      if(parent->left_) {
        parent->left_ = _Insert(key, parent->left_);
      } else {
        parent->left_ = new Node(key);
        cout << parent->left_->key_ << " <- " << parent->key_ << endl;
      }
    } else {
      if(parent->right_) {
        parent->right_ = _Insert(key, parent->right_);
      } else {
        parent->right_ = new Node(key);
        cout << parent->key_ << " -> " << parent->right_->key_  <<  endl;
      }
    }
    
    return parent;
  }
  
  Node *_Delete(int key, Node *parent, Node *grandpa) {
    Node *del_node = nullptr;
    if(key == parent->key_) {
      bool left = true;
      cout << "Binggo => " << key << endl;
      Node *swap_node = _LeftMostNode(parent);
      if(swap_node == parent) { // no left subtree
        left = false;
        cout << swap_node->key_ << " has no left subtree ";
        swap_node = _RightMostNode(parent); 
        if(swap_node == parent) { // no left and no right and subtree, it's a leaf node, just delete the leaf.
          cout << ",and no right subtree neither." << endl;
          delete parent;
          del_node = parent;
          if(grandpa) { // delete the root node
            if(grandpa->left_ == parent) {
              grandpa->left_ = nullptr;
            } else {
              grandpa->right_ = nullptr;
            }
          } else {
            root_ = nullptr;
          }
          return del_node;
        }
      }
      
			cout << "swap " << parent->key_ << " with " << swap_node->key_ << endl;
			cout << "swap to del " << swap_node->key_ << endl;
      parent->key_ = swap_node->key_;
      if(left) {
        del_node = _Delete(swap_node->key_, parent->left_, parent);
      } else {
        del_node = _Delete(key, parent->right_, parent);
      }
    } else if(key < parent->key_) {
      if(parent->left_) {
        del_node = _Delete(key, parent->left_, parent);
      }
    } else {
      if(parent->right_) {
        del_node = _Delete(key, parent->right_, parent);
      }
    }
    return del_node;
  }

  void _Delete(Node *parent) {
    if(parent->left_) {
      _Delete(parent->left_);
    }
    if(parent->right_) {
      _Delete(parent->right_);
    }
    if(!(parent->left_)&&!(parent->right_)) {
      delete parent;
    }
  }
	
  void _InTravse(Node *parent) {
    if(parent) {
	  _InTravse(parent->left_);
      cout << parent->key_ << " ";
      _InTravse(parent->right_);
    }
  }
  
  void _PreTravse(Node *parent) {
    if(parent) {
      cout << parent->key_ << " ";
      _PreTravse(parent->left_);
      _PreTravse(parent->right_);
    }
  }
  
  void _PostTravse(Node *parent) {
    if(parent) {
	    _PostTravse(parent->left_);
      _PostTravse(parent->right_);
	    cout << parent->key_ << " ";
    }
  }

	Node *_LeftMostNode(Node *parent) {
		Node *left_most = parent;
		if(parent->left_) {
			left_most = _LeftMostNode(parent->left_);
		}
		return left_most;
	}
	
	Node *_RightMostNode(Node *parent) {
		Node *right_most = parent;
		if(parent->left_) {
			right_most = _RightMostNode(parent->left_);
		}
		return right_most;
	}
	
  Node *root_;
  queue<Node *> node_queue_;
};

class BSTree_GTest : public ::testing::Test {

protected:
  BSTree *tree_;
  int leafs_key_[7] = {100, 50, 250, 20, 70, 200, 300};
  virtual void SetUp(){
    tree_ = new BSTree();
    for(unsigned int i = 0; i < (sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
      tree_->Insert(leafs_key_[i]);
    }
  }
  
  virtual void TearDown(){
    delete tree_;
  }
};

TEST_F(BSTree_GTest, TraverseTopBottom_GTest){
  tree_->TraversePreOrder();
  tree_->TraverseTopBottom();
}

TEST_F(BSTree_GTest, DeleteNode_GTest){
  int input;
  bool ret = false;
  do {
    cout << "Please input node key to delete: " << endl;
    cin >> input;
    ret = tree_->Delete(input);
    if(!ret) {
      cout << "Delete " << input << " failed!" << endl;
    } else {
      cout << "Delete " << input << " Success!" << endl;
    }
    tree_->TraversePreOrder();
    tree_->TraverseTopBottom();
  }while(ret);
}


int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

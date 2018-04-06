#include <assert.h>
#include <iostream>
#include <string>
#include <queue>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <gtest/gtest.h>

using std::cout;
using std::cin;
using std::endl;

using std::queue;
using std::vector;
using std::find;

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

class RandBox
{
public:
  RandBox(int size, int min, int max):size_(size), min_(min), max_(max), dis_(max - min) {
    if(size_ > dis_) {
      cout << "size : " << size << " must not larger than dis=(max-min): " << dis_ << endl;
      assert(0);
    }
    bool *marks = new bool[size_];
    for(int i; i < size_; i ++) {
      marks[i] = false;
    }
    orig_ = new int[size];
    cout << "min: " << min_ << " max: " << max_ << " dis: " << dis_ << endl;
		cout << "Initial rand vals:" << endl;
    for(int i = 0; i < size_; i ++) {
      int val = rand()%dis_;
      int limit = dis_ + 1;
      do {
				vector<int>::iterator itor = find(rands_.begin(), rands_.end(), val);
        if(itor != rands_.end()) {
          val ++;
          val %= dis_;
          limit --;
        } else {
          orig_[i] = val;
					rands_.push_back(orig_[i]);
					cout << orig_[i]+min_ << " ";
					break;
        }
      }while(limit);
      
      if(!limit) {
        cout << "create rand failed!" << endl;
      }
    }
    cout << endl;
    delete marks;
  }
  
  ~RandBox(){
    rands_.clear();
    size_ = 0;
    min_ = 0;
    max_ = 0;
    dis_ = 0;
    delete orig_;
  };

  void Shake(unsigned int seed) {
    rands_.clear();
    srand(seed);
    int *rand_pos = new int[size_];
    bool *marks = new bool[size_];
    for(int i = 0; i < size_; i ++) {
      marks[i] = false;
    }
    for(int i = 0; i < size_; i ++) {
      int pos = rand()%size_;
      int limit = size_;
      do {
        if(marks[pos]) {
          pos ++;
          pos %= size_;
          limit --;
        } else {
          marks[pos] = true;
          rand_pos[i] = pos;
          break;
        }
      }while(limit);
    }
    cout << "Shake seed: " << seed << endl;
    vector<int>::iterator itor;
		for(int i = 0; i < size_; i ++) {
		  int pos = rand_pos[i];
		  cout << orig_[pos] << " ";
		  rands_.push_back(orig_[pos]);
		}
		cout << endl;
		delete marks;
		delete rand_pos;
  }

  bool GetRand(int *val) {
    bool ret = false;
    if(!rands_.empty()) {
      *val = (rands_.back() + min_);
      rands_.pop_back();
      ret = true;
    }
    return ret;
  }

private:
  
  int size_;
  int min_;
  int max_;
  int dis_;
  int *orig_;
  vector<int> rands_;
};

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
  
	bool IsLeaf() {
	  bool leaf = true;
	  if(this->left_||this->right_){
	    leaf = false;
	  }
	  return leaf;
	}
	
  bool CopyKey(Node *src) {
    bool ret = false;
    if(src) {
		  key_ = src->key_;
    }
		return ret;
  }
	
	Node *RemoveChild(Node *child) {
	  Node *inherit_child = nullptr;
		if((child->left_)&&(child->right_)) {
			cout << "RemoveChild Failed: Child "<< child->key_ << " has two sub children" << endl;
			assert(0);
		}
	  if(child) {
		  cout << "Remove child: " << child->key_;
			if(!(child->left_)&&!(child->right_)) {
				inherit_child = nullptr;
			} else if(child->left_) {
				cout << " hookup its left sub child: " <<  child->left_->key_ << endl;
			  inherit_child = child->left_;
			} else {
				cout << " hookup its right sub child: " <<  child->right_->key_ << endl;
			  inherit_child = child->right_;
			}
			cout << endl;
			delete child;
	  }
		return inherit_child;
	}
	
	void RemoveLChild() {
	  if(left_) {
			Node *del_node = left_;
			if((left_->left_)&&(left_->right_)) {
				cout << "RemoveLChild Failed: Child "<< left_->key_ << " has two sub children" << endl;
				assert(0);
			}
			if(!(left_->left_)&&!(left_->right_)) {
				cout << key_ << " remove left child: " << left_->key_ << endl;
				left_ = nullptr;
			} else if(left_->left_) {
				cout << key_ << " remove left child: " << left_->key_ << " hookup its left sub child: " <<  left_->left_->key_ << endl;
			  left_ = left_->left_;
			} else {
				cout << key_ << " remove left child: " << left_->key_ << " hookup its right sub child: " <<  left_->right_->key_ << endl;
			  left_ = left_->right_;
			}
			delete del_node;
	  }
	}
	
	void RemoveRChild() {
	  if(right_) {
			Node *del_node = right_;
			if((right_->left_)&&(right_->right_)) {
				cout << "RemoveRChild Failed: Child "<< right_->key_ << " has two sub children" << endl;
				assert(0);
			}
			if(!(right_->left_)&&!(right_->right_)) {
				cout << key_ << " remove right child: " << right_->key_ << endl;
				right_ = nullptr;
			} else if(right_->left_) {
				cout << key_ << " remove right child: " << right_->key_ << " hookup its left sub child: " <<  right_->left_->key_ << endl;
				right_ = right_->left_;
			} else {
				cout << key_ << " remove right child: " << right_->key_ << " hookup its right sub child: " <<  right_->right_->key_ << endl;
				right_ = right_->right_;
			}
			delete del_node;
	  }
	}
	int Key() {
	  return key_;
	}
	
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

  bool IsEmpty() {
    return (root_ == nullptr);
  }
  
  void Insert(int key) {
    if(!root_) {
		  root_ = new Node(key);
		  return;
    }
		root_ = _Insert(key, root_);
    return;
  }
  
  bool Delete(int key) {
		bool ret = false;
    if(!root_) {
      cout << "Delete failed on a empty tree" << endl;
      return false;
    }
		if((key == root_->key_)&&\
			 (!root_->left_)&&\
			 (!root_->right_)){
			 cout << "Delete root_ become an empty tree!" << endl;
			 delete root_;
			 root_ = nullptr;
			 ret = true;
		} else {
			root_ = _Delete(key, root_);
			if(root_)
				ret =  true;
			else
				ret =  false;
		}
//		cout << "del: " << key << " ret: " << ret << endl;
		return ret;
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
    if(!root_) {
			cout << "Empty Tree, Can't Traverse!" << endl;
			return ;
    }
    cout << "Top Bottom:" << endl;
    int node_cnt = 0;
    int height_pre = _Height(root_);
    int height_cur = height_pre;
    root_->ofs_ = height_cur;
    node_queue_.empty();
    node_queue_.push(root_);
    node_cnt ++;
    START();
    while(node_queue_.size()) {
      Node *cur = node_queue_.front();
      height_cur = _Height(cur);
      //cout << cur->key_ << ": " << height_cur << endl;
      if(cur && cur->left_) {
        cur->left_->ofs_ = cur->ofs_ - 1;
        node_queue_.push(cur->left_);
			  node_cnt ++;
      }
      if(cur && cur->right_) {
        cur->right_->ofs_ = cur->ofs_ + 1;
        node_queue_.push(cur->right_);
				node_cnt ++;
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
    cout << "Total: " << node_cnt << " nodes" << endl;
  }
  
  int Height(void) {
    int height = 0;
    height = _Height(root_);
    return height;
  }

	
private:
	int _Height(Node *parent) {
		int height = 0;
		int height_left = 0;
		int height_right = 0;
	
		if(!parent) {
			height = 1;
		} else {
			height_left = _Height(parent->left_);
			height_right = _Height(parent->right_);
			height = MAX(height_left, height_right) + 1;
		}
		return height;
	}

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
  
	Node *_LeftMostNode(Node *parent) {
		if(parent->left_) {
			return _LeftMostNode(parent->left_);
		} else {
		  return parent;
		}
	}
	
	Node *_RightMostNode(Node *parent) {
		if(parent->right_) {
			return _RightMostNode(parent->right_);
		} else {
		  return parent;
		}
	}
#if 1
  Node *_Delete(int key, Node *parent) {
    Node *del_node = parent;
    if(key == parent->key_) {
      //cout << "Binggo <= " << key << endl;
      Node *swap_node = nullptr;
      if(parent->IsLeaf()) { // First delete node is a leaf.
        cout << parent->key_ << " is a leaf, just del the leaf" << endl;
        cout << "Del => " << parent->key_ << endl;
        delete parent;
			  del_node = nullptr;
      } else { // First delete node is not leaf, swap delete node
        if(parent->left_) {
  			  swap_node = _RightMostNode(parent->left_);
  				cout << "Right most node: " << swap_node->key_ << " in " << parent->key_ << " left sub tree" << endl;
  				if(swap_node == parent->left_) {
  					parent->CopyKey(swap_node);
  				  parent->left_ = parent->RemoveChild(swap_node);
  				} else {
  					cout << "Swap Del =>" << swap_node->Key() << endl;
  				  parent->CopyKey(swap_node);
  					parent->left_ = _Delete(parent->Key(), parent->left_);
  				}
        } else {
  				swap_node = _LeftMostNode(parent->right_);
  				cout << "Left most node: " << swap_node->key_ << " in " << parent->key_ << " right sub tree" << endl;
  				if(swap_node == parent->right_) {
  					parent->CopyKey(swap_node);
  					parent->right_ = parent->RemoveChild(swap_node);
  				} else {
  					cout << "Swap Del =>" << swap_node->Key() << endl;
  					parent->CopyKey(swap_node);
  					parent->right_ = _Delete(parent->Key(), parent->right_);
          }
        }
        del_node = parent;
      }
    } else if(key < parent->key_) {
      if(parent->left_) {
        parent->left_ = _Delete(key, parent->left_);
				del_node = parent;
      }
    } else {
      if(parent->right_) {
        parent->right_ = _Delete(key, parent->right_);
				del_node = parent;
      }
    }
    return del_node;
  }	
#else
  Node *_Delete(int key, Node *parent) {
    Node *del_node = nullptr;
    if(key == parent->key_) {
      cout << "Binggo <= " << key << endl;
      Node *swap_node = nullptr;
		  Node *swap_parent = parent;
      if(parent->IsLeaf()) { // First delete node is a leaf.
        cout << parent->key_ << " is a leaf, just del the leaf" << endl;
        cout << "Del => " << parent->key_ << endl;
        delete parent;
			  del_node = nullptr;
      } else { // First delete node is not leaf, swap delete node
				if(parent->right_) {
					swap_node = parent->right_;
					while(swap_node->left_){
						swap_parent = swap_node;
						cout << "new swap_parent: " << swap_parent->key_ << endl;
						swap_node = swap_node->left_;
					};
					cout << "Left most node: " << swap_node->key_ << " in " << parent->key_ << " right sub tree" << endl;
				} else {
					swap_node = parent->left_;
					while(swap_node->right_){
						swap_parent = swap_node;
						swap_node = swap_node->right_;
					};
					cout << "Right most node: " << swap_node->key_ << " in " << parent->key_ << " left sub tree" << endl;
				}
				cout << "Parent: " << parent->key_ << endl;
				parent->CopyKey(swap_node);
				swap_parent->RemoveChild(swap_node);
        del_node = parent;
      }
    } else if(key < parent->key_) {
      if(parent->left_) {
        parent->left_ = _Delete(key, parent->left_);
				del_node = parent;
      }
    } else {
      if(parent->right_) {
        parent->right_ = _Delete(key, parent->right_);
				del_node = parent;
      }
    }
    return del_node;
  }
#endif
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

TEST_F(BSTree_GTest, RandBox_GTest){
  int rand_val = 0;
  RandBox *box = new RandBox(100, 0, 101);
  box->Shake(0);
  box->Shake(0);
  //box->Shake(unsigned(time(nullptr)));
  
  bool ret = box->GetRand(&rand_val);
  cout << "GetRand:" << endl;
  while(ret) {
		cout << rand_val << " ";
    ret = box->GetRand(&rand_val);
  }
  cout << endl;
}

TEST_F(BSTree_GTest, RandomNode_GTest){
  int val = 0;
  const int node_num = 1000;
  const int node_val_max = 1500;
  const int node_val_min = 0;
  BSTree *tree = new BSTree();
  RandBox *rbox = new RandBox(node_num, node_val_min, node_val_max);
  bool ret = rbox->GetRand(&val);
  int insert_cnt = 0;
  while(ret){
    tree->Insert(val);
    ret = rbox->GetRand(&val);
    insert_cnt ++;
  }
  EXPECT_EQ(insert_cnt, node_num);
  //tree->TraversePreOrder();
  

  rbox->Shake(0);
  ret = rbox->GetRand(&val);
  int del_cnt = 0;
  while(ret){
		cout << "Kill <" << val <<">" << endl;
    EXPECT_TRUE(tree->Delete(val));
		//tree->TraversePreOrder();
    ret = rbox->GetRand(&val);
    del_cnt ++;
  }
  EXPECT_EQ(del_cnt, node_num);
  EXPECT_TRUE(tree->IsEmpty());
  
	//tree->TraversePreOrder();
  //tree->TraverseTopBottom();
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

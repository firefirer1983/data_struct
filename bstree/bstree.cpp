#include <assert.h>
#include <iostream>
#include <string>
#include <queue>
#include <stdlib.h>

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

bool *nodes_marks = nullptr;

int Random(int start, int end){
  
  int dis = end - start;
  int limit = dis;
  int rand_val = rand() % dis + start;
  do{
    if(!nodes_marks[rand_val]) {
      nodes_marks[rand_val] = true;
      break;
    }else{
      rand_val = rand_val - start + 1 ;
      rand_val = rand_val%dis + start;
      limit --;
    }
  }while(limit);
  return rand_val;
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
  
	bool IsLeaf() {
	  bool leaf = true;
	  if(this->left_||this->right_){
	    leaf = false;
	  }
	  return leaf;
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
  
  void Insert(int key) {
    if(!root_) {
		  root_ = new Node(key);
		  return;
    }
		root_ = _Insert(key, root_);
    return;
  }
  
  bool Delete(int key) {
    Node *del_node = nullptr;
		bool ret = false;
    if(!root_) {
      cout << "Delete failed on a empty tree" << endl;
      return false;
    }
		if((key == root_->key_)&&\
			 (!root_->left_)&&\
			 (!root_->right_)){
			 root_ = _Delete(key, root_);
			 ret = true;
		} else {
			del_node = _Delete(key, root_);
			root_ = del_node;
			if(del_node)
				ret =  true;
			else
				ret =  false;
		}
		cout << "del: " << key << " ret: " << ret << endl;
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
  
  Node *_Delete(int key, Node *parent) {
    Node *del_node = nullptr;
    if(key == parent->key_) {
      cout << "Binggo => " << key << endl;
      Node *swap_node = nullptr;
		  Node *swap_parent = parent;
      if(parent->IsLeaf()) { // First delete node is a leaf.
        cout << parent->key_ << " is a leaf, just del the leaf" << endl;
        delete parent;
			  del_node = nullptr;
      } else { // First delete node is not leaf, swap delete node
				if(parent->right_) {
					swap_node = parent->right_;
					while(swap_node->left_){
						swap_parent = swap_node;
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
				
				if(swap_node->IsLeaf()) {
					cout << "Swap node is a leaf, just del the swap node." << endl;
					if(swap_parent != parent) {
						parent->key_ = swap_node->key_;
					} else {
						swap_parent->key_ = swap_node->key_;
					}
					if(swap_parent->left_ == swap_node) {
						swap_parent->left_ = nullptr;
					} else {
						swap_parent->right_ = nullptr;
					}
					delete swap_node;
				} else {
					cout << "Swap " << parent->key_ << " with " << swap_node->key_ << endl;
					cout << "Swap to del " << swap_node->key_ << endl;
					parent->key_ = swap_node->key_;
					_Delete(swap_node->key_, swap_node);
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
		if(parent->right_) {
			right_most = _RightMostNode(parent->right_);
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

TEST_F(BSTree_GTest, RandomNode_GTest){
  const int node_num = 100;
  const int node_val_max = 100;
  const int node_val_min = 0;
  BSTree *tree = new BSTree();
  int *rand_nodes = (int*)malloc(node_num*sizeof(int));
  int *rand_idx = (int*)malloc(node_num*sizeof(int));
  nodes_marks = (bool*)malloc(node_val_max*sizeof(bool));
  for(int i = 0; i < node_val_max; i ++) {
    nodes_marks[i] = false;
  }
  cout << "random nodes: :" << endl;
  for(int i = 0; i < node_num; i ++) {
    rand_nodes[i] = Random(node_val_min, node_val_max);
    cout << rand_nodes[i]<< " ";
  }
  cout << endl;
  for(int i = 0; i < node_num; i ++) {
    tree->Insert(rand_nodes[i]);
  }
  
  tree->TraverseTopBottom();
  int node_cnt = node_num;
  delete []nodes_marks;
  nodes_marks = (bool*)malloc(node_num*sizeof(bool));
  for(int i = 0; i < node_num; i ++) {
    nodes_marks[i] = false;
  }
  cout << "random index :" << endl;
  for(int i = 0; i < node_num; i ++) {
    int idx = Random(0, node_num);
    rand_idx[i] = idx;
    cout << idx << " ";
  }
  cout << endl;
  
  cout << "random to del :" << endl;
  for(int i = 0; i < node_num; i ++) {
    cout << rand_nodes[rand_idx[i]]<< " ";
  }
  cout << endl;
	cout << "Tree: " << endl;
  tree->TraversePreOrder();
  for(int i = 0; i < node_num; i ++) {
    int node_key = rand_nodes[rand_idx[i]];
    bool ret = tree->Delete(node_key);
    if(ret) {
      node_cnt --;
      cout << "Success del: " << node_key << " " << node_cnt << " nodes left" << endl;
    } else {
      cout << "Failed del: " << node_key << endl;
    }
    tree->TraversePreOrder();
  }
  tree->TraverseTopBottom();
  delete []nodes_marks;
  delete []rand_nodes;
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

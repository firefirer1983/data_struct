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

#define CHK_BLACK(x) (x?x->IsBlack():true)

#define CHK_RED(x) (x?x->IsRed():false)

#define COLOR_STR(x) (x?(x->IsBlack()?"BLACK":"RED"):"BLACK")

#define CHG2_BLACK(x) \
do{\
  if(x){\
    x->TurnBlack();\
  }\
}while(0)

#define CHG2_RED(x) \
do{\
	if(x){\
		x->TurnRed();\
	}\
}while(0)


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
  Node(int key)  : key_(key), left_(nullptr), right_(nullptr), red_(true){};
  
  ~Node() {
    left_ = nullptr;
    right_ = nullptr;
    key_ = 0;
    ofs_ = 0;
  };
  
	bool IsLeaf() {
	  bool leaf = true;
	  if(left_||right_){
	    leaf = false;
	  }
	  return leaf;
	}
	
	bool IsBlack() {
	  return !red_;
	}
	
	bool IsRed() {
	  return red_;
	}
	void TurnBlack(void) {
	  red_ = false;
	}
	
	void TurnRed(void) {
		red_ = true;
	}

	bool RedConflict(void) {
	  if(IsRed()) {
	    return ((left_&&left_->IsRed())||(right_&&right_->IsRed()));
	  } else {
	    return false;
	  }
	}
	
  int key_;
  Node *left_;
  Node *right_;
  bool red_;
	int ofs_; // Just for showing the tree structure use
};

class RBTree
{
public:
  RBTree():root_(nullptr){};
  
  ~RBTree(){
		if(root_) {
			_Delete(root_);
			root_ = nullptr;
		}
	};

  bool IsEmpty() {
    return (root_ == nullptr);
  }
  
  void Insert(int key) {
    cout << "INSERT ==> " << key << endl;
    if(!root_) {
		  root_ = new Node(key);
		  cout << "insert " << key << " on root" << endl;
    } else {
		  root_ = _Insert(key, root_);
		}
		root_->TurnBlack();
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

	int _Diff(Node *parent) {
	  int diff = 0;
	  if(parent) {
  		int right = _Height(parent->right_);
  		int left = _Height(parent->left_);
  		diff = left - right;
    }
		//		cout << parent->key_ << ": " << left << " vs " << right << " diff: " << diff << endl;
		return diff;
	}
	
	int _BHeight(Node *parent) {
		int height = 0;
		int height_left = 0;
		int height_right = 0;
	
		if(!parent) {
			height = 1;
		} else {
			height_left = _BHeight(parent->left_);
			height_right = _BHeight(parent->right_);
			height = MAX(height_left, height_right) + (parent->IsBlack()?1:0);
		}
		return height;
	}

	int _BDiff(Node *parent) {
	  int diff = 0;
	  if(parent) {
  		int right = _BDiff(parent->right_);
  		int left = _BDiff(parent->left_);
  		diff = left - right;
  		//		cout << parent->key_ << ": " << left << " vs " << right << " diff: " << diff << endl;
		}
		return diff;
	}

/*
				 red/black tree insert rules
				 
1. always insert red node

2. if insert root node, root=>black -> done

3. if parent(black) -> done

4. if parent(red)&&uncle(red)
	 parent=>black uncle=>black  and grandpa=>red, recheck grand parent, push black to bottom untill the root
	 
5. if parent(red)&&uncle(black)
	 5a: parent(left)&&node(left): 
	i.	rightRotate
	ii. recolor:	parent=>black, children=>red

	 5b: parent(right)&&node(right):
	i.	leftRotate
	ii. recolor: parent=>black, children=>red
	
	 5c: parent(left)&&node(right):
	i. leftRotate + rightRotate
	ii. recolor: parent=>black, children=>red

	 5d: parent(right)&&node(left):
	i. rightRotate + leftRotate
	ii. recolor: parent=>black, children=>red 	
*/

  Node *_Insert(int key, Node *parent) {
    Node *ret_node = parent;
    Node *new_node = nullptr;
    bool left = false;
    if(key == parent->key_) { // key already exist in the tree, just return what it is
      return ret_node;
    } else if(key < parent->key_) {
      if(parent->left_) {
        parent->left_ = _Insert(key, parent->left_);
      } else {
        parent->left_ = new Node(key);
        cout << parent->left_->key_ << " <- " << parent->key_ << endl;
				new_node = parent->left_;
      }
      left = true;
    } else {
      if(parent->right_) {
        parent->right_ = _Insert(key, parent->right_);
      } else {
        parent->right_ = new Node(key);
        cout << parent->key_ << " -> " << parent->right_->key_  <<  endl;
				new_node = parent->right_;
      }
    }
    
    if(!new_node) {
      Node *sub_parent = nullptr;
      Node *sub_uncle = nullptr;
      if(left) {
        sub_parent = parent->left_;
        sub_uncle = parent->right_;
      } else {
        sub_parent = parent->right_;
        sub_uncle = parent->left_;
      }
      cout << "parent: " << COLOR_STR(sub_parent) << " uncle: " << COLOR_STR(sub_uncle) << endl;
      if(sub_parent->RedConflict()) {
				if(CHK_RED(sub_parent)&&CHK_RED(sub_uncle)) { // parent && uncle is RED: recolor
					cout << "parent && uncle are RED, recolor: "<< endl;
					cout << "parent && uncle => BLACK, grandpa => RED" << endl;
					CHG2_BLACK(sub_parent);
					CHG2_BLACK(sub_uncle);
					CHG2_RED(parent);
				} else if(CHK_RED(sub_parent)&&CHK_BLACK(sub_uncle)){
					cout << "parent RED && uncle BLACK, rotate and recolor: "<< endl;
					int diff = _Diff(parent);
					cout << parent->key_ << " diff: " << diff << endl;
					if(diff > 1) {
						diff = _Diff(parent->left_);
						cout << parent->left_->key_ << " diff: " << diff << endl;
						if(diff > 0) {
							CHG2_RED(parent); // grandpa => RED
							CHG2_BLACK(parent->left_);// parent => BLACK
							CHG2_RED(parent->left_->left_); // child => RED
							cout << "grandpa: " << COLOR_STR(parent) << "  ";
							cout << "parent: " << COLOR_STR(parent->left_) << "  ";
							cout << "child: " << COLOR_STR(parent->left_->left_) << "  ";
							cout << endl;
							ret_node = _RightRotate(parent);
						} else {
							CHG2_RED(parent);
							CHG2_RED(parent->left_);
							CHG2_BLACK(parent->left_->right_);
							cout << "grandpa: " << COLOR_STR(parent) << "  ";
							cout << "parent: " << COLOR_STR(parent->left_) << "  ";
							cout << "child: " << COLOR_STR(parent->left_->right_) << "	";
							cout << endl;
							ret_node = _LRRotate(parent);
						}
					} else if(diff < -1) {
						diff = _Diff(parent->right_);
						cout << parent->right_->key_ << " diff: " << diff << endl;
						if(diff < 0) {
							CHG2_RED(parent); // grandpa => RED
							CHG2_BLACK(parent->right_);// parent => BLACK
							CHG2_RED(parent->right_->right_); // child => RED
							cout << "grandpa: " << COLOR_STR(parent) << "  ";
							cout << "parent: " << COLOR_STR(parent->right_) << "	";
							cout << "child: " << COLOR_STR(parent->right_->right_) << "  ";
							cout << endl;
							ret_node = _LeftRotate(parent);
						} else {
							CHG2_RED(parent);
							CHG2_RED(parent->right_);
							CHG2_BLACK(parent->right_->left_);
							cout << "grandpa: " << COLOR_STR(parent) << "  ";
							cout << "parent: " << COLOR_STR(parent->right_) << "	";
							cout << "child: " << COLOR_STR(parent->right_->left_) << "	";
							cout << endl;
							ret_node = _RLRotate(parent);
						}
					}
				}
      }
    }
    cout << "ret: " << ret_node->key_ << endl;
    return ret_node;
  }
  
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
					cout << "Swap node is a leaf, just del the swap node: " << swap_node->key_ << endl;
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
					cout << "Del => " << swap_node->key_ << endl;
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

    int diff = _Diff(parent);
    if(diff > 1) {
      diff = _Diff(parent->left_);
      if(diff > 0) {
        del_node = _RightRotate(parent);
      } else {
        del_node = _LRRotate(parent);
      }
    } else if(diff < -1){
      diff = _Diff(parent->right_);
      if(diff < 0) {
        del_node = _LeftRotate(parent);
      } else {
        del_node = _RLRotate(parent);
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
		  EXPECT_EQ(_BDiff(parent), 0);
      _InTravse(parent->right_);
    }
  }
  
  void _PreTravse(Node *parent) {
    if(parent) {
      cout << parent->key_ << ": " << COLOR_STR(parent) << "   ";
      EXPECT_EQ(_BDiff(parent), 0);
      _PreTravse(parent->left_);
      _PreTravse(parent->right_);
    }
  }
  
  void _PostTravse(Node *parent) {
    if(parent) {
	    _PostTravse(parent->left_);
      _PostTravse(parent->right_);
	    cout << parent->key_ << " ";
      EXPECT_EQ(_BDiff(parent), 0);
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
	
  Node *_LeftRotate(Node *node) {
    cout << "_LeftRotate["<<node->key_<<"]" << endl;
    Node *root = node->right_;
    node->right_ = root->left_;
    root->left_ = node;
    return root;
  }
  
  Node *_RightRotate(Node *node) {
    cout << "_RightRotate["<<node->key_<<"]" << endl;
    Node *root = node->left_;
    node->left_ = root->right_;
    root->right_ = node;
    return root;
  }
  
  Node *_LRRotate(Node *node) {
    cout << "_LRRotate["<<node->key_<<"]" << endl;
    node->left_ = _LeftRotate(node->left_);
    return _RightRotate(node);
  }
  
  Node *_RLRotate(Node *node) {
    cout << "_RLRotate["<<node->key_<<"]" << endl;
    node->right_ = _RightRotate(node->right_);
    return _LeftRotate(node);
  }
	
  Node *root_;
  queue<Node *> node_queue_;
};

class RBTree_GTest : public ::testing::Test {

protected:
  virtual void SetUp(){
  }
  
  virtual void TearDown(){
  }
};

TEST_F(RBTree_GTest, RandomNode_GTest){
  int val = 0;
  const int node_num = 25;
  const int node_val_max = 50;
  const int node_val_min = 0;
  RBTree *tree = new RBTree();
  RandBox *rbox = new RandBox(node_num, node_val_min, node_val_max);
  bool ret = rbox->GetRand(&val);
  int insert_cnt = 0;
  while(ret){
    tree->Insert(val);
    tree->TraversePreOrder();
    ret = rbox->GetRand(&val);
    insert_cnt ++;
  }
  EXPECT_EQ(insert_cnt, node_num);
  tree->TraversePreOrder();
  tree->TraverseTopBottom();
}


TEST_F(RBTree_GTest, InsertNode_GTest){
  int input;
  RBTree *tree = new RBTree();
  do {
    cout << "Please input node key to insert: " << endl;
    cin >> input;
    tree->Insert(input);
    tree->TraversePreOrder();
  }while(1);
}


#if 0
TEST_F(RBTree_GTest, TraverseTopBottom_GTest){
  tree_->TraversePreOrder();
  tree_->TraverseTopBottom();
}

TEST_F(RBTree_GTest, RandomNode_GTest){
  int val = 0;
  const int node_num = 50;
  const int node_val_max = 100;
  const int node_val_min = 0;
  RBTree *tree = new RBTree();
  RandBox *rbox = new RandBox(node_num, node_val_min, node_val_max);
  bool ret = rbox->GetRand(&val);
  int insert_cnt = 0;
  while(ret){
    tree->Insert(val);
    ret = rbox->GetRand(&val);
    insert_cnt ++;
  }
  EXPECT_EQ(insert_cnt, node_num);
  tree->TraversePreOrder();
  tree->TraverseTopBottom();
  

  rbox->Shake(0);
  ret = rbox->GetRand(&val);
  int del_cnt = 0;
  while(ret){
    EXPECT_TRUE(tree->Delete(val));
    ret = rbox->GetRand(&val);
    del_cnt ++;
    tree->TraversePreOrder();
  }
  EXPECT_EQ(del_cnt, node_num);
  EXPECT_TRUE(tree->IsEmpty());
  
	tree->TraversePreOrder();
  tree->TraverseTopBottom();
}

TEST_F(RBTree_GTest, DeleteNode_GTest){
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
#endif
int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

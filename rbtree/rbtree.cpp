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

#define RED 0
#define BLACK 1
#define BLACK_BLACK 2

//#define SHOW_TREE
#ifdef SHOW_TREE
#define OFFSET_UNIT " "
#define NEW_LINE endl
#else
#define OFFSET_UNIT ""
#define NEW_LINE "";
#endif

#define RSTT "\033[31m"
#define CEND "\033[0m"
#define BSTT "\033[34m"
#define GSTT "\033[32m"

const int NULL_KEY = 0xFFFFFFFF;

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

#define COLOR_STR(x) (x?(x->IsBlack()?(BSTT "BLACK" CEND):(x->IsRed()?(RSTT "RED" CEND):(GSTT "BLACK+BLACK" CEND))):(BSTT "BLACK" CEND))

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

#define CHG2_BLACKBLACK(x) \
do{\
	if(x){\
		x->TurnBlackBlack();\
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
  Node(int key)  : left_(nullptr), right_(nullptr), blackness_(RED), key_(key){};
  Node()  : left_(nullptr), right_(nullptr), blackness_(BLACK_BLACK){};
  ~Node() {
    left_ = nullptr;
    right_ = nullptr;
    key_ = NULL_KEY;
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
	  return blackness_ == BLACK;
	}

	bool Is2Black() {
	  return blackness_ == BLACK_BLACK;
	}
	
	bool IsRed() {
	  return blackness_ == RED;
	}

  Node *GetRedChild() {
		Node *red_child = nullptr;
		if(left_&&left_->IsRed()) {
		  red_child = left_;
		} else if(right_&&right_->IsRed()) {
		  red_child = right_;
		}
		return red_child;
  }
	
	Node *GetDoubleBlackChild() {
		Node *blackblack_child = nullptr;
		if(left_&&left_->Is2Black()) {
		  blackblack_child = left_;
		} else if(right_&&right_->Is2Black()) {
		  blackblack_child = right_;
		}
		return blackblack_child;
  }
	
  bool IsNull(void) {
		return key_ == NULL_KEY;
  }
  
  void TurnRed(void) {
    blackness_ = RED;
  }
	
  void TurnBlack(void) {
    blackness_ = BLACK;
  }

  void TurnBlackBlack(void) {
    blackness_ = BLACK_BLACK;
  }
	
  bool CopyKey(Node *src) {
    bool ret = false;
    if(src&&key_!=NULL_KEY&&src->key_!=NULL_KEY) {
			key_ = src->key_;
			ret = true;
    }
		return ret;
  }
	
  bool SwapKey(Node *src) {
  	bool ret = false;
  	if(src&&key_!=NULL_KEY&&src->key_!=NULL_KEY) {
  		int tmp = key_;
			key_ = src->key_;
		  src->key_ = tmp;
  		ret = true;
  	}
  	return ret;
  }

	Node *RemoveChild(Node *child) {
		if((child->left_)&&(child->right_)) {
			cout << "RemoveChild Failed: Child "<< child->key_ << " has two sub children" << endl;
			assert(0);
		}
	  if(child) {
			Node *del_node = child;
			Node **child_ptr = (child == left_?&left_:&right_);
      cout << "Delete node " << child->Key() << " : " << COLOR_STR(child) << endl;
			if(!(child->left_)&&!(child->right_)) {
				*child_ptr = nullptr;
			} else if(child->left_) {
				cout << " Hookup its left sub child: " <<  child->left_->key_ << endl;
			  *child_ptr = child->left_;
			} else {
				cout << " Hookup its right sub child: " <<  child->right_->key_ << endl;
			  *child_ptr = child->right_;
			}
			if(child->IsRed()) {
			} else {
			  if((*child_ptr)&&((*child_ptr)->IsRed())) {
				  cout << child->Key() << " has a " << COLOR_STR((*child_ptr)) << " child!" << endl;
			  } else {
				  cout << child->Key() << " has a " << COLOR_STR((*child_ptr)) << " child!" << endl;
				  *child_ptr = new Node();
					cout << "Replact with a double black null node" << endl;
			  }
			}
			cout << endl;
			delete del_node;
			return *child_ptr;
	  }
		return nullptr;
	}

	void ReplaceLeaf(Node *old_node, Node *new_node) {
	  if(!old_node->IsLeaf()) {
			cout << "ReplaceLeaf Failed: Child "<< old_node->key_ << " has two sub children" << endl;
			assert(0);
	  }
		if(old_node&&new_node) {
		  Node *del_node = old_node;
			Node **child_ptr = (old_node == left_?&left_:&right_);
		  cout << "Replace node " << old_node->key_ << " with " << new_node->Key() << endl;
			*child_ptr = new_node;
			delete del_node;
		}
		
	}
	inline int Key() {
	  return key_;
	}
	
	bool RedConflict(void) {
	  if(IsRed()) {
	    return ((left_&&left_->IsRed())||(right_&&right_->IsRed()));
	  } else {
	    return false;
	  }
	}
	
  Node *left_;
  Node *right_;
	int ofs_; // Just for showing the tree structure use
private:
  int blackness_;
  int key_;
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
		if((key == root_->Key())&&root_->IsLeaf()) {
			cout << "Delete root: " << root_->Key() << endl;
			cout << "Become Empty Tree!" << endl;
      delete root_;      
      root_ = nullptr;
      ret = true;
		} else {
			del_node = _Delete(key, root_);
			if(del_node) {
				if(del_node->Is2Black()) {
				  cout << "Case 3-1:" << endl;
					cout << "root_ " << root_->Key() << " : " << COLOR_STR(del_node) << endl;
				  CHG2_BLACK(del_node);
				}
			  root_ = del_node;
			  ret =  true;
			}	else {
				ret =  false;
			}
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
      //cout << cur->Key() << ": " << height_cur << endl;
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
      cout << cur->Key() << " ";
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

	void CountBlackNum() {
	  int prev_val = 0;
	  _BlackCount(root_, 0);
	  prev_val = black_cnt_.front();
	  for(vector<int>::iterator itor = black_cnt_.begin(); itor != black_cnt_.end(); itor ++) {
      EXPECT_EQ(prev_val, *itor);
      prev_val = *itor;
	  }
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
		//		cout << parent->Key() << ": " << left << " vs " << right << " diff: " << diff << endl;
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
  		int right = _BHeight(parent->right_);
  		int left = _BHeight(parent->left_);
  		diff = left - right;
  		//		cout << parent->Key() << ": " << left << " vs " << right << " diff: " << diff << endl;
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
	ii. recolor: grandpa=>red, parent=>black, children=>red 	

	 5b: parent(right)&&node(right):
	i.	leftRotate
	ii. recolor: grandpa=>red, parent=>black, children=>red
	
	 5c: parent(left)&&node(right):
	i. leftRotate + rightRotate
	ii. recolor: grandpa=> red, parent=>red, children=>black

	 5d: parent(right)&&node(left):
	i. rightRotate + leftRotate
	ii. recolor: grandpa=>red, parent=>red, children=>black 	
*/

  Node *_Insert(int key, Node *parent) {
    Node *ret_node = parent;
    Node *new_node = nullptr;
    bool left = false;
    if(key == parent->Key()) { // key already exist in the tree, just return what it is
      return ret_node;
    } else if(key < parent->Key()) {
      if(parent->left_) {
        parent->left_ = _Insert(key, parent->left_);
      } else {
        parent->left_ = new Node(key);
        cout << parent->left_->Key() << " <- " << parent->Key() << endl;
				new_node = parent->left_;
      }
      left = true;
    } else {
      if(parent->right_) {
        parent->right_ = _Insert(key, parent->right_);
      } else {
        parent->right_ = new Node(key);
        cout << parent->Key() << " -> " << parent->right_->Key()  <<  endl;
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
					cout << parent->Key() << " diff: " << diff << endl;
					if(diff > 1) {
						diff = _Diff(parent->left_);
						cout << parent->left_->Key() << " diff: " << diff << endl;
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
							CHG2_RED(parent); // grandpa => RED
							CHG2_RED(parent->left_); // parent => RED
							CHG2_BLACK(parent->left_->right_); // child => BLACK
							cout << "grandpa: " << COLOR_STR(parent) << "  ";
							cout << "parent: " << COLOR_STR(parent->left_) << "  ";
							cout << "child: " << COLOR_STR(parent->left_->right_) << "	";
							cout << endl;
							ret_node = _LRRotate(parent);
						}
					} else if(diff < -1) {
						diff = _Diff(parent->right_);
						cout << parent->right_->Key() << " diff: " << diff << endl;
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
							CHG2_RED(parent); // prandpa => RED
							CHG2_RED(parent->right_); // parent => RED
							CHG2_BLACK(parent->right_->left_); // child = > BLACK
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
    cout << "ret: " << ret_node->Key() << endl;
    return ret_node;
  }

  Node *_SolveDoubleBlack(Node *parent, Node *double_black) {
		Node *ret_node = nullptr;
    Node *sibling = ((double_black == parent->left_)?parent->right_:parent->left_);
		bool sibling_on_left = ((double_black == parent->left_)?false:true);
		cout <<"Parent " << parent->Key() <<" : " << COLOR_STR(parent) << endl;
		cout <<"Sibling " << sibling->Key()<<" : " << COLOR_STR(sibling) << " is on the " << (sibling_on_left?"LEFT":"RIGHT") << endl;
		cout <<"Sibling left child: " << COLOR_STR(sibling->left_) << endl;
		cout <<"Sibling right child: " << COLOR_STR(sibling->right_) << endl;
		
		if(sibling->IsRed()&&CHK_BLACK(parent)&&CHK_BLACK(sibling->left_)&&CHK_BLACK(sibling->right_)) { // Case 2, RED sibling with 2 black children
		  cout << "Case 3-2:" << endl;
		  CHG2_RED(parent);
			CHG2_BLACK(sibling);
		  sibling = _LeftRotate(parent);
			cout <<"Parent " << parent->Key() <<" : " << COLOR_STR(parent) << endl;
			cout <<"Sibling " << sibling->Key() <<" : " << COLOR_STR(sibling) << endl;
			cout <<"Sibling left child: " << COLOR_STR(sibling->left_) << endl;
			cout <<"Sibling right child: " << COLOR_STR(sibling->right_) << endl;
			ret_node = sibling;
		} else if(sibling->IsBlack()&&parent->IsBlack()&&CHK_BLACK(sibling->left_)&&CHK_BLACK(sibling->right_)) {
			cout << "Case 3-3:" << endl;
			if(double_black->IsNull()) {
			  if(sibling_on_left) {
					parent->right_ = nullptr;
			  } else {
					parent->left_ = nullptr;
			  }
			} else {
		    CHG2_BLACK(double_black);
			}
			CHG2_BLACKBLACK(parent);
			CHG2_RED(sibling);
			cout <<"Parent " << parent->Key() <<" : " << COLOR_STR(parent) << endl;
			cout <<"Sibling " << sibling->Key() <<" : " << COLOR_STR(sibling) << endl;
			cout <<"Sibling left child: " << COLOR_STR(sibling->left_) << endl;
			cout <<"Sibling right child: " << COLOR_STR(sibling->right_) << endl;
			ret_node = parent;
		} else if(sibling->IsBlack()&&parent->IsRed()&&CHK_BLACK(sibling->left_)&&CHK_BLACK(sibling->right_)) {
		  cout << "Case 3-4:" << endl;
			CHG2_BLACK(parent);
			cout << "Double black " << double_black->Key() << " : " << COLOR_STR(double_black) << endl;
			if(double_black->IsNull()) {
				cout << "Cut " << (sibling_on_left?"RIGHT":"LEFT") << " null " << COLOR_STR(double_black) << " node " << endl;
			  if(sibling_on_left) {
					parent->right_ = nullptr;
			  } else {
					parent->left_ = nullptr;
			  }
			} else {
		    CHG2_BLACK(double_black);
			}
			
			CHG2_RED(sibling);
			cout <<"Parent " << parent->Key() <<" : " << COLOR_STR(parent) << endl;
			cout <<"Sibling " << sibling->Key() <<" : " << COLOR_STR(sibling) << endl;
			cout <<"Sibling left child: " << COLOR_STR(sibling->left_) << endl;
			cout <<"Sibling right child: " << COLOR_STR(sibling->right_) << endl;
			ret_node = parent;
		} else if(sibling->IsBlack()&&\
			((sibling_on_left&&CHK_BLACK(sibling->left_)&&CHK_RED(sibling->right_)) ||\
			(!sibling_on_left&&CHK_BLACK(sibling->right_)&&CHK_RED(sibling->left_)))){
		  cout << "Case 3-5:" << endl;
			CHG2_RED(sibling);
			if(sibling_on_left) {
				CHG2_BLACK(sibling->right_);
			  parent->left_ = _LeftRotate(sibling);
			} else {
				CHG2_BLACK(sibling->left_);
			  parent->right_ = _RightRotate(sibling);
			}
			cout <<"Parent " << parent->Key() <<" : " << COLOR_STR(parent) << endl;
			cout <<"Sibling " << sibling->Key() <<" : " << COLOR_STR(sibling) << endl;
			cout <<"Sibling left child: " << COLOR_STR(sibling->left_) << endl;
			cout <<"Sibling right child: " << COLOR_STR(sibling->right_) << endl;
			ret_node = parent;
		} else if(sibling->IsBlack()&&\
			((sibling_on_left&&CHK_RED(sibling->left_)) ||\
			(!sibling_on_left&&CHK_RED(sibling->right_)))) {
		  cout << "Case 3-6:" << endl;
			if(sibling_on_left) {
				CHG2_BLACK(parent);
				CHG2_BLACK(sibling->left_);
        ret_node = _RightRotate(parent);
			} else {
				CHG2_BLACK(parent);
				CHG2_BLACK(sibling->right_);
        ret_node = _LeftRotate(parent);
			}
			cout << "Cut " << (sibling_on_left?"RIGHT":"LEFT") << " null " << COLOR_STR(double_black) << " node " << endl;
			if(double_black->IsNull()) {
			  if(sibling_on_left) {
					parent->right_ = nullptr;
			  } else {
					parent->left_ = nullptr;
			  }
			} else {
		    CHG2_BLACK(double_black);
			}
			cout <<"Parent " << parent->Key() <<" : " << COLOR_STR(parent) << endl;
			cout <<"Sibling " << sibling->Key() <<" : " << COLOR_STR(sibling) << endl;
			cout <<"Sibling left child: " << COLOR_STR(sibling->left_) << endl;
			cout <<"Sibling right child: " << COLOR_STR(sibling->right_) << endl;
		}
		return ret_node;
  }

  Node *_Delete(int key, Node *parent) {
    Node *del_node = parent;
    if(key == parent->Key()) {
      //cout << "Binggo <= " << key << endl;
      Node *swap_node = nullptr;
      if(parent->IsLeaf()) { // First delete node is a leaf.
        if(parent->IsRed()) {
          cout << parent->Key() << " : " << COLOR_STR(parent) <<" leaf, just del the leaf" << endl;
          delete parent;
          return nullptr;
        } else {
          cout << parent->Key() << " : " << COLOR_STR(parent) <<" leaf, add a null double black child node " << endl;
          cout << "Del => " << parent->Key() << endl;
          delete parent;
					del_node = new Node();
					return del_node;
        }
      } else { // First delete node is not leaf, swap delete node
        if(parent->left_) {
  			  swap_node = _RightMostNode(parent->left_);
  				cout << "Right most node: " << swap_node->Key() << " in " << parent->Key() << " left sub tree" << endl;
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
  				cout << "Left most node: " << swap_node->Key() << " in " << parent->Key() << " right sub tree" << endl;
  				if(swap_node == parent->right_) {
  					parent->CopyKey(swap_node);
  					parent->right_ = parent->RemoveChild(swap_node);
  				} else {
  					cout << "Swap Del =>" << swap_node->Key() << endl;
  					parent->CopyKey(swap_node);
  					parent->right_ = _Delete(parent->Key(), parent->right_);
          }
        }
      }
    } else if(key < parent->Key()) {
      if(parent->left_) {
        parent->left_ = _Delete(key, parent->left_);
      }
    } else {
      if(parent->right_) {
        parent->right_ = _Delete(key, parent->right_);
      }
    }
		Node *double_black = parent->GetDoubleBlackChild();
		if(double_black) {
			cout << "Parent " << parent->Key() << " child " << double_black->Key() << " : " << COLOR_STR(double_black) << endl;
			_SolveDoubleBlack(parent, double_black);
		}
    return parent;
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
      cout << parent->Key() << " ";
		  EXPECT_EQ(_BDiff(parent), 0);
			EXPECT_FALSE(parent->RedConflict());
      _InTravse(parent->right_);
    }
  }
  
  void _PreTravse(Node *parent) {
    if(parent) {
      cout << parent->Key() << ": " << COLOR_STR(parent) << "   ";
      EXPECT_EQ(_BDiff(parent), 0);
			EXPECT_FALSE(parent->RedConflict());
      _PreTravse(parent->left_);
      _PreTravse(parent->right_);
    }
  }
  
  void _PostTravse(Node *parent) {
    if(parent) {
	    _PostTravse(parent->left_);
      _PostTravse(parent->right_);
	    cout << parent->Key() << " ";
      EXPECT_EQ(_BDiff(parent), 0);
			EXPECT_FALSE(parent->RedConflict());
    }
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

  Node *_LeftRotate(Node *node) {
    cout << "_LeftRotate["<<node->Key()<<"]" << endl;
    Node *root = node->right_;
    node->right_ = root->left_;
    root->left_ = node;
    return root;
  }
  
  Node *_RightRotate(Node *node) {
    cout << "_RightRotate["<<node->Key()<<"]" << endl;
    Node *root = node->left_;
    node->left_ = root->right_;
    root->right_ = node;
    return root;
  }
  
  Node *_LRRotate(Node *node) {
    cout << "_LRRotate["<<node->Key()<<"]" << endl;
    node->left_ = _LeftRotate(node->left_);
    return _RightRotate(node);
  }
  
  Node *_RLRotate(Node *node) {
    cout << "_RLRotate["<<node->Key()<<"]" << endl;
    node->right_ = _RightRotate(node->right_);
    return _LeftRotate(node);
  }
  
	void _BlackCount(Node *parent, int num) {
		if(parent->IsBlack())
			num ++;
	  if(!(parent->left_)&&!(parent->right_)) {
	    
      black_cnt_.push_back(num + 1);
      cout << "End point " << parent->Key() << ": " << num + 1 << " black nodes" << endl;
    } else{
      if(parent->left_)
			  _BlackCount(parent->left_, num);
			if(parent->right_)
        _BlackCount(parent->right_, num);
    }
	}
	
  Node *root_;
  queue<Node *> node_queue_;
  vector<int> black_cnt_;
};

class RBTree_GTest : public ::testing::Test {

protected:
	RBTree *rbtree = new RBTree();
	//int leafs_key_[15] = {55, 23, 91, 20, 25, 80, 120, 15, 21, 24, 28, 70, 85, 110, 150};
	int leafs_key_[6] = {55, 23, 91, 20, 25, 15};
  virtual void SetUp(){
		cout << "SetUp Tree: " << endl;
		for(unsigned i = 0; i <(sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
		  cout << leafs_key_[i] << " ";
		}
		cout << endl;
	  for(unsigned i = 0; i < (sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
		  rbtree->Insert(leafs_key_[i]);
		}
		rbtree->TraversePreOrder();
  }
  
  virtual void TearDown(){
    delete rbtree;
  }
};


TEST_F(RBTree_GTest, RandomNode_GTest){
  int val = 0;
  const int node_num = 10;
  const int node_val_max = 100;
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
  tree->CountBlackNum();
  EXPECT_EQ(insert_cnt, node_num);
  tree->TraversePreOrder();
  tree->TraverseTopBottom();
}

TEST_F(RBTree_GTest, DeleteNode_GTest){
  int input;
  do {
    cout << "Please input node key to delete: " << endl;
    cin >> input;
    rbtree->Delete(input);
    rbtree->TraversePreOrder();
  }while(1);
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

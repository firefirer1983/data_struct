#include <assert.h>
#include <iostream>
#include <string>
#include <queue>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <gtest/gtest.h>
#include <stdarg.h>
#include <stdio.h>

using std::cout;
using std::cin;
using std::endl;

using std::queue;
using std::vector;
using std::find;

#define MAX(x, y) (x>y?x:y)

#define DBG 1
#ifdef DBG
#define print(...) dbg(__VA_ARGS__)
#define LOG_BUF_SIZE    2048
inline int dbg(const char *fmt,...)  
{  
		va_list ap;  
		char buf[LOG_BUF_SIZE]; 		 

		va_start(ap, fmt);	
		vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);	
		va_end(ap);  
		printf("%s",buf);
		return 0;
}
#else
#define print(...) ((void)0)
#endif

class RandBox
{
public:
  RandBox(int size, int min, int max):size_(size), min_(min), max_(max), dis_(max - min) {
    if(size_ > dis_) {
			print("size : %d must not larger than dis=(max-min): %d\n", size, dis_);
      assert(0);
    }
    bool *marks = new bool[size_];
    for(int i; i < size_; i ++) {
      marks[i] = false;
    }
    orig_ = new int[size];
		print("min: %d   max: %d  dis: %d\n",min_,max_,dis_);
		print("Initial rand vals:\n");
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
				print("create rand failed!\n");
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
  Node(int key)  : left_(nullptr), right_(nullptr), parent_(nullptr), key_(key){};
  ~Node() {
    left_ = nullptr;
    right_ = nullptr;
    parent_ = nullptr;
  };
  
	bool IsLeaf() {
	  bool leaf = true;
	  if(left_||right_){
	    leaf = false;
	  }
	  return leaf;
	}
	
  bool CopyKey(Node *src) {
    bool ret = false;
		key_ = src->key_;
		ret = true;
		return ret;
  }
	
  bool SwapKey(Node *src) {
		int tmp = key_;
		key_ = src->key_;
		src->key_ = tmp;
  	return true;
  }

	inline int Key() {
	  return key_;
	}
	
  Node *left_;
  Node *right_;
  Node *parent_;
private:
  int key_;
};

class Heap
{
public:
  Heap():root_(nullptr){};
  
  ~Heap(){
		if(root_) {
			_DeleteHeap(root_);
			root_ = nullptr;
		}
	};

  bool IsEmpty() {
    return (root_ == nullptr);
  }
  
  void Insert(int key) {
		print("INSERT ==> %d\n", key);
    if(!root_) {
		  root_ = new Node(key);
			print("insert %d on root\n", key);
    } else {
		  _Insert(key, root_);
		}
  }
  
  bool Delete(int key) {

		return true;
  }

  void TraverseInOrder() {
    print("In Order:\n");
    _InTravse(root_);
    cout << endl;
  }
  
  void TraversePreOrder() {
		print("Pre Order:\n");
    _PreTravse(root_);
    cout << endl;
  }
  
  void TraversePostOrder() {
    print("Post Order:\n");
    _PostTravse(root_);
    cout << endl;
  }
	
  void TraverseTopBottom() {
    if(!root_) {
      print("Empty Tree, Can't Traverse!");
			return ;
    }
    print("Top to bottom:\n");
    int node_cnt = 0;
    node_queue_.empty();
    node_queue_.push(root_);
    node_cnt ++;
    while(node_queue_.size()) {
      Node *cur = node_queue_.front();
      if(cur && cur->left_) {
        node_queue_.push(cur->left_);
			  node_cnt ++;
      }
      if(cur && cur->right_) {
        node_queue_.push(cur->right_);
				node_cnt ++;
      }
      print("%d ", cur->Key());
      node_queue_.pop();
    }
    print("Total: %d nodes\n",node_cnt);
  }

  Node *_GetInsertParent(Node *parent) {
    Node *insert_parent = nullptr;
    node_queue_.push(parent);
    while(!node_queue_.empty()) {
      Node *cur = node_queue_.front();
      node_queue_.pop();
      if(cur->left_) {
        node_queue_.push(cur->left_);
      }
      if(cur->right_) {
        node_queue_.push(cur->right_);
      }
      if(!(cur->left_)||!(cur->right_)) {
        insert_parent = cur;
      }
    }
    return insert_parent;
  }

  Node *_GetDeleteParent(Node *parent) {
    Node *del_parent = nullptr;
    Node *cur = nullptr;
    node_queue_.push(parent);
    while(!node_queue_.empty()) {
      cur = node_queue_.front();
      node_queue_.pop();
      if(cur->left_) {
        node_queue_.push(cur->left_);
      }
      if(cur->right_) {
        node_queue_.push(cur->right_);
      }
			if(cur->right_||cur->left_) {
				del_parent = cur;
			}
    }
    return del_parent;
  }
  
  int Height(void) {
    int height = 0;
    height = _Height(root_);
    return height;
  }
	
  bool Delete() {
    if(!root_) {
			print("Can't delete on a empty heap\n");
		  return false;
    }
		if(root_->IsLeaf()) {
			delete root_;
			root_ = nullptr;
		} else {
			_Delete(root_);
		}
		return true;
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
    return diff;
	}

	void _BubbleUp(Node *tail) {
	  if((tail->parent_)&&(tail->Key() > tail->parent_->Key())) {
	    tail->parent_->SwapKey(tail);
	    _BubbleUp(tail->parent_);
	  }
	}
	
	Node *_BubbleUp(int key, Node *parent) {
	  Node *ret = nullptr;
	  if(parent) {
	    if(parent->Key() == key){
	      ret = parent;
	    } else {
	      Node *left = _BubbleUp(key, parent->left_);
	      Node *right = _BubbleUp(key, parent->right_);
	      if(left) {
	        if(left->Key() > parent->Key()) {
	          parent->SwapKey(left);
	          ret = parent;
	        }
	      } else if(right) {
          if(left->Key() > parent->Key()) {
            parent->SwapKey(left);
	          ret = parent;
          }
	      }
	    }
	  }
	  return ret;
	}
	
  void _Insert(int key, Node *parent) {
    Node *insert_node = new Node(key);
    Node *insert_parent = _GetInsertParent(root_);
    if(insert_parent->left_) {
      insert_parent->right_ = insert_node;
    } else {
      insert_parent->left_ = insert_node;
    }
    insert_node->parent_ =  insert_parent;
    _BubbleUp(key, parent);
  }

#define BIGGER_NODE(child, parent) ((child)?(((child)->Key()<(parent)->Key())?child:parent):parent)
  void _BubbleDown(Node *parent) {
    Node *bigger = BIGGER_NODE(parent->right_, BIGGER_NODE(parent->left_, parent));
    if(bigger!=parent) {
      parent->SwapKey(bigger);
      _BubbleDown(bigger);
    }
  }

  void _Delete(Node *parent) {
    Node *del_parent = _GetDeleteParent(parent);
		if(del_parent->right_) {
		  del_parent->right_->SwapKey(parent);
			del_parent->right_ = nullptr;
		} else {
		  del_parent->left_->SwapKey(parent);
			del_parent->left_ = nullptr;
		}
    _BubbleDown(parent);
  }	

	
  void _DeleteHeap(Node *parent) {
    if(parent->left_) {
      _DeleteHeap(parent->left_);
    }
    if(parent->right_) {
      _DeleteHeap(parent->right_);
    }
    if(!(parent->left_)&&!(parent->right_)) {
      delete parent;
    }
  }
	
  void _InTravse(Node *parent) {
    if(parent) {
      _InTravse(parent->left_);
      _InTravse(parent->right_);
    }
  }

  void _PreTravse(Node *parent) {
    if(parent) {
      _PreTravse(parent->left_);
      _PreTravse(parent->right_);
    }
  }
  
  void _PostTravse(Node *parent) {
    if(parent) {
      _PostTravse(parent->left_);
      _PostTravse(parent->right_);
    }
  }

  Node *root_;
  queue<Node *> node_queue_;
};


const int NUMBER_NODE = 50;
const int MAX_VAL = 50;
const int MIN_VAL = 0;

class Heap_GTest : public ::testing::Test {

protected:
	Heap *heap = new Heap();
  RandBox *rbox = new RandBox(NUMBER_NODE, MIN_VAL, MAX_VAL);
  virtual void SetUp(){
    print("debug log is enable!\n");
  }
  
  virtual void TearDown(){
    delete heap;
  }
};
  

TEST_F(Heap_GTest, RandomNode_GTest){
  int val = 0;
  int insert_cnt = 0;
  bool ret = rbox->GetRand(&val);
  while(ret){
    heap->Insert(val);
    heap->TraverseTopBottom();
    ret = rbox->GetRand(&val);
    insert_cnt ++;
  }
  print("Total %d nodes inserted!\n", insert_cnt);
  rbox->Shake(0);
  ret = rbox->GetRand(&val);
  int del_cnt = 0;
  while(del_cnt<insert_cnt){
		print("Kill => <%d>\n",val);
    heap->Delete();
    heap->TraverseTopBottom();
    ret = rbox->GetRand(&val);
    del_cnt ++;
  }
  EXPECT_EQ(del_cnt, insert_cnt);
  EXPECT_TRUE(heap->IsEmpty());
}


int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

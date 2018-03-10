#include <assert.h>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

using std::cout;
using std::endl;

template <typename T>
class Heap
{
public:
  Heap(int heap_max_size)  : heap_max_size_(heap_max_size), node_num_(0) {
    array_ = new T[heap_max_size_];
  };
  
  ~Heap() {
    delete[] array_;
  };
  
  void Swap(T x, T y) {
    T tmp = array_[y];
    array_[y] = array_[x];
    array_[x] = tmp;
  }

  int GetParent(int child) {
    int parent = child;
    if(child) {
      if(child%2) {
        parent = (child - 1)/2;
      } else {
        parent = (child - 2)/2;
      }
    }
//    cout << child << " => " << parent << endl;
    return parent;
  }

  int GetChild(int parent) {
    int left = (parent*2 + 1);
    int right = (parent*2 + 2);
    if(node_num_ + 1 < left) {
      return parent;
    }
    else if(node_num_ + 1 == left) {
      return left;
    } else {
      return (array_[left] > array_[right]?left:right);
    }
  }

  int GetChildren(int parent, int *left, int *right) {
    int left_child = (parent*2 + 1);
    int right_child = (parent*2 + 2);
    if(left_child > node_num_) {
      return 0;
    } else if(right_child > node_num_) {
      *left = left_child;
      return 1;
    } else {
      *left = left_child;
      *right = right_child;
      return 2;
    }
  }
  
  void BubbleUp(int node) {
    int cur = node;
    int parent = node;
    parent = GetParent(cur);
    while(parent != cur) {
//      cout << array_[parent] << " vs " << array_[cur] << endl;
      if(array_[parent] < array_[cur]) {
//        cout << "swap!" << endl;
        Swap(cur, parent);
		cur = parent;
      } else {
        break;
      }
      parent = GetParent(cur);
    }
  }

  void BubbleDown(int node) {
    int cur = node;
    int left,right;
    int child_num = 0;
    int bigger_child = 0;
    child_num = GetChildren(cur, &left, &right);
//    cout << "child_num: " << child_num << " left:array_[" << left << "]:" << array_[left] << " right:array_[" << right << "]:" << array_[right] << endl;
    cout << "child_num: " << child_num  << endl;

    while(child_num) {
      if(child_num == 1) {
	    bigger_child = left;
      } else {
        bigger_child = array_[left] > array_[right]?left:right;
      }
	  if(array_[bigger_child] > array_[cur]) {
		Swap(bigger_child, cur);
		cur = bigger_child;
	  } else {
		break;
	  }
	  cout << "bigger child: " << array_[bigger_child] << endl;
	  child_num = GetChildren(cur, &left, &right);
    }
  }

  bool Insert(T key) {
    if(node_num_+1 == heap_max_size_) {
      cout << "heap full!" << endl;
      return false;
    }
	array_[node_num_] = key;
	node_num_ ++;
//	cout << "insert =>" << key << endl;
    BubbleUp(node_num_-1);
	return true;
  }
  
  T Slice() {
    T node(array_[0]);
    if(node_num_ != 0) {
      cout<< "array_[0]=>" <<  array_[0] << " array_[" << node_num_-1 << "]=>" << array_[node_num_-1] << endl;
      array_[0] = array_[node_num_-1];
      node_num_ --;
      BubbleDown(0);
    }
    cout << "node_num_: " << node_num_ << endl;
    return node;
  }
  
  void Traverse() {
    for(int i = 0; i< node_num_; i ++) {
      cout << array_[i] << " ";
    }
    cout << endl;
  }

  void Upheap(T key, int child) {
    if(child) {
      int parent = GetParent(child);
      if(array_[parent] < array_[child]) {
        Swap(child, parent);
        Upheap(key, parent);
      } 
    }
  }
  
  T Downheap(T key, int parent) {
    T node(0);
    int child = GetChild(parent);
    if(child != parent) {
      Swap(child, parent);
      Downheap(key, child);
    }
    return node;
  }
  
private:
  int heap_max_size_;
  int node_num_;
  T *array_;
};

class Heap_GTest : public ::testing::Test {

protected:
  Heap<int> *heap_ = new Heap<int>(100);
  int leafs_key_[5] = {10, 6, 11, 5, 13};
  int leafs_sorted_[6] = {20, 13, 11, 10, 6, 5};
  int leafs_num_ = 0;
  int leafs_sorted_num_ = 0;
  virtual void SetUp(){
    leafs_num_ = sizeof(leafs_key_)/sizeof(leafs_key_[0]);
    leafs_sorted_num_ = sizeof(leafs_sorted_)/sizeof(leafs_sorted_[0]);
    for(int i = 0; i < leafs_num_; i ++) {
      heap_->Insert(leafs_key_[i]);
    }
    heap_->Traverse();
  }
  
  virtual void TearDown(){
    delete heap_;
  }
};
//TEST_F(Heap_GTest, HeapInsert_GTest){
//  heap_->Insert(20);
//  heap_->Traverse();
//}
TEST_F(Heap_GTest, HeapSlice_GTest){
  heap_->Insert(20);
  heap_->Traverse();
//  heap_->Insert(1);
//  heap_->Traverse();
//  heap_->Insert(7);
//  heap_->Traverse();
  
  for(int i = 0; i < leafs_sorted_num_; i ++) {
    int node = heap_->Slice();
    EXPECT_EQ(leafs_sorted_[i], node);
    heap_->Traverse();
  }
  cout << endl;
}
int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


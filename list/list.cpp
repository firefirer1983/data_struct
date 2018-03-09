#include <assert.h>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

using std::cout;
using std::endl;

class List
{
public:
  List()  : prev_(nullptr), next_(nullptr) {};
  
  ~List() {
    if(prev_){
      prev_->next_ = next_;
    }
    if(next_){
      next_->prev_ = prev_;
    }
    prev_ = nullptr;
    next_ = nullptr;
  };
  
  void Append(List *node)  {
  	if(this->next_) {
  	  this->next_->prev_ = node;
	  node->next_ = this->next_;
	}
	node->prev_ = this;
	this->next_ = node;
  }
  
  void Prepend(List *node) {
    if(this->prev_) {
	  this->prev_->next_ = node;
      node->prev_ = this->prev_;
    }
    node->next_ = this;
    this->prev_ = node;
  }
  
  List *GetTail() {
    List *tail = this;
    while(tail->next_) {
      tail = tail->next_;
    }
    return tail;
  }

  List *GetHead() {
    List *head = this;
    while(head->prev_) {
      head = head->prev_;
    }
    return head;
  }

  int CountForward() {
    int length = 0;
    List *node = this->next_;
    while(node) {
      length ++;
      node = node->next_;
    }
    return length;
  }
  
  int CountBackward() {
    int length = 0;
    List *node = this->prev_;
    while(node) {
      length ++;
      node = node->prev_;
    }
    return length;
  }

  bool CheckComplete() {
    int forward_count = this->GetHead()->CountForward();
    int backward_count = this->GetTail()->CountBackward();
    cout << "forward: " << forward_count << " vs " << "backword: " << backward_count << endl;
    return (forward_count == backward_count);
  }
  
  List *prev_;
  List *next_;
};

class List_GTest : public ::testing::Test {

protected:
  List *list_head_;
  virtual void SetUp(){
    list_head_ = new List();
  }
  
  virtual void TearDown(){
    List *node = list_head_->next_;
    int delete_node_num = 0;
	while(node){
	  List *tmp = node->next_;
	  delete node;
	  delete_node_num ++;
	  node = tmp;
	}
	cout << "delete " << delete_node_num << " nodes in TearDown!" << endl;
  }
};

TEST_F(List_GTest, ListAppendPrepend_GTest){
  const int knode_num = 100;
  List *node = list_head_;
  for(int i = 0; i < knode_num; i ++) {
    List *tmp = new List;
    node->Append(tmp);
    node = tmp;
  }
  EXPECT_EQ(knode_num, list_head_->CountForward());
  EXPECT_TRUE(node->CheckComplete());
  
  for(int i = 0; i < knode_num; i++) {
    List *tmp = new List;
    node->Prepend(tmp);
    node = tmp;
  }
  EXPECT_EQ(2*knode_num, list_head_->CountForward());
  EXPECT_TRUE(node->CheckComplete());
}

TEST_F(List_GTest, ListSplice_GTest){
  const int kfirst_node_num = 100;
  const int ksecond_node_num = 50;
  
  List *list1 = list_head_;
  List *list2 = new List;
  List *node = list1;
  
  for(int i = 0; i < kfirst_node_num; i ++) {
    List *tmp = new List;
    node->Append(tmp);
    node = tmp;
  }
  EXPECT_TRUE(node->CheckComplete());
  
  node = list2;
  for(int i = 0; i < ksecond_node_num; i++) {
    List *tmp = new List;
    node->Append(tmp);
    node = tmp;
  }
  EXPECT_TRUE(node->CheckComplete());
  List *tail = list1->GetTail();
  EXPECT_EQ(kfirst_node_num, tail->CountBackward());

  List *head = list2->GetHead();
  EXPECT_EQ(ksecond_node_num, head->CountForward());
  
  tail->Append(head);

  EXPECT_EQ(kfirst_node_num + ksecond_node_num + 1, list1->CountForward());
  EXPECT_TRUE(list1->CheckComplete());

  EXPECT_EQ(kfirst_node_num + ksecond_node_num + 1, node->CountBackward());
  EXPECT_TRUE(node->CheckComplete());
  
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


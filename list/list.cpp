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
  
  ~List() : prev_(nullptr), next_(nullptr) {};
  
  void Append(List *node)  {
  	if(this->next_)
  	  this->next_->prev_ = node;
	node->next_ = this->next_;
	node->prev_ = this;
	this->next_ = node;
  }
  
  void Prepend(List *node) {
    if(this->prev_)
	  this->prev_->next_ = node;
    node->prev_ = this->prev_;
    node->next_ = this;
    this->prev_ = node;
  }

  bool Remove(List *node) {
    // this node is the node need to be removed
    if(this == node) {
	  if(this->prev_) {
	  	this->prev_->next_ = this->next_;
	  }
	  if(this->next_) {
	  	this->next_->prev_ = this->prev_;
	  }
    }

	// search backward
    List *search_node = this->prev_;
    while(search_node) {
	  if(search_node == node) {
		if(node->prev_)
	  	  node->prev_->next_ = node->next_;
		node->next_->prev_ = node->prev_;
		delete node;
		return true;
	  }
	  search_node = search_node->prev_;
	  if(search_node == nullptr) {
	  	break;
	  }
	}

	// search forward
	search_node = this->next_;
	while(search_node) {
	  if(search_node == node) {
	  	if(node->next_) {
		  node->next_->prev_ = node->prev_;
	  	}
		node->prev_->next_ = node->prev_;
		delete node;
		return true;
	  }
	  search_node = search_node->next_;
	  if(search_node == nullptr) {
	  	break;
	  }
	}
  }
  
private:
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
	if(node == nullptr) {
	  delete list_head_;
	  return;
	}
	
	while(node){
      delete node->prev_;
      node->prev_ = nullptr;
      node = node->next_;
	  if(node == nullptr) {
	  	delete node->prev_;
		break;
	  }
	}
  }
};

TEST_F(Stack_GTest, StackCreate_GTest){
  EXPECT_EQ(kstack_test_size_, stack_->GetStackMaxSize());
  int elem = 100;
  EXPECT_TRUE(stack_->Push(elem));
  EXPECT_EQ(elem, stack_->Pop());
}

TEST_F(Stack_GTest, StackPushPopPeek_GTest){
  int elem = 100;
  EXPECT_TRUE(stack_->Push(elem));
  EXPECT_EQ(elem, stack_->Pop());
  EXPECT_EQ(0, stack_->Peek(0));
  EXPECT_TRUE(stack_->Push(elem));
  cout << "peek res: " << stack_->Peek(0) << endl;
  EXPECT_EQ(elem, stack_->Peek(0));
  EXPECT_EQ(elem, stack_->Peek(0));
}

TEST_F(Stack_GTest, StackPushOverflow_GTest){
  int i = 0;
  for(i = 1; i <= stack_->GetStackMaxSize(); i ++) {
	  EXPECT_TRUE(stack_->Push(i));
  }
  EXPECT_FALSE(stack_->Push(7788));
  
  for(i = stack_->GetStackMaxSize(); i > 0; i --) {
	  EXPECT_EQ(i, stack_->Pop());
  }
  EXPECT_EQ(0, stack_->Pop());
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


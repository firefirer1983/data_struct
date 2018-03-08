#include <assert.h>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

using std::cout;
using std::endl;

template <typename T>
class Stack
{
public:
  Stack(int stack_max_size = 500) : stack_max_size_(stack_max_size){ 
    top_ = 0;
    elems_ = new T[stack_max_size_];
  };
  
  Stack(const Stack &stk) {
    stack_max_size_ = stk.GetStackMaxSize();
    elems_ = new T[stack_max_size_];
  };
  
  ~Stack() {
    delete [] elems_;
  };
  
  bool Push(const T &elem){
    if (top_ < stack_max_size_) {
      elems_[top_] = elem;
      top_ ++;
      cout << "Push <= " << elem << endl;
      return true;
    }
    return false;
  };
  
  T Pop(void){
	T elem(0);
    if(top_ > 0){
      top_ --;
      elem =  elems_[top_];
	  cout << "Pop => " << elem << endl;
    }
	return elem;
  };
  
  T Peek(int index){
    T elem(0);
    if(index < top_){
      elem =  elems_[index];
	  cout << "Peek => " << elem << endl;
    }else{
      cout << "Peek failed!" << endl;
    }
	return elem;
  };

  int GetStackMaxSize(void) {
    return stack_max_size_;
  }
  
private:
  T *elems_;
  int top_;
  int stack_max_size_;
};

class Stack_GTest : public ::testing::Test {

protected:
  Stack<int> *stack_;
  const int kstack_test_size_ = 500;
  virtual void SetUp(){
    stack_ = new Stack<int>(kstack_test_size_);
  }
  
  virtual void TearDown(){
    delete stack_;
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


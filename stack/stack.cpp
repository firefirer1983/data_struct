#include <assert>
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
  
  Stack(const Stack &stk) : {
    stack_max_size_ = stk.GetStackMaxSize();
    elems_ = new T[stack_max_size_];
  };
  
  ~Stack() {
    delete [] elems_;
  };
  
  bool Push(const T &elem){
    if (top_ < stack_max_size_ - 1) {
      elems_[top_] = elem;
      top_ ++;
      cou
      return true;
    }
    return false;
  };
  
  T Pop(void){
	T elem;
    if(top_ > 0){
      elem =  elems_[top_];
      top_ --;
    }
	return elem;
  };
  
  T Peek(void){
    T elem;
    if(top_ > 0){
      T elem =  elems_[top_];
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

int main(int argc, char *argv[])
{
  
}

#include <assert.h>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

using std::cout;
using std::endl;

template <typename T>
class Queue
{
public:
  Queue(int queue_max_size = 500) : queue_max_size_(queue_max_size), queue_all_size_(queue_max_size_+2){ 
    front_ = 0;
    end_ = 1;
    elems_ = new T[queue_all_size_];
  };
  
  Queue(const Queue &stk):   queue_max_size_(stk.GetQueueMaxSize()), queue_all_size_(queue_max_size_+2){
    elems_ = new T[queue_all_size_];
  };
  
  ~Queue() {
    delete [] elems_;
  };
  
  bool Enqueue(const T &elem){
    if((end_ + 1)%queue_all_size_ != front_){
      elems_[end_] = elem;
      end_ = end_ + 1;
      end_ = end_%queue_all_size_;
      cout << "Enqueue => elems_[" << end_ << "]: " << elem << endl;
      return true;
    }
    return false;
  };
  
  T Dequeue(void){
	T elem(0);
    if((front_ + 1)%queue_all_size_ != end_){
      front_ = front_ + 1;
      front_ = front_%queue_all_size_;
      elem = elems_[front_];
	  cout << "Dequeue => elems_[" << front_ << "]: " << elem << endl;
      return elem;
    }
	return elem;
  };
  
  T Peek(int index){
    T elem(0);
    if(end_ > front_ && index >= front_ && index < (end_-1)){
      elem = elems_[index];
    }else if(end_ < front_ && (index >= front_ || index < (end_-1))){
  	  elem = elems_[index];
    }
	return elem;
  };

  int GetQueueMaxSize(void) {
    return queue_max_size_;
  }
  
private:
  T *elems_;
  int queue_max_size_;
  int queue_all_size_;
  int front_;
  int end_;
};

class Queue_GTest : public ::testing::Test {

protected:
  Queue<int> *queue_;
  const int kqueue_test_size_ = 500;
  virtual void SetUp(){
//    cout << "SetUp <=" << endl;
    queue_ = new Queue<int>(kqueue_test_size_);
  }
  
  virtual void TearDown(){
//    cout << "TearDown =>" << endl;
    delete queue_;
  }
};

TEST_F(Queue_GTest, QueueCreate_GTest){
  EXPECT_EQ(kqueue_test_size_, queue_->GetQueueMaxSize());
  int elem = 100;
  EXPECT_TRUE(queue_->Enqueue(elem));
  EXPECT_EQ(elem, queue_->Dequeue());
}

TEST_F(Queue_GTest, QueueInOut_QTest){
  int elem = 100;
  EXPECT_TRUE(queue_->Enqueue(elem));
  EXPECT_EQ(elem, queue_->Dequeue());
  EXPECT_EQ(0, queue_->Peek(1));
  EXPECT_TRUE(queue_->Enqueue(elem));
//  cout << "peek res: " << queue_->Peek(1) << endl;
  EXPECT_EQ(elem, queue_->Peek(1));
  EXPECT_EQ(elem, queue_->Peek(1));
}

TEST_F(Queue_GTest, QueueOverflow_GTest){
  int i = 0;
  for(i = 1; i <= queue_->GetQueueMaxSize(); i ++) {
	  EXPECT_TRUE(queue_->Enqueue(i));
  }
  EXPECT_FALSE(queue_->Enqueue(7788));
  
  for(i = 1; i <= queue_->GetQueueMaxSize(); i ++) {
	  EXPECT_EQ(i, queue_->Dequeue());
  }
  EXPECT_EQ(0, queue_->Dequeue());
}

TEST_F(Queue_GTest, QueueSnake_GTest){
  int i = 0;
  for(i = 1; i <= queue_->GetQueueMaxSize()/2; i ++) {
	  EXPECT_TRUE(queue_->Enqueue(i));
  }  
  for(i = 1; i <= queue_->GetQueueMaxSize()/2; i ++) {
	  EXPECT_EQ(i, queue_->Dequeue());
  }
  for(i = 1; i <= queue_->GetQueueMaxSize()/3; i ++) {
	  EXPECT_TRUE(queue_->Enqueue(i));
  }  
  for(i = 1; i <= queue_->GetQueueMaxSize()/3; i ++) {
	  EXPECT_EQ(i, queue_->Dequeue());
  }
  
  for(i = 1; i <= queue_->GetQueueMaxSize()/2; i ++) {
	  EXPECT_TRUE(queue_->Enqueue(i));
  }  
  for(i = 1; i <= queue_->GetQueueMaxSize()/2; i ++) {
	  EXPECT_EQ(i, queue_->Dequeue());
  }
  
  for(i = 1; i <= queue_->GetQueueMaxSize()/3; i ++) {
	  EXPECT_TRUE(queue_->Enqueue(i));
  }  
  for(i = 1; i <= queue_->GetQueueMaxSize()/3; i ++) {
	  EXPECT_EQ(i, queue_->Dequeue());
  }
  EXPECT_EQ(0, queue_->Dequeue());
  cout << "All Test Pass!" << endl;
}


int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


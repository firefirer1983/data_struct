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

//#define DBG 1
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
					print("%d ",orig_[i]+min_);
					break;
        }
      }while(limit);
      
      if(!limit) {
				print("create rand failed!\n");
      }
    }
		print("\n");
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
		print("Shake seed: %d\n", seed);
		for(int i = 0; i < size_; i ++) {
		  int pos = rand_pos[i];
		  print("%d ",orig_[pos]);
		  rands_.push_back(orig_[pos]);
		}
		print("\n");
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


#define SWAP(x, y) \
do{\
int tmp = x;\
x = y;\
y = tmp;\
}while(0)

const int DIGIT_NUM = 10;
class Sort {
public:
	Sort(int *array, int size):array_(array), size_(size){
	};
	~Sort(){
	};

  void SortNum(){
	  _RedixSort(size_);
  };

int POW10[10] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
#define pow10(x) (POW10[x])

#define GET_REDIX(pos, val) ((val/(pow10(pos)))%10)  
  void _RedixSort(int size) {
    vector<int> buckets[DIGIT_NUM];
    for(int i=0; i<DIGIT_NUM; i++) {
      buckets[i].clear();
    }
    int max_num = 0;
    for(int i=0; i<size; i++) {
      if(array_[i] > max_num) {
        max_num = array_[i];
      }
    }
    print("Max num:%d\n", max_num);
    int max_digit_num = 1;
    int digit = max_num/10;
    while(digit) {
      max_digit_num ++;
      digit = digit/10;
    }
    print("Max num:%d => digit count:%d\n", max_num,max_digit_num);
    for(int i=0; i<max_digit_num; i++) {
      print("Start to sort digit:%d\n",i);
      for(int j=0; j<size; j++) {
        int bucket_idx = GET_REDIX(i,array_[j]);
        print("val:%d => bucket:%d\n",array_[j],bucket_idx);
        buckets[bucket_idx].push_back(array_[j]);
      }
      int num = 0;
			for(int bucket_idx=9; bucket_idx>=0; bucket_idx--) {
			  print("bucket[%d]: \n",bucket_idx);
			  vector<int>::iterator itor;
			  for(itor=buckets[bucket_idx].begin(); itor!=buckets[bucket_idx].end(); itor++) {
			    array_[num] = *itor;
			    print("array_[%d]:%d ",num,array_[num]);
			    num ++;
			  }
			  buckets[bucket_idx].clear();
			  print("\n");
			}
			print("%d digit sorted:\n",i);
			for(int n=0; n<size; n++) {
			  print("%d ", array_[n]);
			}
			print("\n");
    }
  }
	
private:
  int *array_;
	int size_;
	
};

const int NUMBER_TO_SORT = 10000;
const int MAX_VAL = 20000;
const int MIN_VAL = 0;
class Sort_GTest : public ::testing::Test {

protected:
	RandBox *rbox = nullptr;
	Sort *robot = nullptr;
	int *sort_array = nullptr;
	
  virtual void SetUp(){
    rbox = new RandBox(NUMBER_TO_SORT, MIN_VAL, MAX_VAL);
		int val = 0;
    bool ret = rbox->GetRand(&val);
    sort_array = new int[NUMBER_TO_SORT];
  	print("To sort:\n");
    for(int i = 0; ret; i ++) {
  	  sort_array[i] = val;
  		print("%d ", sort_array[i]);
  		ret = rbox->GetRand(&val);
    }
  	print("\n");
  	robot = new Sort(sort_array, NUMBER_TO_SORT);
  }
  
  virtual void TearDown(){
		print("\n");
		print("Sorted result:\n");
		for(int i = 0; i < NUMBER_TO_SORT; i ++) {
			print("%d ", sort_array[i]);
		}
		print("\n");
    delete rbox;
		delete robot;
		delete sort_array;
  }
};

TEST_F(Sort_GTest, SortInt_GTest){
	robot->SortNum();
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

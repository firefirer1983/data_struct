#include <assert.h>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

using std::cout;
using std::endl;

enum dir_t{
  DIR_LEFT,
  DIR_RIGHT,
  DIR_NUM,
};



#define NODE(dir, node) (node->branchs_[dir]) 
#define LEFT_NODE(node) (node->branchs_[DIR_LEFT])
#define RIGHT_NODE(node) (node->branchs_[DIR_RIGHT])

//class BSTNode;
//
//typedef struct _branch_t{
//  BSTNode *b_left;
//  BSTNode *b_right;
//}branch_t;
//
//union tree {
//  BSTNode *branchs[DIR_NUM];
//  branch_t branch;
//};

class BSTNode
{
public:
  BSTNode(int key)  : key_(key) {
    LEFT_NODE(this) = nullptr;
    RIGHT_NODE(this) = nullptr;
  };
  
  ~BSTNode() {
    LEFT_NODE(this) = nullptr;
    RIGHT_NODE(this) = nullptr;
  };
  int key_;
  BSTNode *branchs_[DIR_NUM];
};


class BST
{
public:
  BST(BSTNode *node)  : root_(node){};
  
  ~BST() {
    DeleteTree(root_);
  };
  
  void DeleteTree(BSTNode *root) {
    BSTNode *node = root;
    if(node) {
      DeleteTree(LEFT_NODE(node));
      DeleteTree(RIGHT_NODE(node));
      delete node;
    }
  }

  bool Delete(int key) {
    cout << "del =>" << key << endl;
    bool ret = false;
    if(root_) {
	  if(key == root_->key_) {
	  	ret = _Delete(key, nullptr, DIR_NUM);
	  } else if(key < root_->key_) {
	    ret = _Delete(key, root_, DIR_LEFT);
	  } else {
	    ret = _Delete(key, root_, DIR_RIGHT);
	  }
    }
	return ret;
  }

  BSTNode *_FindMinNode(BSTNode *node) {
    if(!LEFT_NODE(node)) {
	  return node;
    } else {
	  return _FindMinNode(LEFT_NODE(node)); 
    }
  }
  
  BSTNode *_FindMaxNode(BSTNode *node) {
    if(!RIGHT_NODE(node)) {
	  return node;
    } else {
	  return _FindMinNode(RIGHT_NODE(node)); 
    }
  }

  bool _Delete(int key, BSTNode *parent, dir_t dir) {
    BSTNode *cur = nullptr;
    if(parent) {
      cur = NODE(dir, parent);
    } else {
      cur = root_;
    }
	
	if(!cur) {
	  cout << "cur nullptr" << endl;
	  return false;
	}
	cout << key << " vs " << cur->key_ << endl;
	if(key == cur->key_) {
	  if(!LEFT_NODE(cur)&& !RIGHT_NODE(cur)) { // no left, no right child, just go delete
	    cout << "no left or right" << endl;
		cout << "delete->" << cur->key_ << endl;
	    delete cur;
	    if(parent) {
	      NODE(dir, parent) = nullptr;
	    } else {
	      root_ = nullptr;
	    }
		
	  } else if(LEFT_NODE(cur) && !RIGHT_NODE(cur)) { // only left child
	    cout << "only left child" << endl;
		if(parent) {
		  NODE(dir, parent) = LEFT_NODE(cur);
		}else {
		  root_ = LEFT_NODE(cur);
		}
		cout << "delete->" << cur->key_ << endl;
		delete cur;
	  } else if(!LEFT_NODE(cur) && RIGHT_NODE(cur)) { // only right child
        cout << "only right child" << endl;
		if(parent) {
		  NODE(dir, parent) = RIGHT_NODE(cur);
		} else {
		  root_ = RIGHT_NODE(cur);
		}
		cout << "delete->" << cur->key_ << endl;
		delete cur;
	  } else { // both left and right child
	    cout << "both left and right child!" << endl;
	    BSTNode *start = RIGHT_NODE(cur);
		BSTNode *min = _FindMinNode(start);
		cout << " min: " << min->key_ << " cur: " << cur->key_ << endl;
		cur->key_ = min->key_;
		if(min == start) {
		  RIGHT_NODE(cur) = RIGHT_NODE(min);
		} else {
	      _Delete(cur->key_, cur, DIR_RIGHT);
		}
	  }
	  return true;
	} else if(key < cur->key_) {
	  return _Delete(key, cur, DIR_LEFT);
	} else {
	  return _Delete(key, cur, DIR_RIGHT);
	}
  }
  
  void Insert(int key) {
    if(key < root_->key_) {
	  _Insert(key, root_, DIR_LEFT);
    } else {
	_Insert(key, root_, DIR_RIGHT);
    }
  }

  void _Insert(int key, BSTNode *parent, dir_t dir) {
    BSTNode *cur = NODE(dir, parent);
	if(!cur) {
	  NODE(dir, parent) = new BSTNode(key);
	} else {
	  if(key < cur->key_) {
	  	_Insert(key, cur, DIR_LEFT);
	  } else {
        _Insert(key, cur, DIR_RIGHT);
	  }
	}
  }

  BSTNode *Search(int key) {
    if(key == root_->key_) {
	  return root_;
    } else if(key < root_->key_) {
      return _Search(key, root_, DIR_LEFT);
    } else {
      return _Search(key, root_, DIR_RIGHT);
    }
  }
  
  BSTNode *_Search(int key, BSTNode *parent, dir_t dir) {
    BSTNode *target = nullptr;
	BSTNode *cur = NODE(dir, parent);
    if(cur) {
      if(cur->key_ == key) {
        target = cur;
      } else if(key < cur->key_) {
        target = _Search(key, cur, DIR_LEFT);
      } else {
        target = _Search(key, cur, DIR_RIGHT);
      }
    }
	return target;
  }

  void _Traverse(BSTNode *node) {
    if(node) {
	  cout << node->key_ << " ";
      _Traverse(LEFT_NODE(node));
      _Traverse(RIGHT_NODE(node));
    }
  }
  
  void Traverse() {
    _Traverse(root_);
	cout << endl;
  }

  
  BSTNode *root_;
};

class BST_GTest : public ::testing::Test {

protected:
  BST *tree_;
  int leafs_key_[15] = {55, 23, 91, 20, 25, 80, 120, 15, 21, 24, 28, 70, 85, 110, 150};
  virtual void SetUp(){
	  tree_ = new BST(new BSTNode(leafs_key_[0]));
	  for(unsigned int i = 1; i < (sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
		tree_->Insert(leafs_key_[i]);
	  }
	  tree_->Traverse();
  }
  
  virtual void TearDown(){
    delete tree_;
  }
};

TEST_F(BST_GTest, BSTInsert_GTest){
  tree_->Insert(8);
  tree_->Insert(89);
  tree_->Traverse();
}

TEST_F(BST_GTest, BSTSearch_GTest){

  for(unsigned i = 0; i < (sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
    EXPECT_TRUE(tree_->Search(leafs_key_[i]));
  }

  EXPECT_EQ(nullptr, tree_->Search(100));
  tree_->Insert(100);
  cout << endl;
  EXPECT_NE(nullptr, tree_->Search(100));
  EXPECT_EQ(true, tree_->Delete(100));
  EXPECT_EQ(nullptr, tree_->Search(100));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(110));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(91));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(20));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(55));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(21));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(24));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(28));
  tree_->Traverse();
  
  EXPECT_EQ(true, tree_->Delete(15));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(120));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(70));
  tree_->Traverse();
  
  EXPECT_EQ(true, tree_->Delete(150));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(23));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(25));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(80));
  tree_->Traverse();
  EXPECT_EQ(true, tree_->Delete(85));
  tree_->Traverse();
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


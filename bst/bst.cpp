#include <assert.h>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

using std::cout;
using std::endl;

class BSTNode
{
public:
  BSTNode(int key)  : key_(key), left_(nullptr), right_(nullptr) {};
  
  ~BSTNode() {
    left_ = nullptr;
    right_ = nullptr;
  };
  int key_;
  BSTNode *left_;
  BSTNode *right_;
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
      DeleteTree(node->left_);
      DeleteTree(node->right_);
      delete node;
    }
  }

  void CopyRightNode(BSTNode *dst, BSTNode *src) {
    dst->key_ = src->key_;
    if(src->right_) {
      dst->right_ = src->right_;
    }
  }
  
  void CopyNode(BSTNode *dst, BSTNode *src) {
    dst->left_ = src->left_;
    dst->right_ = src->right_;
    dst->key_ = src->key_;
  }
  
  bool Delete(int key) {
    bool ret = false;
    BSTNode *parent = root_;
    BSTNode *cur = parent;
    BSTNode *child = BubbleDownSearch(key, cur);
    while(1) {
      child = BubbleDownSearch(key, cur);
      if(child && child != cur) {
        parent = cur;
        cur = child;
      } else {
        break;
      }
    }
    if(child) {
      cout << "Go it: " << parent->key_ << ((key < parent->key_)?" <- ":" -> ") << key << endl;
      if(child->right_ && child->left_) {
    	BSTNode *parent = child;
    	BSTNode *min = child->right_;
    	while(1) {
    	  if(min->left_) {
    		parent = min;
    		min = min->left_;
    	  }
    	  else {
    		break;
    	  }
    	}
    	cout << "min: " << parent->key_ << " => " << min->key_ << endl;
		child->key_ = min->key_;
    	if(parent->right_ == min) {
    	  cout << "adjacent right node replacement!" << endl;
    	  child->right_ = min->right_;
    	} else {
    	  parent->left_ = nullptr;
    	}
    	delete min;
      } else if(child->right_) {
        cout << "only right tree" << endl;
    	CopyNode(child, child->right_);
    	delete child->right_;
      } else if(child->left_) {
        cout << "only left tree" << endl;
    	CopyNode(child, child->left_);
    	delete child->left_;
      } else {
        cout << "delete leafs without sub trees" <<endl;
        if(parent) {
          if(key < parent->key_) {
            parent->left_ = nullptr;
          } else {
            parent->right_ = nullptr;
          }
    	}
    	delete child;
      }
      ret = true;
    }
    return ret;  
  }

  BSTNode *BubbleDown(int key, BSTNode *parent) {
  	BSTNode *cur = parent;
    if(key < cur->key_) {
      if(cur->left_) {
        cur = cur->left_;
      }
    } else {
      if(cur->right_) {
        cur = cur->right_;
      }
    }
	return cur;
  }

  void Insert(int key, BSTNode *node) {
    BSTNode *cur = node;
    BSTNode *child = BubbleDown(key, node);
    while(child != cur) {
      cur = child;
      child = BubbleDown(key, cur);
    }
    
    if(key < child->key_) {
      child->left_ = new BSTNode(key);
      cout << child->key_ << " <- " << child->left_->key_ <<  " ";
    } else {
      child->right_ = new BSTNode(key);
      cout << child->key_ << " -> " << child->right_->key_ << " ";
    }
  }
  
  BSTNode *BubbleDownSearch(int key, BSTNode *parent) {
    BSTNode *cur = nullptr;
    if(key == parent->key_) {
      cur = parent;
    }else if(key < parent->key_) {
      if(parent->left_) {
        cur = parent->left_;
      }
    } else {
      if(parent->right_) {
        cur = parent->right_;
      }
    }
	return cur;
  }
  
  BSTNode *Search(int key) {
    BSTNode *cur = root_;
    BSTNode *child = BubbleDownSearch(key, cur);
    while(child && cur != child) { // child quals nullptr means can't find.  child not equal parent means not find yet! 
      cur = child;
      child = BubbleDownSearch(key, cur);
    }
    return child;
  }
  
  void InsertNode(int key, BSTNode *node) {
    if(key < node->key_) {
      if(node->left_) {
        InsertNode(key, node->left_);
        return ;
      }
	  node->left_ = new BSTNode(key);
	  cout << key << " < " << node->key_ << endl; 
    } else {
      if(node->right_) {
	    InsertNode(key, node->right_);
        return ;
      }
      node->right_ = new BSTNode(key);
	  cout << key << " > " << node->key_ << endl; 
    }
  }
  void Traverse() {
    TraverseNode(this->root_);
    cout << endl;
  }
  void TraverseNode(BSTNode *node) {
    if(node) {
      cout << node->key_ << " ";
      TraverseNode(node->left_);
      TraverseNode(node->right_);
    }
  }

  BSTNode *SearchNode(int key, BSTNode *node) {
    if(node){
      if(node->key_ == key) {
        cout << "Hit key: " << key << endl;
        return node;
      } else {
        BSTNode *res = SearchNode(key, node->left_); 
        if(res)
          return res;
        res = SearchNode(key, node->right_); 
        if(res)
          return res;
      }
    } 
    return nullptr;
  }
  
  BSTNode *root_;
};

class BST_GTest : public ::testing::Test {

protected:
  BST *tree_;
  //int leafs_key_[15] = {55, 23, 91, 20, 25, 80, 120, 15, 21, 24, 28, 70, 85, 110, 150};
	int leafs_key_[6] = {55, 23, 91, 20, 25, 15};
  virtual void SetUp(){
	  tree_ = new BST(new BSTNode(leafs_key_[0]));
	  for(unsigned int i = 1; i < (sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
		tree_->Insert(leafs_key_[i], tree_->root_);
	  }
	  cout << endl;
	  tree_->Traverse();
  }
  
  virtual void TearDown(){
    delete tree_;
  }
};

TEST_F(BST_GTest, BSTInsert_GTest){
  tree_->Insert(8, tree_->root_);
  cout << endl;
  tree_->Insert(89, tree_->root_);
  cout << endl;
  tree_->Traverse();
}

TEST_F(BST_GTest, BSTSearch_GTest){

  for(unsigned i = 0; i < (sizeof(leafs_key_)/sizeof(leafs_key_[0])); i ++) {
    EXPECT_TRUE(tree_->Search(leafs_key_[i]));
  }

  EXPECT_EQ(nullptr, tree_->Search(100));
  tree_->Insert(100, tree_->root_);
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
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


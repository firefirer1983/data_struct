#include <assert.h>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

using std::cout;
using std::endl;

class AVLNode
{
public:
  AVLNode(int key)  : key_(key), left_(nullptr), right_(nullptr) {};
  
  ~AVLNode() {
    left_ = nullptr;
    right_ = nullptr;
  };
  int key_;
  int height_;
  AVLNode *left_;
  AVLNode *right_;
  int GetBalance(void) {
  	
  }
};

class AVL
{
public:
  AVL(AVLNode *node)  : root_(node){};
  
  ~AVL() {
    DeleteTree(root_);
  };
  
  void DeleteTree(AVLNode *root) {
    AVLNode *node = root;
    if(node) {
      DeleteTree(node->left_);
      DeleteTree(node->right_);
      delete node;
    }
  }

  
  
  bool Delete(int key) {
    bool ret = false;
    AVLNode *parent = root_;
    AVLNode *cur = parent;
    AVLNode *child = BubbleDownSearch(key, cur);
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
    	AVLNode *parent = child;
    	AVLNode *min = child->right_;
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
		child->key_ = child->right_->key_;
		child->left_ = child->right_->left_;
		child->right_ = child->right_->right_;
    	delete child->right_;
      } else if(child->left_) {
        cout << "only left tree" << endl;
		child->key_ = child->right_->key_;
		child->left_ = child->left_->left_;
		child->right_ = child->left_->right_;
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

  AVLNode *BubbleDown(int key, AVLNode *parent) {
  	AVLNode *cur = parent;
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

  void Insert(int key, AVLNode *node) {
    AVLNode *cur = node;
    AVLNode *child = BubbleDown(key, node);
    while(child != cur) {
      cur = child;
      child = BubbleDown(key, cur);
    }
    
    if(key < child->key_) {
      child->left_ = new AVLNode(key);
      cout << child->key_ << " <- " << child->left_->key_ <<  " ";
    } else {
      child->right_ = new AVLNode(key);
      cout << child->key_ << " -> " << child->right_->key_ << " ";
    }
  }
  
  AVLNode *BubbleDownSearch(int key, AVLNode *parent) {
    AVLNode *cur = nullptr;
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
  
  AVLNode *Search(int key) {
    AVLNode *cur = root_;
    AVLNode *child = BubbleDownSearch(key, cur);
    while(child && cur != child) { // child quals nullptr means can't find.  child not equal parent means not find yet! 
      cur = child;
      child = BubbleDownSearch(key, cur);
    }
    return child;
  }
  
  void InsertNode(int key, AVLNode *node) {
    if(key < node->key_) {
      if(node->left_) {
        InsertNode(key, node->left_);
        return ;
      }
	  node->left_ = new AVLNode(key);
	  cout << key << " < " << node->key_ << endl; 
    } else {
      if(node->right_) {
	    InsertNode(key, node->right_);
        return ;
      }
      node->right_ = new AVLNode(key);
	  cout << key << " > " << node->key_ << endl; 
    }
  }
  void Traverse() {
    TraverseNode(this->root_);
    cout << endl;
  }
  void TraverseNode(AVLNode *node) {
    if(node) {
      cout << node->key_ << " ";
      TraverseNode(node->left_);
      TraverseNode(node->right_);
    }
  }

  AVLNode *SearchNode(int key, AVLNode *node) {
    if(node){
      if(node->key_ == key) {
        cout << "Hit key: " << key << endl;
        return node;
      } else {
        AVLNode *res = SearchNode(key, node->left_); 
        if(res)
          return res;
        res = SearchNode(key, node->right_); 
        if(res)
          return res;
      }
    } 
    return nullptr;
  }
  
  AVLNode *root_;
};

class AVL_GTest : public ::testing::Test {

protected:
  AVL *tree_;
  int leafs_key_[15] = {55, 23, 91, 20, 25, 80, 120, 15, 21, 24, 28, 70, 85, 110, 150};
  virtual void SetUp(){
	  tree_ = new AVL(new AVLNode(leafs_key_[0]));
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

TEST_F(AVL_GTest, AVLInsert_GTest){
  tree_->Insert(8, tree_->root_);
  cout << endl;
  tree_->Insert(89, tree_->root_);
  cout << endl;
  tree_->Traverse();
}

TEST_F(AVL_GTest, AVLSearch_GTest){

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


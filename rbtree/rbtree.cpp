#include <assert.h>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

using std::cin;
using std::cout;
using std::endl;

/*
	           (A)        red/black tree rules:
1. root is black
2. nil leaf is black
3. node is either black or red
4. no consecutive red node
5. all path from root to nil leaf have the same black depth

			   (B)        red/black tree insert rules
1. always insert red node

2. if insert root node, root=>black -> done

3. if parent(black) -> done

4. if parent(red)&&uncle(red)
   parent=>black and grandpa=>red, recheck grand parent, push black to bottom untill the root
   
5. if parent(red)&&uncle(black)
   5a: parent(left)&&node(left): 
  i.  rightRotate
  ii. recolor:  parent=>black, children=>red

   5b: parent(right)&&node(right):
  i.  leftRotate
  ii. recolor: parent=>black, children=>red
  
   5c: parent(left)&&node(right):
  i. leftRotate + rightRotate
  ii. recolor: parent=>black, children=>red

   5d: parent(right)&&node(left):
  i. rightRotate + leftRotate
  ii. recolor: parent=>black, children=>red   

  			(C) red/black tree delection rules:
Rules for the nodes to be delete:
  i. only got one child ( because it's the right most node in the left subtree or left most node in the right subtree)
 ii. left most node have no left sibling, right most node have no right sibling

 
Case 1 and Case 2 won't create double black node, So it's simple!

Case 1:
The node to be delete is RED:
  *Simply delete it
  *if node has a child, then replace it with the child

Case 2:
The node to be delete is BLACK, but has only one RED child:
  *Relace the node with its child, Recolor the RED child to BLACK


Case 3:
The node to be delete is BLACK and has one BLACK child
  *Replace the node with its child, and the child will become a DOUBLE BLACK node

Case 3-1: (Terminal)
  *DOUBLE BLACK node is root.
  -- Just change DOUBLE BLACK node to BLACK node

  Then Case 3-1 Terminated!
  
Case 3-2:
  *DOUBLE BLACK node has a BLACK parent and a right RED sibling
     a. Left rotate its parent
     b. Recolor BLACK parent -> RED 
     c. Recolor RED silbing -> BLACK
     
  Case 3-2 -> Case 3-4 

Case 3-3:
  *DOUBLE BLACK node has a BLACK parent, and a right BLACK sibling , and the right 
   BALCK sibling has two BLACK children
     a. change DOUBLE BLACK node to BLACK node
     b. change BLACK parent -> DOUBLE BLACK
     c. change BLACK sibling -> RED

   Case 3-3 -> Case 3-1
   Case 3-3 -> Case 3-5

Case 3-4: (Terminal)
  * DOUBLE BLACK node has a RED parent , BLACK sibling with 2 BLACK children
     a. Recolor the DOUBLE BLACK -> BLACK
     b. Recolor the RED parent -> BLACK
     c. Recolor the BLACK sibling -> RED

  Then Case 3-4 Terminated!
  
Case 3-5:
  * DOUBLE BLACK node has a BLACK sibling. BLACK sibling has a inner RED child
     a. Right rotate the BLACK sibling
     b. Recolor the BLACK sibling 0> RED
     c. Recolr BLACK sibling's RED child -> BLACK

Case 3-6:
  * DOUBLE BLACK node has a BLACK sibling . BLACK sibling has a outer RED child
     a. Left rotate its parent
     b. Recolor its parent -> BLACK
     c. Recolor DOUBLE BLACK node -> BLACK
     dm Recolor its sibing outer RED child -> BLACK
*/   

class RBNode
{
public:
  RBNode(int key)  : key_(key), black_(false), left_(nullptr), right_(nullptr){};
  
  ~RBNode() {
    left_ = nullptr;
    right_ = nullptr;
  };
  int key_;
  bool black_;
  RBNode *left_;
  RBNode *right_;
  
  void Red() {
    this->black_ = false;
  }
  
  bool IsRed() {
    return (this->black_ == false);
  }
  
  void Black() {
    this->black_ = true;
  }
  
  bool IsBlack() {
    return this->black_;
  }
  
  void ReColor() {
    this->Black();
    if(this->left_) {
      this->left_->Red();
    }
    if(this->right_) {
      this->right_->Red();
    }
  }

  bool RedConflict() {
    if(this->IsRed()) {
	  if((this->left_ && this->left_->IsRed()) || (this->right_ && this->right_->IsRed())) {
	  	return true;
	  }
    }
	return false;
  }
};
  

class RBTree
{
public:
  RBTree() :root_(nullptr){};
  
  ~RBTree() {
    Delete(root_);
  };

  void Delete(RBNode *node) {
    if(node){
      Delete(node->left_);
      Delete(node->right_);
      delete node;
    }
  }


  RBNode *Insert(int key, RBNode *parent, RBNode *uncle, RBNode *grandpa) {
    RBNode *root = nullptr;
	RBNode *inserted = nullptr;
    if(!parent) {
      root = new RBNode(key);
	  root->Black();
    } else if(key < parent->key_) {
      if(parent->left_) {
        parent->left_ = Insert(key, parent->left_, parent->right_, parent);
      } else {
        parent->left_ = new RBNode(key);
        cout<< "[" << key << "]" << "<-"<< "[" << parent->key_ << "]" <<  endl;
      }
	  inserted = parent->left_;
    } else {
      if(parent->right_) {
        parent->right_ = Insert(key, parent->right_, parent->left_, parent);
      } else {
        parent->right_ = new RBNode(key);
        cout<<"[" << parent->key_ <<"]" << "->" << "[" << key << "]" << endl;
      }
	  inserted = parent->right_;
    }
    
    if(parent) {
	  if(parent->RedConflict()) { // red confict
	    printf("parent[%d] child[%d] red confict\n", parent->key_, inserted->key_);
	  	if(uncle&&uncle->IsRed()) { // red uncle
	  	  cout << "Red uncle => Push black to bottom" << endl;
	  	  parent->Black();
	  	  uncle->Black();
	  	  if(grandpa) {
	  	    grandpa->Red();
			printf("[color-flip] parent[%d]=>black uncle[%d]=>black grandpa[%d]=>red\n",parent->key_,uncle->key_,grandpa->key_);
	  	  }
	  	  root = parent;
	  	} else {
		  cout << "Black uncle => Do rotate" << endl;
	  	}
	  } else {
	  	root = parent;
	  }
	  
      if((parent->left_&&parent->left_->RedConflict())||(parent->right_&&parent->right_->RedConflict())) {
	    if(parent->left_&&parent->left_->RedConflict()) {
		  printf("child[%d] red conflit!\n",parent->left_->key_);
	    }
	    if(parent->right_&&parent->right_->RedConflict()) {
		  printf("child[%d] red conflit!\n",parent->right_->key_);
	    }
        if(_Diff(parent) > 1) {
          cout << "Unbalance: " << _Height(root->left_) << " << [" <<root->key_ << "] >> " << _Height(root->right_) << " -> diff: " << _Diff(root) << endl;
          if(_Diff(parent->left_) > 0) {
            root = _RightRotate(parent);
          } else {
            root = _LRRotate(parent);
          }
          root->ReColor();
          cout << _Height(root->left_) << " << [" <<root->key_ << "] >> " << _Height(root->right_) << " -> diff: " << _Diff(root) << endl;
        } else if(_Diff(parent) < -1) {
          cout << "Unbalance: " << _Height(root->left_) << " << [" <<root->key_ << "] >> " << _Height(root->right_) << " -> diff: " << _Diff(root) << endl;
          if(_Diff(parent->right_) < 0) {
          root = _LeftRotate(parent);
          } else {
          root = _RLRotate(parent);
          }
          root->ReColor();
          cout << _Height(root->left_) << " << [" <<root->key_ << "] >> " << _Height(root->right_) << " -> diff: " << _Diff(root) << endl;
        } else {
          root = parent;
        }
      }else {
	    root = parent;
      }
	}
	return root;
  }

  int _Diff(RBNode *parent) {
    if(parent) {
      int right = _Height(parent->right_);
      int left = _Height(parent->left_);
      int diff = left - right;
  //    cout << parent->key_ << ": " << left << " vs " << right << " diff: " << diff << endl;
      return diff;
    } else {
	  return 0;
    }
  }
  
  RBNode *_LeftRotate(RBNode *node) { // right rotation once!
    cout << "_LeftRotate["<<node->key_<<"]" << endl;
    RBNode *root = node->right_;
    node->right_ = root->left_;
    root->left_ = node;
    return root;
  }
  
  RBNode *_RightRotate(RBNode *node) { // right rotation once!
    cout << "_RightRotate["<<node->key_<<"]" << endl;
    RBNode *root = node->left_;
    node->left_ = root->right_;
    root->right_ = node;
    return root;
  }

  
  RBNode *_LRRotate(RBNode *node) { // right rotation once!
    cout << "_LRRotate["<<node->key_<<"]" << endl;
	node->left_ = _LeftRotate(node->left_);
    return _RightRotate(node);
	
  }
  
  RBNode *_RLRotate(RBNode *node) { // right rotation once!
    cout << "_RLRotate["<<node->key_<<"]" << endl;
	node->right_ = _RightRotate(node->right_);
    return _LeftRotate(node);
  }

  void Traverse() {
    _Traverse(root_);
    cout << endl;
  }
  
  void _Traverse(RBNode *node) {
    if(node) {
      cout << node->key_ << " " << " h:" << _Height(node) << " color: " << (node->black_?"Black":"Red")<< endl;
      _Traverse(node->left_);
      _Traverse(node->right_);
    }
  }
  
  int _MAX(int x, int y) {
    return (x>y?x:y);
  }
  
  int _MIN(int x, int y) {
    return (x>y?y:x);
  }

  int _Height(RBNode *node) {
    int left_height = 0;
    int right_height = 0;
    int height = 0;
    if(!node) {
      height = 0;
    } else {
      left_height = _Height(node->left_);
      right_height = _Height(node->right_);
      height = _MAX(left_height, right_height) + 1;
    }
    return height;
  }
  RBNode *root_;
};

int main(int argc, char *argv[])
{
  RBTree *tree_ = new RBTree();
  int input_key = 0;
  do {
    cout << "Please input value to add to the tree: " << endl;
    cin >> input_key;
    tree_->root_ = tree_->Insert(input_key, tree_->root_, nullptr, nullptr);
    if(tree_->root_->IsRed())
      tree_->root_->Black();
    tree_->Traverse(); 
  }while(1);
}


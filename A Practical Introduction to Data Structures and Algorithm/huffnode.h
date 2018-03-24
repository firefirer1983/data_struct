// From the software distribution accompanying the textbook
// "A Practical Introduction to Data Structures and Algorithm Analysis,
// Third Edition (C++)" by Clifford A. Shaffer.
// Source code Copyright (C) 2007-2011 by Clifford A. Shaffer.

// Huffman tree node abstract base class
template <typename E> class HuffNode {
public:
  virtual ~HuffNode() {}                // Base destructor
  virtual int weight() = 0;             // Return frequency
  virtual bool isLeaf() = 0;            // Determine type
  virtual HuffNode* left() const = 0;   // Get left
  virtual void setLeft(HuffNode*) = 0;  // Set left
  virtual HuffNode* right() const = 0;  // Get right
  virtual void setRight(HuffNode*) = 0; // Set right
};

template <typename E>   // Leaf node subclass
class LeafNode : public HuffNode<E> {
private:
  KVpair<int,E>* it;         // Frequency key-value pair
public:
  LeafNode(int freq, const E& val)   // Constructor
    { it = new KVpair<int,E>(freq, val); }
  int weight() { return it->key(); }
  KVpair<int,E>* val() { return it; }
  bool isLeaf() { return true; }
  virtual HuffNode<E>* left() const { return NULL; }
  virtual void setLeft(HuffNode<E>*) { }
  virtual HuffNode<E>* right() const { return NULL; }
  virtual void setRight(HuffNode<E>*) { }
};

template <typename E>   // Internal node subclass
class IntlNode : public HuffNode<E> {
private:
  HuffNode<E>* lc;   // Left child
  HuffNode<E>* rc;   // Right child
  int wgt;              // Subtree weight
public:
  IntlNode(HuffNode<E>* l, HuffNode<E>* r)
    { wgt = l->weight() + r->weight(); lc = l; rc = r; }
  int weight() { return wgt; }
  bool isLeaf() { return false; }
  HuffNode<E>* left() const { return lc; }
  void setLeft(HuffNode<E>* b)
    { lc = (HuffNode<E>*)b; }
  HuffNode<E>* right() const { return rc; }
  void setRight(HuffNode<E>* b)
    { rc = (HuffNode<E>*)b; }
};

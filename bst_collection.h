//----------------------------------------------------------------------
// FILE: bst_collection
// NAME: Scott Tornquist
// DATE: 11/10/2020
// DESC: Implements a Binary Search Tree version of the collection
//       class. Includes all member function inherited from collection
//       as well as the constructor, destructor, copy constructor, and
//       assignment operator and height function
//----------------------------------------------------------------------


#ifndef BST_COLLECTION_H
#define BST_COLLECTION_H

#include "array_list.h"
#include "collection.h"
#include <functional>

template<typename K, typename V>
class BSTCollection : public Collection<K,V>
{
public:
  // add a new key-value pair into the collection 
    void add(const K& a_key, const V& a_val);

  // remove a key-value pair from the collection
    void remove(const K& a_key);

  // find and return the value associated with the key
  // if key isn't found, returns false, otherwise true
    bool find(const K& search_key, V& the_val) const;

  // find and return each key >= k1 and <= k2 
    void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
  
  // return all of the keys in the collection 
    void keys(ArrayList<K>& all_keys) const;

  // return all of the keys in ascending (sorted) order
    void sort(ArrayList<K>& all_keys_sorted) const;

  // return the number of key-value pairs in the collection
    size_t size() const;

    // constructor
    BSTCollection();

    //destructor
    ~BSTCollection();

    //copy constructor
    BSTCollection(const BSTCollection <K,V>& rhs);

    // assignment operator
    BSTCollection& operator=(const BSTCollection<K,V>& rhs);

    //tree height function
    size_t height() const;



//private:

    //tree node
    struct Node{
        K key;
        V value;
        Node* left;
        Node* right;
    };

    // root node of the tree
    Node* root;

    //number of kv-pairs stored in the collection
    size_t node_count;

    //remove all elements in the bst
    void make_empty(Node* subtree_root);

    //copy helper 
    void copy (Node* lhs_subtree_root, const Node* rhs_subtree_root);

    // remove helper
    Node* remove(Node* subtree_root, const K& a_key);

    //helper function to recursively build up key list
    void find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys)const;

    //helper to recursivley build sorted list of keys
    void keys(const Node* subtree_root, ArrayList<K>& all_keys)const;

    //helper to recursively find height of the tree
    size_t height(const Node* subtree_root)const;

};

template<typename K, typename V>
BSTCollection<K,V>::BSTCollection(){ //constructor
    node_count=0;
    root = nullptr;
};

    //destructor
template<typename K, typename V>
BSTCollection<K,V>::~BSTCollection(){ //destructor
  if(size() > 1){
    make_empty(root);
  }
  else if(size() ==1){
    delete root;
  }
  node_count=0;
};

    //copy constructor
template<typename K, typename V>
BSTCollection<K,V>::BSTCollection(const BSTCollection <K,V>& rhs){
    root = nullptr;
    *this = rhs; // defers to assignment operator
};

    // assignment operator
template<typename K, typename V>
BSTCollection<K,V>& BSTCollection<K,V>::operator=(const BSTCollection<K,V>& rhs){
if(this != &rhs){ // tree1 != tree1
  if(root != nullptr){
    make_empty(root);
  }
  if(rhs.size() >0){
  node_count = 1;
  root = new Node; //set root to something
  root->left = nullptr;
  root->right = nullptr;
  copy(root, rhs.root);
  }
  }
  //return lhs(this)
  return *this;
};

template<typename K, typename V>
size_t BSTCollection<K,V>::height()const{
  if(size() > 0){
    return height(root);
  }
  return 0;

};

template<typename K, typename V>
void BSTCollection<K,V>::make_empty(Node* subtree_root){
  if(subtree_root != nullptr){
    make_empty(subtree_root->left);
    make_empty(subtree_root->right);
    delete subtree_root;
    node_count--;
  }
};

template<typename K, typename V>
void BSTCollection<K,V>::copy (Node* lhs_subtree_root, const Node* rhs_subtree_root){
  if(rhs_subtree_root == nullptr){
    return;
  }
  else {
    lhs_subtree_root->key = rhs_subtree_root->key;
    lhs_subtree_root->value = rhs_subtree_root->value;
    lhs_subtree_root->right = nullptr;
    lhs_subtree_root->left = nullptr;
    if(rhs_subtree_root->right != nullptr){
      lhs_subtree_root->right = new Node;
      node_count++;
    }
    if(rhs_subtree_root->left != nullptr){
      lhs_subtree_root->left = new Node;
      node_count++;
    }
    if(rhs_subtree_root->left != nullptr){
    copy(lhs_subtree_root->left, rhs_subtree_root->left); // traverse left on rhs
    }
    if(rhs_subtree_root->right != nullptr){
    copy(lhs_subtree_root->right, rhs_subtree_root->right); // traverse right on rhs
    }
  }
};

template<typename K, typename V>
typename BSTCollection<K,V>::Node*
BSTCollection<K,V>::remove(Node* subtree_root, const K& a_key){
  if(size() >0){
  if(subtree_root != nullptr && subtree_root->key > a_key){ // if greater than go left
    subtree_root->left = remove(subtree_root->left, a_key);
  }
  else if(subtree_root != nullptr && subtree_root->key < a_key){ // if less than go right
    subtree_root->right = remove(subtree_root->right, a_key);
  }
  else if(subtree_root != nullptr && subtree_root->key == a_key){
    Node* curr =nullptr;
    Node* prev = nullptr;
    if(subtree_root->left == nullptr){
      if(subtree_root->right == nullptr){ // no children case: leaf node
        if(root == subtree_root){
          root == nullptr;
        }
        delete subtree_root;
        node_count--;
        return nullptr;
      }
      else{// one right child case 
        if(root == subtree_root){
          root = subtree_root->right;
        }
        curr = subtree_root->right;
        delete subtree_root;
        node_count--;
        return curr;
      }
    }
    else{
      if(subtree_root->right == nullptr){ // one leftchild case
        if(root == subtree_root){
          root = subtree_root->left;
        }
        curr = subtree_root->left;
        delete subtree_root;
        node_count--;
        return curr;
      }
      else{// two children case
        curr = subtree_root;
        curr = curr->right;
        if(curr->left == nullptr){ //we are in the special case
          subtree_root->right = curr->right; //skipping over right node
          subtree_root->value = curr->value;// copying values and keys
          subtree_root->key = curr->key;
          delete curr; //delete or return
          node_count--;
        }
        else{
          while(curr->left != nullptr){ // finding in-order successor
            prev = curr;
            curr = curr->left;
          }
          subtree_root->value = curr->value;
          subtree_root->key = curr->key;
          prev->left = curr->right;
          delete curr;
          node_count--;
        }
      }
    }
  }
  }
  return subtree_root;
};

template<typename K, typename V>
void BSTCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys)const{
  if(subtree_root == nullptr){ // if at nullptr return
    return;
  }
  else if(subtree_root->key < k1){ // if current key is greater than range go right
    find(subtree_root->right,k1,k2,keys);
  }
  else if(subtree_root->key > k2){ // if current key is less than range go left
    find(subtree_root->left,k1,k2,keys);
  }
  else { // if current key is in the range add to keys and go left and right
    keys.add(subtree_root->key);
    find(subtree_root->left,k1,k2,keys);
    find(subtree_root->right,k1,k2,keys);
  }
  return;
};

template<typename K, typename V>
void BSTCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys)const{
  if(subtree_root == nullptr){
    return;
  }
  keys(subtree_root->left,all_keys);

  all_keys.add(subtree_root->key);

  keys(subtree_root->right, all_keys);
    
};

template<typename K, typename V>
size_t BSTCollection<K,V>::height(const Node* subtree_root)const{
  size_t h =1;
  if(subtree_root->right != nullptr){ //checking right path
    if(subtree_root->left != nullptr){// two paths
      h = 1 + height(subtree_root->left);
      h = 1 + height(subtree_root->right);
    }
    else{ //one path to the right
      h = 1 + height(subtree_root->right);
    }
  }
  else{//one path to the left
    if(subtree_root->left != nullptr){
      h = 1 + height(subtree_root->left);
    }
    else{ // left and right are nullptr so we are at leaf node
      return h;
    }
  }
    return h;
};


template<typename K, typename V>
void BSTCollection<K,V>:: add(const K& a_key, const V& a_val){
  Node* ptr = new Node;
  ptr->left = nullptr;
  ptr->right = nullptr;
  ptr->key = a_key;
  ptr->value = a_val;
  if(node_count ==0){
    root = ptr;
    node_count++;
  }
  else{
    Node* itr =root;
    while(itr != nullptr){
      if(itr->key == a_key){
        itr->value = a_val;
        break;
      }
      else if(itr->key > a_key){ //if current key is larger go left
        if(itr->left == nullptr){ //if we cant go left
          itr->left = ptr; // add node
          node_count++;
          break;
        }
        else{// otherwise go left
          itr = itr-> left;
        }
      }
      else{ // current key must be small than our key
        if(itr->right == nullptr){ //if we cant go right
          itr->right = ptr; // add node
          node_count++;
          break;
        }
        else{
          itr = itr->right; // go right
        }
      }
    }
  }
  ptr = nullptr;

};



template<typename K, typename V>
void BSTCollection<K,V>:: remove(const K& a_key){
  if((size() > 0)){ //there are keys in the table
    remove(root,a_key);
  }
  
};

template<typename K, typename V>
bool BSTCollection<K,V>:: find(const K& search_key, V& the_val) const{
  if(node_count > 0){
    Node* itr =root;
    while(itr != nullptr){
      if(itr->key == search_key){
        the_val = itr->value;
        return true;
      }
      else if(itr->key > search_key){ //if current key is larger go left
        if(itr->left == nullptr){ //if we cant go left
          return false; // it is not in the list
        }
        else{// otherwise go left
          itr = itr-> left;
        }
      }
      else{ // current key must be small than our key
        if(itr->right == nullptr){ //if we cant go right
          return false; // it is not in the list
        }
        else{
          itr = itr->right; // go right
        }
      }
    }
  }
  return false; // if not found return false
 };

template<typename K, typename V>
void BSTCollection<K,V>:: find(const K& k1, const K& k2, ArrayList<K>& keys) const{ 
    if((k2 >= k1) && (size() > 0)){ // if we have a positive range and a postive size
        while(keys.size()>0){// make return array empty
            keys.remove(0);
        }
          find(root, k1, k2, keys);
        }         
 };

template<typename K, typename V>
void BSTCollection<K,V>:: keys(ArrayList<K>& all_keys) const{
if(size() > 0){
    while(all_keys.size()>0){
        all_keys.remove(0);
    }
    keys(root, all_keys);
  }
 };

template<typename K, typename V>
void BSTCollection<K,V>:: sort(ArrayList<K>& all_keys_sorted) const{ 
    keys(all_keys_sorted); //we can simply call keys because it is and inorder traversal
 };

template<typename K, typename V>
size_t BSTCollection<K,V>:: size() const{
    return node_count;
};


#endif
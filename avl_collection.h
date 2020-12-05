//----------------------------------------------------------------------
// FILE: avl_collection
// NAME: Scott Tornquist
// DATE: 11/10/2020
// DESC: Implements a AVL Tree version of the collection
//       class. Includes all member function inherited from collection
//       as well as the constructor, destructor, copy constructor, and
//       assignment operator and height function. This tree data structure
//       uses roatations to keep the tree balanced and efficient
//----------------------------------------------------------------------


#ifndef AVL_COLLECTION_H
#define AVL_COLLECTION_H

#include "array_list.h"
#include "collection.h"
#include <functional>

template<typename K, typename V>
class AVLCollection : public Collection<K,V>
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
    AVLCollection();

    //destructor
    ~AVLCollection();

    //copy constructor
    AVLCollection(const AVLCollection <K,V>& rhs);

    // assignment operator
    AVLCollection& operator=(const AVLCollection<K,V>& rhs);

    //tree height function
    size_t height() const;



//private:

    //tree node
    struct Node{
        K key;
        V value;
        int height;
        Node* left;
        Node* right;
    };

    // root node of the tree
    Node* root;

    //number of kv-pairs stored in the collection
    size_t node_count;

    //remove all elements in the AVL
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

    //new to hw 8

    // add helper

    Node* add(Node* subtree_root, const K& key, const V& val);

    //rotate right helper 
    Node* rotate_right(Node* k2);

    // rotate left helper
    Node* rotate_left(Node* k2);

    //rebalance the subtree rooted at subree_root
    Node* rebalance(Node* subtree_root);

    //helper for rebalance to make sure heights are correct
    Node* updateheight(Node* subtree_root);

    // for testing
    void print_tree(std::string indent, Node* subtree_root);

};

template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::rotate_right(Node* k2){ //simple right rotation
  Node* k1 = k2->left;
  k2->left = k1->right;
  k1->right = k2;
  return k1; 
};

template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::rotate_left(Node* k2){ //simple left rotation
  Node* k1 = k2->right;
  k2->right = k1->left;
  k1->left = k2;
  return k1;
};

template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::rebalance(Node* subtree_root){
    if(subtree_root == nullptr){
      return subtree_root;
    }
    Node* lptr = subtree_root->left;
    Node* rptr = subtree_root->right;

    //left but no right pointer(special case)
    if(lptr != nullptr && rptr == nullptr && lptr->height >1){
      if(lptr->right != nullptr && lptr->left == nullptr ){//checking for double rotation
        subtree_root->left = rotate_left(lptr);
        lptr = subtree_root->left;
        lptr->left->height = 1;// updating height
        lptr->height = lptr->left->height+1;
      }
      subtree_root = rotate_right(subtree_root);
      subtree_root->height = subtree_root->left->height +1; //updating height
      subtree_root->right->height = 1;// we know that if C is null, be must be null as well to be in this case
      //subtree_root->right->height = subtree_root->right->right->height +1; // if rptr not null
      root = subtree_root;
    }
    else if(rptr != nullptr && lptr == nullptr && rptr->height >1 ){//right but no left pointer (special case)
      if(rptr->left != nullptr && rptr->right == nullptr ){//checking for double rotation
        subtree_root->right = rotate_right(rptr);
        rptr = subtree_root->right;
        rptr->right->height = 1;//updating height
        rptr->height = rptr->right->height+1;// updating height
      }
      subtree_root = rotate_left(subtree_root);
      subtree_root->height = subtree_root->right->height +1; //updating height
      subtree_root->left->height = 1;
      root = subtree_root;
    }
    else if(rptr != nullptr && lptr != nullptr && lptr->height > rptr->height+1){ // left heavy: left right and left left
      if((lptr->left == nullptr && lptr->right != nullptr) || lptr->right != nullptr && lptr->left != nullptr && lptr->right->height > lptr->left->height ){//checking for double rotation with 2 nodes
        subtree_root->left = rotate_left(lptr); // left right
        if(lptr->left != nullptr){ //updating lptr and handling special case of only having right node
          lptr->height = lptr->left->height+1; // left and maybe right child
        }
        else if( lptr->right != nullptr){ // only right child
          lptr->height = lptr->right->height+1;
        }
        else{ // no children
          lptr->height =1;
        }
        lptr = subtree_root->left; // updating lptr
        lptr->height = lptr->left->height+1;
      } //left left case
      subtree_root = rotate_right(subtree_root);
      subtree_root->height = subtree_root->left->height +1; //updating height
      subtree_root->right->height = subtree_root->right->right->height +1; // if rptr not null
    }
    else if(lptr != nullptr && rptr != nullptr && rptr->height > lptr->height+1){ // right heavy
      if((rptr->right == nullptr && rptr->left != nullptr) || rptr->left != nullptr && rptr->right != nullptr && rptr->left->height > rptr->right->height ){//checking for double rotation with 2 nodes
        subtree_root->right = rotate_right(rptr);//right left case
        if(rptr->right != nullptr){ //right and makybe left
          rptr->height = rptr->right->height+1;
        }
        else if( rptr->left != nullptr){ // only left
          rptr->height = rptr->left->height+1;
        }
        else{ //none
          rptr->height =1;
        }
        rptr = subtree_root->right; // updating rptr
        rptr->height = rptr->right->height+1;
      }
      subtree_root = rotate_left(subtree_root);
      subtree_root->height = subtree_root->right->height +1; //updating height
      subtree_root->left->height = subtree_root->left->left->height +1; // if rptr not null
    }
    return subtree_root;
};


template<typename K, typename V>
void AVLCollection<K,V>:: print_tree(std::string indent, Node* subtree_root){ // degubb helper
    if(!subtree_root){
        return;
    }
    std::cout << indent << subtree_root->key << " (h="
              << subtree_root->height << ")" << std::endl;
    print_tree(indent + "  ", subtree_root->left);
    print_tree(indent + "  ", subtree_root->right);
}


template<typename K, typename V>
AVLCollection<K,V>::AVLCollection(){ //constructor
    node_count=0;
    root = nullptr;
};

    //destructor
template<typename K, typename V>
AVLCollection<K,V>::~AVLCollection(){ //destructor
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
AVLCollection<K,V>::AVLCollection(const AVLCollection <K,V>& rhs){
    root = nullptr;
    *this = rhs; // defers to assignment operator
};

    // assignment operator
template<typename K, typename V>
AVLCollection<K,V>& AVLCollection<K,V>::operator=(const AVLCollection<K,V>& rhs){
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
size_t AVLCollection<K,V>::height()const{
  if(root != nullptr){
    return root->height;
  }
  return 0;

};

template<typename K, typename V>
void AVLCollection<K,V>::make_empty(Node* subtree_root){
  if(subtree_root != nullptr){
    make_empty(subtree_root->left);
    make_empty(subtree_root->right);
    delete subtree_root;
    node_count--;
  }
};

template<typename K, typename V>
void AVLCollection<K,V>::copy (Node* lhs_subtree_root, const Node* rhs_subtree_root){ //copy contructor helper
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
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::updateheight(Node* subtree_root){ // helper funtion for removal to make sure heights are correct before rebalancing
  if(subtree_root == nullptr){
    return nullptr;
  }
  subtree_root->left = updateheight(subtree_root->left);
  subtree_root->right = updateheight(subtree_root->right);
  // backtracking: update heights
  if(subtree_root->left != nullptr && subtree_root->right != nullptr){ // if it has 2 children
          if( subtree_root->left->height >= subtree_root->right->height){
            subtree_root->height = subtree_root->left->height +1;
          }
          else{
            subtree_root->height = subtree_root->right->height +1;
          }
  }
  if(subtree_root->right != nullptr && subtree_root->left == nullptr){ // one right child
    subtree_root->height = subtree_root->right->height +1;
  }
  else if(subtree_root->left != nullptr && subtree_root->right == nullptr){ // one left child
  subtree_root->height = subtree_root->left->height +1;
  }
  else if(subtree_root->left == nullptr && subtree_root->right == nullptr){ // no children
    subtree_root->height = 1;
  }

  return subtree_root;

}

template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>::remove(Node* subtree_root, const K& a_key){
if(size() >0){
  if(subtree_root != nullptr && subtree_root->key > a_key){ // if greater than go left
    subtree_root->left = remove(subtree_root->left, a_key);
  }
  else if(subtree_root != nullptr && subtree_root->key < a_key){ // if less than go right
    subtree_root->right = remove(subtree_root->right, a_key);
  }
  else if(subtree_root != nullptr && subtree_root->key == a_key){ // here we have found deletion recurrsively
    Node* curr =nullptr;
    // curr = subtree_root->right; // i could not get thsi code to work and update the heights reccursively while deleting
    // if (curr != nullptr ){
    //   if(curr->left == nullptr){ //we are in the special case
    //     //subtree_root->right = curr->right; //skipping over right node
    //     subtree_root->value = curr->value;// copying values and keys
    //     subtree_root->key = curr->key;
    //     remove(subtree_root->right,curr->key);
    //     node_count--;
    //     return subtree_root;
    //     }
    //   else{
    //     while(curr->left != nullptr){ // finding in-order successor
    //       //prev = curr;
    //       curr = curr->left;
    //     }
    //     subtree_root->value = curr->value;//copying values
    //     subtree_root->key = curr->key;
    //         //prev->left = curr->right;
    //         //delete curr;
    //     remove(subtree_root->right,curr->key);
    //     node_count--;
    //     return subtree_root;
    //   }
    // }
    // curr = nullptr;
    Node* prev = nullptr;
    if(subtree_root->left == nullptr){
      if(subtree_root->right == nullptr){ // no children case: leaf node
        if(root == subtree_root){
          root = nullptr;
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
  // back tracking update heights, could not get to work while deleting heights
  // if(subtree_root->left != nullptr && subtree_root->right != nullptr){ // if it has 2 children
  //         if( subtree_root->left->height >= subtree_root->right->height){
  //           subtree_root->height = subtree_root->left->height +1;
  //         }
  //         else{
  //           subtree_root->height = subtree_root->right->height +1;
  //         }
  // }
  // if(subtree_root->right != nullptr && subtree_root->left == nullptr){ // one right child
  //   subtree_root->height = subtree_root->right->height +1;
  // }
  // else if(subtree_root->left != nullptr && subtree_root->right == nullptr){ // one left child
  // subtree_root->height = subtree_root->left->height +1;
  // }
  // else if(subtree_root->left == nullptr && subtree_root->right == nullptr){ // no children
  //   subtree_root->height = 1;
  // }

  }
  return subtree_root;
};

template<typename K, typename V>
void AVLCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys)const{
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
void AVLCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys)const{
  if(subtree_root == nullptr){ //inorder traversal
    return;
  }
  keys(subtree_root->left,all_keys);

  all_keys.add(subtree_root->key);

  keys(subtree_root->right, all_keys);
    
};

template<typename K, typename V>
size_t AVLCollection<K,V>::height(const Node* subtree_root)const{
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


// add recursive helper function
template<typename K, typename V>
typename AVLCollection<K,V>::Node*
AVLCollection<K,V>:: add(Node* subtree_root, const K& a_key, const V& a_val){
    if(subtree_root == nullptr){//create node if we have reached the final location
        subtree_root = new Node;
        subtree_root->left = nullptr;
        subtree_root->right = nullptr;
        subtree_root->key = a_key;
        subtree_root->value = a_val;
        subtree_root->height =1;
    }
    else{
        if(a_key < subtree_root->key){
            subtree_root->left = add(subtree_root->left, a_key, a_val);
        }
        else{
            subtree_root->right = add(subtree_root->right, a_key, a_val); // recurrsively traverse to insertion
        }
        //backtracking: adjust heights
        if(subtree_root->left != nullptr && subtree_root->right != nullptr){ // if it has 2 children
          if( subtree_root->left->height >= subtree_root->right->height){ //compare child heights and update
            subtree_root->height = subtree_root->left->height +1;
          }
          else{
            subtree_root->height = subtree_root->right->height +1;
          }
        }
        if(subtree_root->right != nullptr && subtree_root->left == nullptr){ // one right child
          subtree_root->height = subtree_root->right->height +1;
        }
        else if(subtree_root->left != nullptr && subtree_root->right == nullptr){ // one left child
          subtree_root->height = subtree_root->left->height +1;
        }
        else if(subtree_root->left == nullptr && subtree_root->right == nullptr){
          subtree_root->height = 1;
        }
    }
    // backtracking: rebalance at the subtreee root as needed
    return rebalance(subtree_root);
    //return subtree_root;

};


template<typename K, typename V>
void AVLCollection<K,V>:: add(const K& a_key, const V& a_val){
    root = add(root,a_key, a_val);
    node_count++;
};


template<typename K, typename V>
void AVLCollection<K,V>:: remove(const K& a_key){
  if((size() > 0)){ //there are keys in the table
    remove(root,a_key);// recurresive helper to remove node
    updateheight(root); //helper to make heights correct before rebalancing
    root = rebalance(root); // rebalancing tree and setting to root
  }
};

template<typename K, typename V>
bool AVLCollection<K,V>:: find(const K& search_key, V& the_val) const{
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
void AVLCollection<K,V>:: find(const K& k1, const K& k2, ArrayList<K>& keys) const{ 
    if((k2 >= k1) && (size() > 0)){ // if we have a positive range and a postive size
        while(keys.size()>0){// make return array empty
            keys.remove(0);
        }
          find(root, k1, k2, keys);
        }         
 };

template<typename K, typename V>
void AVLCollection<K,V>:: keys(ArrayList<K>& all_keys) const{
if(size() > 0){
    while(all_keys.size()>0){
        all_keys.remove(0);
    }
    keys(root, all_keys);
  }
 };

template<typename K, typename V>
void AVLCollection<K,V>:: sort(ArrayList<K>& all_keys_sorted) const{ 
    keys(all_keys_sorted); //we can simply call keys because it is and inorder traversal
 };

template<typename K, typename V>
size_t AVLCollection<K,V>:: size() const{
    return node_count;
};




#endif
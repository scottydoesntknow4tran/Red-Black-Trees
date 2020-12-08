//----------------------------------------------------------------------
// Name: Scott Tornquist
// File: rbt_collection.h
// Date: Fall, 2020
// Desc: A Key-Value Collection implementation using a Red-Black tree.
//----------------------------------------------------------------------


#ifndef RBT_COLLECTION_H
#define RBT_COLLECTION_H


#include "string.h"
#include "collection.h"
#include "array_list.h"
#include <algorithm>


template<typename K, typename V>
class RBTCollection : public Collection<K,V>
{
public:

  // create an empty collection
  RBTCollection();
  
  // copy constructor
  RBTCollection(const RBTCollection<K,V>& rhs);

  // assignment operator
  RBTCollection<K,V>& operator=(const RBTCollection<K,V>& rhs);

  // delete collection
  ~RBTCollection();
  
  // add a new key-value pair into the collection 
  void add(const K& a_key, const V& a_val);

  // remove a key-value pair from the collectiona
  void remove(const K& a_key);

  // find and return the value associated with the key
  bool find(const K& search_key, V& the_val) const;

  // find and return each key >= k1 and <= k2 
  void find(const K& k1, const K& k2, ArrayList<K>& keys) const;
  
  // return all of the keys in the collection 
  void keys(ArrayList<K>& all_keys) const;

  // return all of the keys in ascending (sorted) order
  void sort(ArrayList<K>& all_keys_sorted) const;

  // return the number of key-value pairs in the collection
  size_t size() const;

  // return the height of the tree
  size_t height() const;

  // for testing:

  // check if tree satisfies the red-black tree constraints
  bool valid_rbt() const;
  
  // pretty-print the red-black tree (with heights)
  void print() const;

  
private:
  
  // RBT node structure
  enum color_t {RED, BLACK};
  struct Node {
    K key;
    V value;
    Node* left;
    Node* right;
    Node* parent;
    color_t color;
  };

  // root node
  Node* root;

  // number of k-v pairs stored in the collection
  size_t node_count;

  // helper to empty entire hash table
  void make_empty(Node* subtree_root);

  // copy helper
  void copy(Node* lhs_subtree_root, const Node* rhs_subtree_root); 
    
  // helper to recursively find range of keys
  void find(const Node* subtree_root, const K& k1, const K& k2,
            ArrayList<K>& keys) const;

  // helper to build sorted list of keys (used by keys and sort)
  void keys(const Node* subtree_root, ArrayList<K>& all_keys) const;

  // rotate right helper
  void rotate_right(Node* k2);

  // rotate left helper
  void rotate_left(Node* k2);

  // restore red-black constraints in add
  void add_rebalance(Node* x);

  // restore red-black constraints in remove
  void remove_rebalance(Node* x, bool going_right);
  
  // height helper
  size_t height(const Node* subtree_root) const;
  
  // ------------
  // for testing:
  // ------------
  
  // validate helper
  bool valid_rbt(Node* subtree_root) const;

  // validate helper
  size_t black_node_height(Node* subtree_root) const;
  
  // recursive pretty-print helper
  void print_tree(std::string indent, Node* subtree_root) const;
};

//______________________________________________________________________________________
// TODO: Finish the above functions below
//______________________________________________________________________________________



template<typename K, typename V>
void RBTCollection<K,V>::rotate_right(Node* k2){ //simple right rotation
    //same code
    Node* k1 = k2->left;
    k2->left = k1->right;

    //UPDATE K2->left's parent to K2 if not null
    if(k2->left != nullptr){
        k2->left->parent = k2;
    }

    // UPDATE K1's parent
    k1->parent = k2->parent;

    // Set K1's new parent to K1 if it exits
    if(k1->parent != nullptr){
        if(k1->parent->key > k1->key){
            k1->parent->left = k1;
        }
        else{
            k1->parent->right = k1;
        }
    }

    // same code
    k1->right = k2;
    k2->parent = k1;

    //UPDATE ROOT
    if(root == k2){
        root = k1;
    }
};

template<typename K, typename V>
void RBTCollection<K,V>::rotate_left(Node* k2){ //simple left rotation
  //same code
    Node* k1 = k2->right;
    k2->right = k1->left;

    //UPDATE K2->left's parent to K2 if not null
    if(k2->right != nullptr){
        k2->right->parent = k2;
    }

    // UPDATE K1's parent
    k1->parent = k2->parent;

    // Set K1's new paret to K1 if it exits
    if(k1->parent != nullptr){
        if(k1->parent->key > k1->key){
            k1->parent->left = k1;
        }
        else{
            k1->parent->right = k1;
        }
    }

    // same code
    k1->left = k2;
    k2->parent = k1;

    //UPDATE ROOT
    if(root == k2){
        root = k1;
    } 
};

template<typename K, typename V>
void RBTCollection<K,V>::add_rebalance(Node* x){

    Node* p = x->parent;

    // CASE 1: COLOR FLIP (if x is black and both childeren are red)
    if(x->color == BLACK && x->left != nullptr && x->right != nullptr && x->left->color == RED && x->right->color == RED){
        x->color = RED;
        x->left->color = BLACK;
        x->right->color = BLACK;
    }


    //CASE 2 AND 3: ROTATIONS (if x and parent are Red)
    if (x->color == RED && p!= nullptr && p->color == RED ){
        // if P is the root(special case)
        if(p->parent == nullptr){
            // handling if x is the right child
            if(p->right != nullptr && p->right == x){
                rotate_left(p);
                root = x;
            }// handling if x is the left child
            else if (p->left != nullptr && p->left == x){
                rotate_right(p);
                root = x;
            }
        } // if X has a grand parent (Normal case)
        else if( p->parent != nullptr){
            //CASE 2,3: DOUBLE ROTATION

            // LEFT SIDE OF GRANDPARENT
            if(p->parent->left == p){ 
                //INSIDE NODE: DOUBLE ROTATION
                if(p->right == x){
                    rotate_left(p);
                    x = p;
                }
                //CASE 2: OUTSIDE NODE: Single Rotation
                rotate_right(x->parent->parent);
                x->parent->color = BLACK; //recoloring
                x->parent->right->color = RED;
            }

            // RIGHT SIDE OF GRANDPARENT
            else if(p->parent->right == p){ 
                //INSIDE NODE: DOUBLE ROTATION
                if(p->left == x){
                    rotate_right(p);
                    x = p;
                }
                //CASE 2: OUTSIDE NODE: Single Rotation
                rotate_left(x->parent->parent);
                x->parent->color = BLACK; //recoloring
                x->parent->left->color = RED;
            }
        }
    }
    
};



template<typename K, typename V>
void RBTCollection<K,V>:: add(const K& a_key, const V& a_val){

    //INTITALIZING NEW NODE TO RED
    Node* n = new Node;
    n->left = nullptr;
    n->right = nullptr;
    n->parent = nullptr;
    n->key = a_key; //loading key-value pair
    n->value = a_val;
    n->color = RED; // Setting new node to red

    //INITALIZING TRAVERSAL NODE TO ROOT
    Node* x = root;

    //INTITALISING PARENT OF X TO ROOT
    Node* p = nullptr;

    //ITERATIVLEY FINDING THE INSERTION POINT AND REBALANCING
    while(x != nullptr){

        add_rebalance(x); //rebalancing and recoloring

        p = x; //assigning parent of x

        // navitgating down tree
        if(a_key < x->key){ 
            x = x->left; 
        }
        else{
            x = x->right;
        }    
    }

    // ADDING N
    if(p == nullptr){
        root = n;//adding first node
        n->parent = nullptr;
    }
    else if(a_key < p->key){
        p->left = n;
        n->parent = p;
    }
    else{
        p->right = n;
        n->parent = p;
    }
    
    // REBALANCING N
    add_rebalance(n);

    // MAKE ROOT BLACK
    root->color = BLACK;

    node_count++;
};


template<typename K, typename V>
void RBTCollection<K,V>:: remove(const K& a_key){
    if(size() > 0 && root != nullptr){ //there are keys in the table

        //INTITALIZING SENTINEL NODE
        Node* sentinel = new Node;
        sentinel->left = nullptr;
        sentinel->right = root;
        root->parent = sentinel;
        sentinel->parent = nullptr; 
        sentinel->color = RED; // Setting new node to red

        //INITALIZING TRAVERSAL NODE TO ROOT
        Node* x = root;

        //INTITALISING PARENT OF X TO ROOT
        Node* p = sentinel;

        bool found = false; // boolean indicating found

        //ITERATIVLEY FINDING THE REMOVAL NODE AND REBALANCING
        while(x != nullptr && found == false){

            // navitgating down tree
            if(a_key < x->key){ // GO LEFT
                remove_rebalance(x,false);
                x = x->left; 
            }
            else if(a_key > x->key){ // GO RIGHT
                remove_rebalance(x,true);
                x = x->right;
            }   
            else{ //AT REMOVING
                remove_rebalance(x, x->left);
                found = true;// makes sure this else executes befor exiting
            }
            p = x->parent; //updating paretn
        } 

        //CHECK IF NODE WAS FOUND
        if(found == false){
            return;
        }

        // DELETION CASES
        if(x->right == nullptr){
            //NO CHILDREM
            if(x->left == nullptr){
                if(p->left == x){ // X left child of P
                    p->left = x->left;
                }
                else{ // X right child of P
                    p->right = x->left;
                }
                delete x;
                node_count--;
            }
            else{//ONE LEFT CHILD OF X
                if(p->left == x){ // X left child of P
                    p->left = x->left;
                }
                else{ // X right child of P
                    p->right = x->left;
                }
                x->left->parent = p;
                delete x;
                node_count--;
            }
        }
        else{
            if(x->left == nullptr){// ONE RIGHT CHILD
                if(p->left == x){ // X left child of P
                    p->left = x->right;
                }
                else{ // X right child of P
                    p->right = x->right;
                }
                x->right->parent = p;
                delete x;
                node_count--;
            }
            else{//TWO CHILDREN
                Node* s = x->right;
                remove_rebalance(s, false);
                while(s->left != nullptr){
                    s = s->left;
                    remove_rebalance(s, false);
                }

                //FINDING INDORDER SUCCESSOR
                x->key = s->key;// copying values
                x->value = s->value;
                if(s != x->right){ // regular case
                    s->parent->left = s->right;
                }
                else{// special case(did not move)
                    s->parent->right = s->right;
                }
                if(s->right != nullptr){
                    s->right->parent = s->parent;
                }
                delete s;
                node_count--;
            }
        }

        //CLEAN UP
        root = sentinel->right;
        if(root != nullptr){
            root->color = BLACK;
            root->parent = nullptr;
        }
        delete sentinel;
    }
};

template<typename K, typename V>
void RBTCollection<K,V>:: remove_rebalance(Node* x, bool going_right){
    //print();
    //X IS RED, DONE
    if (x->color == RED){
        return;
    }

    //INTITALIZING PARENT AND SIBLING
    Node* p = x->parent;
    Node* t = nullptr;
    if(p->left == x){
        t = p->right;
    }
    else{
        t = p->left;
    }

    //CASE 1: X HAS NON_NAVIGATION RED CHILD
    if(x->right != nullptr && x->right->color == RED){
        // RED RIGHT CHILD AND BLACK or null LEFT CHILD AND LEFT NAVIGATION
        if((x->left == nullptr || x->left->color == BLACK) && going_right == false){
            rotate_left(x);
            x->color = RED;
            x->parent->color = BLACK;
        }
    }
    else if(x->left != nullptr && x->left->color == RED){
        // BLACK or null RIGHT CHILD AND RED LEFT CHILD AND RIGHT NAVIGATION
        if((x->right == nullptr || x->right->color == BLACK) && going_right == true){
            rotate_right(x);
            x->color = RED;
            x->parent->color = BLACK;
        }
    }//CASE 2: COLOR FLIP // given X's children are both black// does t have 2 black or null children
    else if(t != nullptr && t->color == BLACK && (t->left == nullptr && (t->right == nullptr || t->right->color == BLACK) || t->left != nullptr && (t->left->color == BLACK && (t->right == nullptr || t->right->color == BLACK))) ){
        // if(t->left == nullptr){
        //     if(t->right == nullptr){//Both T's children are null
        //         t->color = RED;
        //         x->color = RED;
        //         p->color = RED;
        //     }
        //     else if(t->right->color == BLACK){ //T's left null, right Black
        //         t->color = RED;
        //         x->color = RED;
        //         p->color = RED;
        //     }
        // }
        // else if(t->left->color == BLACK){
        //     if(t->right == nullptr){//T's Left Black right null
        //         t->color = RED;
        //         x->color = RED;
        //         p->color = RED;
        //     }
        //     else if(t->right->color == BLACK){ //T's left Black, right Black
        //         t->color = RED;
        //         x->color = RED;
        //         p->color = RED;
        //     }
        t->color = RED;
        x->color = RED;
        p->color = BLACK;

    }//CASE 3 and 4: OUTSIDE AND INSIDE RED SIBLING CHILDREN //if R is right of T
    else if(t!= nullptr && t->color == BLACK && t->right != nullptr && t->right->color == RED && (t->left == nullptr || t->left->color == BLACK)){
        //T is black, and has a Right RED NODE only to the right
        if(p->right == t){ //right-right case CASE 3
            rotate_left(p);
            p->color = BLACK;
            t->color = RED;
            x->color = RED;
            t->right->color = BLACK;
        }
        else{//left-right: CASE 4
            rotate_left(t);
            rotate_right(p);
            p->color = BLACK;
            //t->color = BLACK;
            x->color = RED;
            //t->parent = BLACK;
        }
    }// if R is left of T
    else if(t!= nullptr && t->color == BLACK && t->left != nullptr && t->left->color == RED && (t->right == nullptr || t->right->color == BLACK)){
        //T is black, and has a Right RED NODE only to the left
        if(p->left == t){ //left-left case
            rotate_right(p);
            p->color = BLACK;
            t->color = RED;
            x->color = RED;
            t->right->color = BLACK;
        }
        else{//right-left
            rotate_right(t);
            rotate_left(p);
            p->color = BLACK;
            //t->color = BLACK;
            x->color = RED;
            //t->parent = BLACK;
        }
    }




};



//________________________________________________________________________________
// done functions
//________________________________________________________________________________



template<typename K, typename V>
size_t RBTCollection<K,V>::height(const Node* subtree_root)const{
  size_t h =1;
  if(subtree_root->right != nullptr){ //checking right path
    if(subtree_root->left != nullptr){// two paths
        h = 1 + height(subtree_root->left);
        if(h < 1+ height(subtree_root->right)){
            h = 1 + height(subtree_root->right);
        }
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
bool RBTCollection<K,V>:: find(const K& search_key, V& the_val) const{
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
void RBTCollection<K,V>:: find(const K& k1, const K& k2, ArrayList<K>& keys) const{ 
    if((k2 >= k1) && (size() > 0)){ // if we have a positive range and a postive size
        while(keys.size()>0){// make return array empty
            keys.remove(0);
        }
          find(root, k1, k2, keys);
        }         
 };

template<typename K, typename V>
void RBTCollection<K,V>:: keys(ArrayList<K>& all_keys) const{
if(size() > 0){
    while(all_keys.size()>0){
        all_keys.remove(0);
    }
    keys(root, all_keys);
  }
 };

template<typename K, typename V>
void RBTCollection<K,V>:: sort(ArrayList<K>& all_keys_sorted) const{ 
    keys(all_keys_sorted); //we can simply call keys because it is and inorder traversal
 };

template<typename K, typename V>
size_t RBTCollection<K,V>:: size() const{
    return node_count;
};


template<typename K, typename V>
void RBTCollection<K,V>::find(const Node* subtree_root, const K& k1, const K& k2, ArrayList<K>& keys)const{
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
void RBTCollection<K,V>::keys(const Node* subtree_root, ArrayList<K>& all_keys)const{
  if(subtree_root == nullptr){ //inorder traversal
    return;
  }
  keys(subtree_root->left,all_keys);

  all_keys.add(subtree_root->key);

  keys(subtree_root->right, all_keys);
    
};

template<typename K, typename V>
RBTCollection<K,V>::RBTCollection(){ //constructor
    node_count=0;
    root = nullptr;
};

    //destructor
template<typename K, typename V>
RBTCollection<K,V>::~RBTCollection(){ //destructor
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
RBTCollection<K,V>::RBTCollection(const RBTCollection <K,V>& rhs){
    root = nullptr;
    *this = rhs; // defers to assignment operator
};

    // assignment operator
template<typename K, typename V>
RBTCollection<K,V>& RBTCollection<K,V>::operator=(const RBTCollection<K,V>& rhs){
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
size_t RBTCollection<K,V>::height()const{
  if(root != nullptr){
    return height(root);
  }
  return 0;

};

template<typename K, typename V>
void RBTCollection<K,V>::make_empty(Node* subtree_root){
  if(subtree_root != nullptr){
    make_empty(subtree_root->left);
    make_empty(subtree_root->right);
    delete subtree_root;
    node_count--;
  }
};

template<typename K, typename V>
void RBTCollection<K,V>::copy (Node* lhs_subtree_root, const Node* rhs_subtree_root){ //copy contructor helper
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





//----------------------------------------------------------------------
// Provided Helper Functions:
//----------------------------------------------------------------------

template<typename K, typename V>
bool RBTCollection<K,V>::valid_rbt() const
{
  return !root or (root->color == BLACK and valid_rbt(root));
}


template<typename K, typename V>
bool RBTCollection<K,V>::valid_rbt(Node* subtree_root) const
{
  if (!subtree_root)
    return true;
  color_t rc = subtree_root->color;
  color_t lcc = subtree_root->left ? subtree_root->left->color : BLACK;
  color_t rcc = subtree_root->right ? subtree_root->right->color : BLACK;  
  size_t lbh = black_node_height(subtree_root->left);
  size_t rbh = black_node_height(subtree_root->right);
  bool lv = valid_rbt(subtree_root->left);
  bool rv = valid_rbt(subtree_root->right);
  // check equal black node heights, no two consecutive red nodes, and
  // left and right are valid RBTs
  return (lbh == rbh) and (rc != RED or (lcc != RED and rcc != RED)) and lv and rv;
}


template<typename K, typename V>
size_t RBTCollection<K,V>::black_node_height(Node* subtree_root) const
{
  if (!subtree_root)
    return 1;
  size_t hl = black_node_height(subtree_root->left);
  size_t hr = black_node_height(subtree_root->right);
  size_t h = hl > hr ? hl : hr;
  if (subtree_root->color == BLACK)
    return 1 + h;
  else
    return h;
}


template<typename K, typename V>
void RBTCollection<K,V>::print() const
{
  print_tree("", root);
}


template<typename K, typename V>
void RBTCollection<K,V>::print_tree(std::string indent, Node* subtree_root) const
{
  if (!subtree_root)
    return;
  std::string color = "[BLACK]";
  if (subtree_root->color == RED)
    color = "[RED]";
  std::cout << indent << subtree_root->key << " "
	    << color << " (h="
	    << height(subtree_root) << ")" << std::endl;
  print_tree(indent + "  ", subtree_root->left);
  print_tree(indent + "  ", subtree_root->right);
}


#endif

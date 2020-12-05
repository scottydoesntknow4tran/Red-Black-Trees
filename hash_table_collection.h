//----------------------------------------------------------------------
// FILE: hash_table_collection
// NAME: Scott Tornquist
// DATE: 10/25/2020
// DESC: Implements a hash table version of the collection
//       class. Includes all member function inherited from collection
//       as well as the constructor, destructor, copy constructor, and
//       assignment operator and three public statistic functions
//----------------------------------------------------------------------


#ifndef HASH_TABLE_COLLECTION_H
#define HASH_TABLE_COLLECTION_H

#include "array_list.h"
#include "collection.h"
#include <functional>

template<typename K, typename V>
class HashTableCollection : public Collection<K,V>
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
    HashTableCollection();

    //destructor
    ~HashTableCollection();

    //copy constructor
    HashTableCollection(const HashTableCollection <K,V>& rhs);

    // assignment operator
    HashTableCollection& operator=(const HashTableCollection<K,V>& rhs);

    //three public statistics functions
    size_t min_chain_length();

    size_t max_chain_length();

    double avg_chain_length();


private:

    //the chain (linkedlist) nodes
    struct Node{
        K key;
        V value;
        Node* next;
    };

    // the (resizable) hash table 
    Node** hash_table;

    // number of k-v pairs stored in the collection
    size_t length;

    //current number of buckets in the hash table
    size_t table_capacity;

    //current load factor of the hash table
    double load_factor_threshold = .75;

    // double size and rehash the hash table
    void resize_and_rehash();

    //ArrayList<std::pair<K,V>> kv_list;

};



template<typename K, typename V>
void HashTableCollection<K,V>::resize_and_rehash(){
size_t newlength = 0; // creating new table
size_t newcapacity = 2*table_capacity;
Node** newtable = new Node*[newcapacity];
for(int i=0; i<newcapacity; i++){ 
    newtable[i] = nullptr;
}
std::hash<K> hash_fun;
for(int i=0; i<table_capacity; i++){// iterating through old table to ge all nodes
        if(hash_table[i] != nullptr){
            Node* itr = hash_table[i];
            while(itr!=nullptr){
                size_t newindex = hash_fun(itr->key)%newcapacity;// finds new index for old node in new table

                Node* ptr = new Node; //creates new node and add it the new table
                ptr->key = itr->key; // I just realized that splicing this node into the new table could save us from having to delete it at all.
                ptr->value = itr->value;
                ptr->next = newtable[newindex];
                newtable[newindex] = ptr;
                newlength++;

                itr = itr->next;
            }
        }
}    
for(int i=0; i<table_capacity; i++){// deletes all old nodes (there wouldnt be any if we spliced them)
    if(hash_table[i]!=nullptr){
        Node* ptr = hash_table[i];
        Node* itr = ptr->next;
        while(itr!=nullptr){
            delete ptr;
            ptr = itr;
            itr = itr->next;
        }
        delete ptr;
    }
}
delete hash_table;// deletes old table and sets new table parameters
hash_table = newtable; //replacing with new table
length = newlength;
table_capacity = newcapacity;
newtable = nullptr;
};

template<typename K, typename V>
HashTableCollection<K,V>::HashTableCollection(){ //constructor
    length =0;
    table_capacity = 16;
    hash_table = new Node*[table_capacity];
    for(int i=0; i<table_capacity; i++){ // sets every pointer to null ptr to indicate empty
        hash_table[i] = nullptr;
    }
};

    //destructor
template<typename K, typename V>
HashTableCollection<K,V>::~HashTableCollection(){ //destructor
for(int i=0; i<table_capacity; i++){// deletes all old nodes 
    if(hash_table[i]!=nullptr){
        Node* ptr = hash_table[i];
        Node* itr = ptr->next;
        while(itr!=nullptr){
            delete ptr;
            ptr = itr;
            itr = itr->next;
        }
        delete ptr;
    }
}
  length = 0;
  table_capacity = 0;
  delete [] hash_table;
};

    //copy constructor
template<typename K, typename V>
HashTableCollection<K,V>::HashTableCollection(const HashTableCollection <K,V>& rhs){
    *this = rhs; // defers to assignment operator, we dont need to create new table beacuse AO does that
};

    // assignment operator
template<typename K, typename V>
HashTableCollection<K,V>& HashTableCollection<K,V>::operator=(const HashTableCollection<K,V>& rhs){
if(this != &rhs){ // list1= list1
  table_capacity = rhs.table_capacity;
  hash_table = new Node*[table_capacity];// creating new hash table
  for(int i=0; i<16; i++){
    hash_table[i] = nullptr;
  }
  length=0;
    for(int i=0; i<table_capacity; i++){ // moving old values in reverse order of the chains as to keep them in their orginal order
        if(rhs.hash_table[i] != nullptr){
            Node* ptr = rhs.hash_table[i];
            int k = 0;
            while(ptr->next != nullptr){// goes to last value in chain
                ptr = ptr->next;
                k++; // counts length of the chain
            }    
            add(ptr->key,ptr->value); // add last value
            for(int q = 0; q < k; q++){ // iterates for the length of the chain
                ptr = rhs.hash_table[i];
                for(int t = 0; t < k-q-1; t++){// iterates to the next last value each time
                ptr = ptr->next;
                }
                add(ptr->key,ptr->value);// adds the last value to the new table.
            }
        }
    }    
  }
  //return lhs(this)
  return *this;
};

    //three public statistics functions
template<typename K, typename V>
size_t HashTableCollection<K,V>:: min_chain_length(){
    size_t min=0;
    if(size() > 0){
        for(int i=0; i<table_capacity; i++){
            if(hash_table[i] != nullptr){
                Node* ptr = hash_table[i];
                size_t count = 0;
                while(ptr!=nullptr){
                    ptr = ptr->next;
                    count++;
                }
                if(min == 0){ //if min hasnt been set yet
                    min = count;
                }
                else if(count < min){
                    min = count;
                }
            }
        }         
    }
    return min;
};

template<typename K, typename V>
size_t HashTableCollection<K,V>::max_chain_length(){
    size_t max=0;
    if(size() > 0){
        for(int i=0; i<table_capacity; i++){
            if(hash_table[i] != nullptr){
                Node* ptr = hash_table[i];
                size_t count =1;
                while(ptr!=nullptr){ // counts the length of each chain
                    ptr = ptr->next;
                    count++;
                }
                if(count > max){ //udpates the max chain value
                    max = count;
                }
            }
        }         
    }
    return max;
};

template<typename K, typename V>
double HashTableCollection<K,V>::avg_chain_length(){
    double chain_count=0;
    if(size() > 0){
        for(int i=0; i<table_capacity; i++){// iterates through the table to find how many pointer are not null, meaning there is a chain
            if(hash_table[i] != nullptr){ 
                chain_count++;
            }
        }         
    return length/chain_count; // calculates average chain length
    }
    return 0;
};

template<typename K, typename V>
void HashTableCollection<K,V>:: add(const K& a_key, const V& a_val){
if(length/table_capacity >= load_factor_threshold){ // if above threshold, rezise and rehash
    resize_and_rehash();
}
std::hash<K> hash_fun; // finding hash index
size_t code = hash_fun(a_key);
size_t index = code%table_capacity; 

Node* ptr = new Node; // placing node at hash index and moving pointers
ptr->key = a_key;
ptr->value = a_val;
ptr->next = hash_table[index];
hash_table[index] = ptr;
length++;
};



template<typename K, typename V>
void HashTableCollection<K,V>:: remove(const K& a_key){
   if((size() > 0)){ //there are keys in the table
    std::hash<K> hash_fun;
    size_t code = hash_fun(a_key);
    size_t index = code%table_capacity;
    if(hash_table[index]!= nullptr){// there are key(s) in the bucket
        Node* ptr2 = hash_table[index];
        if(hash_table[index]->next == nullptr){ //if there is only one key in the bucket
            if(ptr2->key == a_key){ //if it is the correct key
                hash_table[index] = nullptr;
                delete ptr2;
                length--;
                return;
            }
            else{ //if it not the correct key
                return;
            }
        }
        Node* ptr = hash_table[index]->next; // there is at least 2 keys
        if(ptr2->key == a_key){ //if it is the first key
                hash_table[index] = ptr;
                delete ptr2;
                length--;
                return;
        }
        while(ptr!= nullptr){
            if(ptr->key == a_key){
                ptr2->next=ptr->next;
                delete ptr;
                length--;
                return;
            }
        ptr= ptr->next;
        ptr2 = ptr2->next;
        }
    }
   }
 };

template<typename K, typename V>
bool HashTableCollection<K,V>:: find(const K& search_key, V& the_val) const{
   if((size() > 0)){
    std::hash<K> hash_fun;
    size_t code = hash_fun(search_key);
    size_t index = code%table_capacity;
    Node* ptr = hash_table[index]; // hash to index from the key
    while(ptr!= nullptr){ // iterate through the chain to find the value
        if(ptr->key == search_key){
            the_val = ptr->value;
            return true;
        }
        ptr= ptr->next;
    }
   }
     return false; // if not found return false
 };

template<typename K, typename V>
void HashTableCollection<K,V>:: find(const K& k1, const K& k2, ArrayList<K>& keys) const{ 
    if((k2 >= k1) && (size() > 0)){
        while(keys.size()>0){// make return array empty
            keys.remove(0);
        }
        for(int i=0; i<table_capacity; i++){// go though entire table
            if(hash_table[i]!=nullptr){
                Node* ptr = hash_table[i];
                while(ptr!=nullptr){
                    if(ptr->key >= k1 && ptr->key<= k2){// if the key is the range, add it
                        keys.add(ptr->key);
                    }
                    ptr = ptr->next;
                }
            }
        }         
    }
 };

template<typename K, typename V>
void HashTableCollection<K,V>:: keys(ArrayList<K>& all_keys) const{
if(size() > 0){
        while(all_keys.size()>0){
            all_keys.remove(0);
        }
        for(int i=0; i<table_capacity; i++){ // go through entire table
            if(hash_table[i]!=nullptr){
                Node* ptr = hash_table[i];
                while(ptr!=nullptr){
                    all_keys.add(ptr->key);// add all keys
                    ptr = ptr->next;
                }
            }
        }         
    }
 };

template<typename K, typename V>
void HashTableCollection<K,V>:: sort(ArrayList<K>& all_keys_sorted) const{ 
    if((size() > 0)){
      while(all_keys_sorted.size()>0){
       all_keys_sorted.remove(0);
      }
     keys(all_keys_sorted); // get all keys frome the table
     all_keys_sorted.sort(); // sort the array with all the keys using quick sort
   }
 };

template<typename K, typename V>
size_t HashTableCollection<K,V>:: size() const{
    return length;
};


#endif
//----------------------------------------------------------------------
// FILE: array_list_collection
// NAME: Scott Tornquist
// DATE: 10/25/2020
// DESC: Implements a array list version of the collection
//       class. Elements are added by default to the last available
//       index in the array. Remove finds the key to remove and removes
//       it. Find give the value of a given key and returns true if found.
//       Find range gives the all the keys in between 2 keys.
//       keys gives all the keys. Sort give a sorted list of all the keys
//       using quick sort
//----------------------------------------------------------------------

#ifndef ARRAY_LIST_COLLECTION_H
#define ARRAY_LIST_COLLECTION_H

#include "array_list.h"
#include "collection.h"



template<typename K, typename V>
class ArrayListCollection : public Collection<K,V>
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

  ArrayList<std::pair<K,V>> kv_list;

};

template<typename K, typename V>
 void ArrayListCollection<K,V>:: add(const K& a_key, const V& a_val){
   std::pair<K,V> a;
   a = std::make_pair(a_key, a_val);
   kv_list.add(a); // adds pair to end of the array list
 };

 template<typename K, typename V>
 void ArrayListCollection<K,V>:: remove(const K& a_key){
   if((size() > 0)){
     for(int i = 0; i<kv_list.size(); i++){ // iterate throught the list
       std::pair<K,V> a;
         kv_list.get(i,a);
       if(a_key == a.first){ // check each item if it the the key
         kv_list.remove(i); // remove if it is found
       }
     }
   }
 };

 template<typename K, typename V>
 bool ArrayListCollection<K,V>:: find(const K& search_key, V& the_val) const{
   if(size() > 0){
     for(int i = 0; i<kv_list.size(); i++){ //iterate through the list
       std::pair<K,V> a;
       kv_list.get(i,a);
       if(search_key == a.first){ // check each item to find the key
         the_val = a.second; // return the value of the key if found
         return true;
       }
     }
   }
     return false; //if not found return false
 };

 template<typename K, typename V>
 void ArrayListCollection<K,V>:: find(const K& k1, const K& k2, ArrayList<K>& keys) const{
   if((k2 >= k1) && (size() > 0)){
     while(keys.size()>0){ // make sure the list is empty
       keys.remove(0);
      }
     for(int i = 0; i<kv_list.size(); i++){ //search the list fore the first key
       std::pair<K,V> a;
       kv_list.get(i,a);
       if((a.first >= k1) && (a.first <= k2)){
         keys.add(a.first); // add all keys after the first key is found
       }
     }
   }
 };

template<typename K, typename V>
 void ArrayListCollection<K,V>:: keys(ArrayList<K>& all_keys) const{
     if((size() > 0)){
       while(all_keys.size()>0){
       all_keys.remove(0);
      }
     for(int i = 0; i<kv_list.size(); i++){  // add all keys to the list starting from the beginning
       std::pair<K,V> a;
        kv_list.get(i,a);
        all_keys.add(a.first);
     }
   }
 };

 template<typename K, typename V>
 void ArrayListCollection<K,V>:: sort(ArrayList<K>& all_keys_sorted) const{
    if((size() > 0)){
      while(all_keys_sorted.size()>0){
       all_keys_sorted.remove(0); // make list empty
      }
     keys(all_keys_sorted); // get all the keys from the list
     all_keys_sorted.sort(); // quick sort the list and return the list sorted
   }
 };

 template<typename K, typename V>
 size_t ArrayListCollection<K,V>:: size() const{
     return kv_list.size();
 };


#endif

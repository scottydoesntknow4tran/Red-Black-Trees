//----------------------------------------------------------------------
// FILE: bin_search_collection
// NAME: Scott Tornquist
// DATE: 10/25/2020
// DESC: Implements a binary search version of the collection
//       class. Elements are added by default in sorted order and the 
//       collection is kept in sorted order all times. Binary search is
//       give the positon of the element or the position of where the
//       the element should go along with returning true and false.
//----------------------------------------------------------------------


#ifndef BIN_SEARCH_COLLECTION_H
#define BIN_SEARCH_COLLECTION_H

#include "array_list.h"
#include "collection.h"

template<typename K, typename V>
class BinSearchCollection : public Collection<K,V>
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

private:
  ArrayList<std::pair<K,V>> kv_list;

    //binary search helper function
    bool bin_search(const K& key, size_t& index)const;
};

template<typename K, typename V>
 bool BinSearchCollection<K,V>:: bin_search(const K& key, size_t& index)const{
     size_t center = kv_list.size()/2; // finds center of the list
     size_t l = 0; // sets left to 0
     size_t r = kv_list.size(); // sets right to the last element in the list
     int count = 0; // count used to determine when we hav reached the end of the list and the k has not been found
     std::pair<K,V> temp;
for(;;) {
    kv_list.get(center, temp); 
    if (temp.first== key){ // if center is the key we are done 
        index = center;
        return true;
    }
    else if (temp.first > key) { // if the key is less that center search the left half of the list because it is sorter
        r = center; 
        center = (l+r)/2;
    }
    else { // else serach the right side of the list b/c is must be greater than the center
        l = center;
        center = (l+r)/2;
    }

    if((center == l) or (center == r)){ // if ceneter is equal to left or right we know we are down the 2 elements
        count++;
    }
    else{ // be left and right consecutivelye
        count = 0;
    }

    if(count ==2){ // if we search left and right consecutively we know we have searched everything and the element is not in the list
        if(center == l){//if center is left then the element is bigger than center, which is r
            index = r;
        }
        else if(center == r){ //if center is right then the element is smaller than center, which is l
            index = l;
        }
        return false; // we know we have searched the whole list and the item is not in the list so return false
    }
}

};


template<typename K, typename V>
 void BinSearchCollection<K,V>:: add(const K& a_key, const V& a_val){
   std::pair<K,V> a;
   a = std::make_pair(a_key, a_val);
   if(size() == 0){
   kv_list.add(a); //adds item toe end if the list is empty
   }
   else{
       size_t index;
       bin_search(a_key,index); // bin search finds location to add item
       kv_list.add(index, a); //adds items at the index found by binsearch to keep the list in sorted order
   }
 };

 template<typename K, typename V>
 void BinSearchCollection<K,V>:: remove(const K& a_key){
   if((size() > 0)){
     size_t index;
    if(bin_search(a_key,index)){ //if binsearch finds the value remove that item
        kv_list.remove(index);
     }
   }
 };

 template<typename K, typename V>
 bool BinSearchCollection<K,V>:: find(const K& search_key, V& the_val) const{
   if((size() > 0)){
    size_t index;
    if(bin_search(search_key,index)){ // if binsearch finds the item get the value from that key
        std::pair<K,V> temp;
        kv_list.get(index,temp);
        the_val= temp.second;
        return true;
    }
   }
     return false; // if not found return false
 };

 template<typename K, typename V>
 void BinSearchCollection<K,V>:: find(const K& k1, const K& k2, ArrayList<K>& keys) const{ 
   if((k2 >= k1) && (size() > 0)){
     while(keys.size()>0){
       keys.remove(0);
      } size_t index;
      bin_search(k1,index); // using binsearch to find the location of the first key or where it should be
     for(int i = index; i<kv_list.size(); i++){ // putting all values greater than the first key into the return list
       std::pair<K,V> a;
         kv_list.get(i,a);
       if((a.first >= k1) && (a.first <= k2)){ //putting in all keys in the range, all in sorted order
         keys.add(a.first);
       }
     }
   }
 };

template<typename K, typename V>
 void BinSearchCollection<K,V>:: keys(ArrayList<K>& all_keys) const{
     if((size() > 0)){
       while(all_keys.size()>0){
       all_keys.remove(0);
      }
     for(int i = 0; i<kv_list.size(); i++){ // putting all keys into the return list, all sorted
       std::pair<K,V> a;
         kv_list.get(i,a);
        all_keys.add(a.first);
     }
   }
 };

 template<typename K, typename V>
 void BinSearchCollection<K,V>:: sort(ArrayList<K>& all_keys_sorted) const{ 
    if((size() > 0)){
      while(all_keys_sorted.size()>0){
       all_keys_sorted.remove(0);
      }
     keys(all_keys_sorted); // all keys are alredy in sorted order
   }
 };

 template<typename K, typename V>
 size_t BinSearchCollection<K,V>:: size() const{
     return kv_list.size();
 };


#endif

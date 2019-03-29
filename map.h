#ifndef MAP_H
#define MAP_H
#include "pair.h"
//#include "../b_tree/btree.h"
#include "btree.h"

//=============================================================================================
// Class: Map
// Purpose: Self-implemented Map Class (key, value) with tempates
//=============================================================================================
template <typename K, typename V>
class Map{
private:
    //vector<Pair<K,V> > record_vector;
    Tree<Pair<K,V> > tree;
    int _size;
    
public:
    Map();
    int size();
    void Print();
    void Insert(const Pair<K, V> &p);
    void insertAt(K key, V value);
    void clear();
    vector<V> get_values(string findMe);
    //bracket operator vector<V> &operator [](K key)
    //vector<V> &operator [](K key);
};

//=============================================================================================
// Function: Map()
// Purpose: Default Constructor, sets the size of the tree (# of records), to the size
//          of our Map. Since each map holds a vector of record numbers, each record number
//          is also stored inside the tree. Both numbers are equivalent
//=============================================================================================
template <typename K, typename V>
Map<K, V>::Map(){
    _size = tree.getsize();
}

//=============================================================================================
// Function: size
// Purpose: getter
//=============================================================================================
template <typename K, typename V>
int Map<K, V>::size(){
    return tree.getsize();
}

//=============================================================================================
// Function: clear
// Purpose: Clears contents of the index tree
//=============================================================================================
template <typename K, typename V>
void Map<K,V>::clear(){
    tree.clear_tree();
}

//=============================================================================================
// Function: insertAt
// Purpose: Inserts the data into index tree
//=============================================================================================
template <typename K, typename V>
void Map<K,V>::insertAt(K key, V value){
    Pair<K, V> p(key, value);
    Insert(p);
}

//=============================================================================================
// Function: Print()
// Purpose: Prints the index tree
//=============================================================================================
template <typename K, typename V>
void Map<K, V>::Print(){
    tree.testPrint(0);
}

//=============================================================================================
// Function: get_values
// Purpose: gets all values in the index tree (field)
//=============================================================================================
template <typename K, typename V>
vector<V> Map<K,V>::get_values(string findMe){
    //cout<<"Map::get_vector: key: "<<k<<endl;
    Pair<K,V> p(findMe, {});
    if(tree.Search(p)){
        p = tree.Get(p);
        return p.value;
    }
    else{
        return vector<V>();
    }
}

//=============================================================================================
// Function: size
// Purpose: Duplicate records get handled by pushing the value into the vector of record
//          numbers. So if records with the same name are stored once in the tree, but
//          multiple record numbers exist, so just push the record number into the vector
//=============================================================================================
template <typename K, typename V>
void Map<K, V>::Insert(const Pair<K,V> &p){
    if(tree.Search(p)){
        //cout<<"Map::Insert: There is a dup, pushing value"<<endl;
        tree.Get(p).value.push_back(p.value[0]);
    }
    else{
        //cout<<"Map::Insert: Success, No dups"<<endl;
        tree.Insert(p);
    }
}


#endif // MAP_H

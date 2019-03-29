#ifndef PAIR_H
#define PAIR_H
#include "const.h"

//=============================================================================================
// Class: Pair
// Purpose: Self-implemented Pair class that takes in a templated arguments key and value
//=============================================================================================
template <typename K, typename V>
struct Pair{
    Pair();
    Pair(K k, V v);

    K key;
    vector<V> value;

    template <typename KK, typename VV>
    friend ostream& operator << (ostream& out,  Pair<KK,VV> &print);

    template <typename KK, typename VV>
    friend bool operator <= (const Pair<KK,VV> &first, const Pair<KK,VV> &second);

    template <typename KK, typename VV>
    friend bool operator > (const Pair<KK,VV> &first, const Pair<KK,VV> &second);

    template <typename KK, typename VV>
    friend bool operator < (const Pair<KK,VV> &first, const Pair<KK,VV> &second);

    template <typename KK, typename VV>
    friend bool operator >= (const Pair<KK,VV> &first, const Pair<KK,VV> &second);

    template <typename KK, typename VV>
    friend bool operator == (const Pair<KK,VV>&left, const Pair<KK,VV> &right);

};

//=============================================================================================
// Function: Pair
// Purpose: Default constructor
//=============================================================================================
template <typename K, typename V>
Pair<K, V>::Pair(){
    //empty
}

//=============================================================================================
// Function: Pair
// Purpose: Second constructor, takes in a key and a value, which is a vector
//=============================================================================================
template <typename K, typename V>
Pair<K, V>::Pair(K k, V v){
    key = k;
    value.push_back(v);
}

//=============================================================================================
// Function: Overloaded binary operator
// Purpose: To make it easy to print pairs with a cout statement
//=============================================================================================
template <typename KK, typename VV>
ostream& operator << (ostream& out,  Pair<KK,VV> &print){
    //cout<<endl<<"Pair << fired"<<endl;
    int indexes = print.value.size();
    out << print.key << " : ";
    for(int i=0; i<indexes; i++){
        out << print.value[i]<<"|";
    }

    return out;
}

//=============================================================================================
// Function: Overloaded comparison operator
// Purpose: Btree is sorted, so we need Pair to know how to sort itself
//=============================================================================================
template <typename KK, typename VV>
bool operator < (const Pair<KK,VV> &first, const Pair<KK,VV> &second){
    //cout<<endl<<"Pair < fired"<<endl;
    if(first.key < second.key){
        return true;
    }
    else{
        return false;
    }
}

//=============================================================================================
// Function: Overloaded comparison operator
// Purpose: Btree is sorted, so we need Pair to know how to sort itself
//=============================================================================================
template <typename KK, typename VV>
bool operator <= (const Pair<KK,VV> &first, const Pair<KK,VV> &second){
    if(first.key <= second.key){
        return true;
    }
    else{
        return false;
    }
}

//=============================================================================================
// Function: Overloaded comparison operator
// Purpose: Btree is sorted, so we need Pair to know how to sort itself
//=============================================================================================
template <typename KK, typename VV>
bool operator > (const Pair<KK,VV> &first, const Pair<KK,VV> &second){
    if(first.key > second.key){
        return true;
    }
    else{
        return false;
    }
}

//=============================================================================================
// Function: Overloaded comparison operator
// Purpose: Btree is sorted, so we need Pair to know how to sort itself
//=============================================================================================
template <typename KK, typename VV>
bool operator >= (const Pair<KK,VV> &first, const Pair<KK,VV> &second){
    if(first.value >= second.value){
        return true;
    }
    else{
        return false;
    }
}

//=============================================================================================
// Function: Overloaded equality operator
// Purpose: Btree is sorted, so we need Pair to know how to sort itself
//=============================================================================================
template <typename KK, typename VV>
bool operator ==(const Pair<KK,VV> &left, const Pair<KK,VV> &right){
    //cout<<endl<<"Pair == op"<<endl;
    if(left.key == right.key){
        return true;
    }
    else{
        return false;
    }
}

#endif // PAIR_H

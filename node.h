#ifndef NODE_H
#define NODE_H
#include "const.h"
#include "pair.h"
#include "record.h"

//===============================================
// Struct: Tree Node
// Purpose: Created our own tree node data structure
//          To be used in our B-Tree
//===============================================
struct tnode{
    tnode();
    tnode(string n);

    string name;
    tnode* child[T_MAX];
    int child_count;
};
tnode::tnode(){
    name = "UNKNOWN";
    child_count = 0;
}
tnode::tnode(string n){
    name = n;
}


#endif // NODE_H

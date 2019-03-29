#ifndef INDEX_H
#define INDEX_H
#include "btree.h"
#include "pair.h"

//=============================================================================================
// Class: Index
// This class creates an index structure with a string and a key (record number)
// (position in the array of tables in the Table Class.) Each time we insert
// into our index tree, we insert into the Record into the Records array
//=============================================================================================
class Index{
private:
    Tree<Pair<string, long> > indexTree;
    string fieldname;
public:
    Index();
    Index(const string &field);

    void insert(const string &name, long key);
    vector<long> find(string findMe);
    //vector<long> get_indices(string findMe);
    //vector<long> findOp(string find, string op);
    void print();
};

//=============================================================================================
// Function: Index
// Purpose: Default Constructor
//=============================================================================================
Index::Index(){
    //empty
}

//=============================================================================================
// Function: Index
// Purpose: Second Constructor that takes in a string (which is a field, like First Name)
//=============================================================================================
Index::Index(const string &field){
    fieldname = field;
}

//=============================================================================================
// Function: insert
// Purpose: Inserts field along with the record number into our index tree
//=============================================================================================
void Index::insert(const string &name, long key){
    //cout<<endl<<"index insert: "<<key<<endl;
    Pair<string, long> p(name, key);
    if(indexTree.Search(p)){
        //cout<<endl<<"duplicate";
        indexTree.Get(p).value.push_back(key);
    }
    else{
        //cout<<endl<<"no duplicates";
        indexTree.Insert(p);
    }
}

//=============================================================================================
// Function: Find
// Purpose: Finds the field in the tree, returns the record number
//=============================================================================================
vector<long> Index::find(string findMe){
    Pair<string, long> p(findMe, {0});
    if(indexTree.Search(p)){
        p = indexTree.Get(p);
        return p.value;
    }
    else{
        cout<<endl<<"Record not found"<<endl;
        return vector<long>();
    }
}

//=============================================================================================
// Function: Print
// Purpose: Prints the index tree
//=============================================================================================
void Index::print(){
    indexTree.testPrint(0);
}



#endif // INDEX_H

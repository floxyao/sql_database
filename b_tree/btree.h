#ifndef BTREE_H
#define BTREE_H
#include "../b_tree/tree_const.h"
#include "../b_tree/util.h"


//=========================================================================================
//Class:    Tree
//Purpose:  Self-implemented B-Tree.
//          *NEEDS IMPROVEMENT*
//          Variables are all public which was not an intended design choice.
//=========================================================================================
template <class T>
class Tree{

public:
    //This should be private.
    //-----------------------------------------
    T data[MAX+1]; //data[2] right now
    Tree<T>* subtree[MAX+2]; //subtree[3]
    int child_count;
    int data_count;
    int size;
    //-----------------------------------------

    Tree();
    
    //Big Three
    //-----------------------------------------
    ~Tree<T>();
    Tree<T> &operator = (Tree<T> &other);
    Tree<T>(const Tree<T> &other);
    //-----------------------------------------

    void clear_root();
    void copy_subtree(Tree<T>* dest[], Tree<T>* src[]);
    void clear_tree();
    int getsize();
    bool isLeaf();
    bool Insert(const T& item);
    bool Remove(const T& item);
    bool Search(const T& item);
    T& Get(T item);
    bool loose_insert(const T& item);
    bool loose_remove(const T& item);
    void fix_excess(int childNum);
    void fix_shortage(int childNum);
    void remove_biggest(T& item);
    void Print(); //correct way
    void testPrint(int indent); //book way
};

template <class T>
Tree<T>::Tree(){
    //cout<<"Tree::Tree() Fired"<<endl;
    data_count=0;
    child_count=0;
    size =0;
    for(int i=0; i<data_count; i++){
        data[i]=T();
    }
    for(int i=0; i<child_count; i++){
        subtree[i]=NULL;
    }
}

template <class T>
int Tree<T>::getsize(){
    return size;
}

template <class T>
void Tree<T>::copy_subtree(Tree<T>* dest[], Tree<T> *src[]){
    for(int i=0; i<child_count; i++){
        Tree<T>* temp = src[i];
        dest[i]=temp;
    }
}

template <class T>
Tree<T>::Tree(const Tree<T> &other){
    //cout<<"Copy ctor: child count "<<child_count;
    //cout<<endl<<"data_count: "<<data_count<<endl;
    child_count=other.child_count;
    data_count=other.data_count;
    _copy(data, other.data, data_count);
    _copy(subtree, other.subtree, child_count);
}

template <class T>
void Tree<T>::clear_tree(){
    if(!isLeaf()){
        //if you have children
        for(int i=0; i<child_count; i++){
            subtree[i]->clear_tree();
        }
    }
//    for(int i=0; i<data_count; i++){
//        //cout<<"clearing data"<<endl;
//        data[i]=T();
//    }
//    for(int i=0; i<child_count; i++){
//        //cout<<"clearing subtrees"<<endl;
//        delete subtree[i];
//    }
    data_count = 0;
    child_count = 0;
}

template <class T>
Tree<T>::~Tree(){
    //cout<<endl<<"Tree Destructor"<<endl;
    if(subtree){
        clear_tree();
    }
}

template <class T>
Tree<T> &Tree<T>::operator = (Tree<T> &other){
    if(subtree[0]){
        clear_tree();
        subtree[0]=NULL;
    }
    if(other.subtree[0]){
        //return Tree<T>(other.subtree);
    }
}

template <class T>
void Tree<T>::clear_root(){
    data_count = 0;
    for(int i=0; i<child_count; i++){
        //cout<<"clearing subtrees"<<endl;
        subtree[i]=NULL;
    }
    child_count = MIN;
}


template <class T>
bool Tree<T>::isLeaf(){
    return child_count==0;
}

template <class T>
bool Tree<T>::Search(const T &item){
    //cout<<endl<<"Search item is: "<<item<<endl;
    int i = _notLessThan(data, data_count, item);
    //cout<<endl<<"i: "<<i<<endl;
    //cout<<endl<<"data count: "<<data_count;

    if((i < data_count) && !(item < data[i])){
        //cout<<endl<<"Search: found"<<endl;
        //if(data[i])
        return true;
    }
    else if(isLeaf()){
        //cout<<"Search: isLeaf, not found"<<endl;
        return false; //no children not found
    }
    else{
        //cout<<"Recursive Search to the next"<<endl;
        return subtree[i]->Search(item);
    }
}

//This function assumes that the item exists
template <class T>
T& Tree<T>::Get(T item){
    int i = _notLessThan(data, data_count, item);
    if((i < data_count) && !(item < data[i])){
        //cout<<endl<<"Search: found"<<endl;
        return data[i];
    }
    else{
        //cout<<"Recursive Search to the next"<<endl;
        return subtree[i]->Get(item);
    }
}

template <class T>
bool Tree<T>::Remove(const T& item){
    if(!loose_remove(item)){
        //cout<<endl<<"removal fail";
        return false;
    }
    cout<<endl<<"removal success";
    if(data_count == 0 && child_count == 1){
        //fix root
    }
}

template <class T>
bool Tree<T>::loose_remove(const T& item){
    int i = _notLessThan(data, data_count, item);
    //cout<<endl<<"i is "<<i;
    if(i<data_count){
        //cout<<endl<<"loose_remove: i is less than data count"<<endl;
        if(data[i] == item){
            //check to see if that  is the item
            //if it is then check if its a leaf

            if(isLeaf()){
                //cout<<endl<<"is leaf, deleting"<<endl;
                _delete(data, data_count, item);
            }
            else{
                //cout<<endl<<"data[i] : "<<data[i]<<endl;
                subtree[i]->remove_biggest(data[i]);
                //cout<<endl<<data[i]<<"after removing biggest : "<<data[i]<<endl;
                if(subtree[i]->data_count < MIN){
                    //cout<<endl<<"OUTSIDE shortage in subtree["<<i<<"]"<<endl;
                    fix_shortage(i);
                }
            }
            return true;
        }
        else{
            subtree[i]->loose_remove(item);
        }
    }
    else{
        //cout<<endl<<"RECURSIVE CALL WITH subtree["<<i<<"]"<<endl;
        subtree[i]->loose_remove(item);
        return false;
    }
}

template <class T>
void Tree<T>::remove_biggest(T& item){
    if(this->isLeaf()){
        item = data[data_count-1];
        //cout<<endl<<"item is: "<<item<<endl;
        data_count--;
    }
    else{
        subtree[child_count-1]->remove_biggest(item);
    }
}

//i check childNum count because i dont want to check
//in a spot that is out of bounds in the subtree array
//if childNum is MAX then I know to check to the left
//if childNUM is less than MAX then I can check the right
template <class T>
void Tree<T>::fix_shortage(int childNum){
    if( (childNum < MAX) && (subtree[childNum+1]->data_count >= MIN+1) ){
        //if problem child has a right sibling to borrow from
        cout<<"Case 1: Borrow from right sibling"<<endl;
        subtree[childNum]->data[0]=data[childNum];
        //move parent down to the node that will be deleted

        data[childNum] = subtree[childNum+1]->data[0];
        //replace that parent with the right siblings first entry

        _delete(subtree[childNum+1]->data, subtree[childNum+1]->data_count, data[childNum]);
        //delete that right siblings first entry "borrowed"

    }
    else if( (childNum == MAX) && (subtree[childNum-1]->data_count >= MIN+1)){
        //if problem child has a left sibling to borrow from
        cout<<"Case 2: Borrow from left sibling"<<endl;
        subtree[childNum]->data[0] = data[childNum-1];
        data[childNum-1] = subtree[childNum-1]->data[MIN];
        _delete(subtree[childNum-1]->data, subtree[childNum-1]->data_count, data[childNum-1]);
    }
    else if( (childNum < MAX) && (subtree[childNum+1]->data) ){
        //if problem child has a right sibling that you can't borrow from
        cout<<"Case 3: Right Sibling"<<endl;

        subtree[childNum]->data[0] = data[childNum];
        //parent moves down to the node that you're deleting

        _delete(data, data_count, data[childNum]);
        //delete that parent from parent array

        _merge(subtree[childNum]->data, subtree[childNum]->data_count,
                subtree[childNum+1]->data, subtree[childNum+1]->data_count);
    }
    else if( (childNum == MAX) && (subtree[childNum-1]->data) ){
        //if problem child has a left sibling that you can't borrow from
        cout<<"Case 4: Left sibling"<<endl;
        subtree[childNum]->data[0] = data[childNum];
        //parent moves down to the node that you're deleting

        _delete(data, data_count, data[childNum-1]);
        //delete that parent from parent array

        _merge(subtree[childNum-1]->data, subtree[childNum-1]->data_count,
                subtree[childNum]->data, subtree[childNum]->data_count);
        //merge the two arrays
    }
}


template <class T>
bool Tree<T>::Insert(const T &item){
    if(!loose_insert(item)){
        return false;
    }
    if(data_count > MAX){
        Tree<T>* newCopy = new Tree<T>(*this);
        //make copy of current tree

        //cout<<endl<<"Printing new copy: "<<endl;
        //newCopy->testPrint(0);

        clear_root();
        //clear current tree

        subtree[0] = newCopy;
        //in the cleared Tree, subtree[0] points to the copy
        //_print(subtree[0]->data, subtree[0]->data_count);

        fix_excess(0);
        //call fix_excess on itself

    }
    return true;

}

template <class T>
void Tree<T>::fix_excess(int childNum){
    //cout<<endl<<"problem with subtree["<<childNum<<"]: ";
    //_print(subtree[childNum]->data, subtree[childNum]->data_count);

    _insert_sorted(data, data_count, subtree[childNum]->data[MIN]);
    //insert middle child into empty array you just created

    Tree<T>* left = new Tree<T>();
    Tree<T>* right = new Tree<T>();

    int other_count=(subtree[childNum]->data_count)/2;
    //cout<<"other count: "<<other_count;
    T *other = new T[other_count];
    //create other array for split with appropriate size

//    cout<<endl<<"main: ";
//    _print(subtree[childNum]->data, subtree[childNum]->data_count);
//    cout<<endl<<endl<<"other: ";
//    _print(other, other_count);
//    cout<<endl<<endl;

    _split(subtree[childNum]->data, subtree[childNum]->data_count, other, other_count);
    //split the arrays

    //cout<<endl<<"after splitting"<<endl;
    //_print(subtree[childNum]->data, subtree[childNum]->data_count);

    //cout<<endl<<"copy left "<<endl;
    _copy(left->data, subtree[childNum]->data, subtree[childNum]->data_count);
    //_print(left->data, subtree[childNum]->data_count);

    //cout<<endl<<"copy right"<<endl;
    _copy(right->data, other, other_count);
    //_print(right->data, other_count);

    left->data_count=MIN;
    right->data_count=MIN;

    //cout<<endl<<"endendend"<<endl;
    //_print(subtree[childNum]->data, subtree[childNum]->data_count);
    delete[] other;

    if(!subtree[childNum]->isLeaf()){
        //cout<<"childnum not leaf"<<childNum;
        int subtree_count = (subtree[childNum]->child_count)/2;
        Tree<T>* s[subtree_count];

        _split(subtree[childNum]->subtree, subtree[childNum]->child_count, s, subtree_count);
        _copy(left->subtree, subtree[childNum]->subtree, subtree[childNum]->child_count);
        _copy(right->subtree, s, subtree_count);
        left->child_count=MIN+1;
        right->child_count=MIN+1;
    }

    _shift_right(subtree, child_count, childNum+1);
    subtree[childNum]=left;
    subtree[childNum+1]=right;
}

template <class T>
bool Tree<T>::loose_insert(const T &item){
    for(int i=0; i<data_count; i++){
        //check for duplicate entries in data array first
        if(data[i] == item){
            //cout<<endl<<"loose_insert there is a dup"<<endl;
            return false;
        }
    }
    int i = _notLessThan(data, data_count, item);
    //cout<<endl<<"_not less than i : "<<i<<endl;
    if(isLeaf()){
        _insert_sorted(data, data_count, item);
        //cout<<endl<<"just _insert sorted"<<endl;
    }
    else{
        //i is greater than data_count, so it must be in the next level
        subtree[i]->loose_insert(item);
        if(subtree[i]->data_count > MAX){
            fix_excess(i);
        }
    }
    //cout<<endl<<"returning true"<<endl;
    size++;
    return true;
    //fix head root at the end
}

template <class T>
void Tree<T>::testPrint(int indent){
    int extra_indent = 4;
    int i;

    cout << setw(indent) << " ";

    for(i=0; i<data_count; ++i){
        cout<< data[i]<<" ";
        cout<<" "<<endl;
    }

    for(i=0; i<child_count; ++i){
        subtree[i]->testPrint(indent+extra_indent);
    }
}

#endif // BTREE_H

#ifndef UTIL_H
#define UTIL_H
#include "../b_tree/btree.h"

template <class T>
void _init(T a[], int numUsed){
    for(int i=0; i<numUsed; i++){
        a[i]=NULL;
    }
}

template <class T>
void _copy(T dest[], const T src[], int numUsed){
    for(int i=0; i<numUsed; i++){
        dest[i]=src[i];
    }
}

template <class T>
int _notLessThan(T a[], int numUsed, T item){
    for(int i=0; i<numUsed; i++){
        if(item <= a[i]){
            //cout<<a[i]<<endl;
            //cout<<endl<<"returning i = "<<i;
            return i;
        }
    }
    //cout<<endl<<"returning numUsed = "<<numUsed;
    return numUsed;
}

template <class T>
void _split(T a[], int &aSize, T b[], int &bSize){
    //cout<<endl<<"asize: "<<aSize<<endl;
    for(int i=0, j=aSize/2; i<bSize; i++, j++){
        if(aSize%2==0){
            b[i]=a[j];
        }
        else{
            b[i]=a[j+1];
        }
    }
    aSize=aSize - (aSize/2);
}

template <class T>
void _merge(T a[], int &aSize, T b[], int &bSize){
    for(int i=aSize, j=0; i<aSize+bSize; i++, j++){
        a[i]=b[j];
    }
    aSize+=bSize;
    bSize=0;
}

template <class T>
void _shift_right(T a[], int &numUsed, int shiftHere){
    //numUsed++;
    //cout<<endl<<"numUsed: "<<numUsed;
    for(int i=numUsed; i>shiftHere; i--){
        a[i] = a[i-1];
    }
    numUsed++;
}

template <class T>
void _shift_left(T a[], int &numUsed, int shiftHere){
    if(shiftHere >= numUsed-1){
        numUsed--;
    }
    else{
        for(int i=shiftHere; i<numUsed; i++){
            a[i] = a[i+1];
        }
        numUsed--;
    }
}

template <class T>
void _insert_sorted(T a[], int &numUsed, const T &item){
    for(int i=0; i<numUsed; i++){
        if(a[i] > item){
            //shift right at i
            //insert at i
            //cout<<endl<<"inserted at index "<<i<<endl;
            _shift_right(a, numUsed, i);
            a[i]=item;
            return;
        }
    }
   // cout<<endl<<"_ins_sort last item inserted"<<endl;
    numUsed++;
    a[numUsed-1]=item;
    //cout<<"numused : "<<numUsed;
}

template <class T>
int _delete(T a[], int &numUsed, const T &item){
    for(int i=0; i<numUsed; i++){
        if(a[i] == item){
            _shift_left(a, numUsed, i);
        }

    }
    return item;
}

template <class T>
void _print(T a[], int numUsed){
    //cout<<endl<<"numused :"<<numUsed<<endl;
    for(int i=0; i<numUsed; i++){
        cout<<a[i]<<" ";
    }
}

template <class T>
void _printS(T a[], int numUsed){
    //cout<<endl<<"numused :"<<numUsed<<endl;
    for(int i=0; i<numUsed; i++){
        cout<<*a[i]<<" ";
    }
}

#endif // UTIL_H

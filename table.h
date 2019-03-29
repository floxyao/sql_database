#ifndef TABLE_H
#define TABLE_H
#include "record.h"
#include "index.h"
#include <fstream>
using namespace std;

//=========================================================================================
//Class:    Table
//Purpose:  This table class has stores and manages records. Contains the table name,
//          the number of records, a vector of the records it contains, e.g. Student
//          Table will contain records of students.
//=========================================================================================
struct Table{
private:
    string _tablename;
    vector<string> _columns; //Fname Lname Age
    vector<Record> _records;
    long last_rec;
    int num_columns;
    int num_records;

public:
    Table();
    Table(string tablename);
    Table(string tablename, vector<string> columns);
    vector<Index> indices;
    void buildIndexTrees();
    void addToIndexTree(const Record &r);
    void create();
    void load();

    string getTableName();
    Record getRecord(int index);
    vector<long> searchRecords(vector<int> col_indices, vector<string> str);
    string column_names(int index);
    int insert(const Record &r);
    int num_of_records();
    int num_of_columns();
    long get_last_rec();
    void set_num_columns(int num);
    void set_tablename(string name);
    void set_columns(vector<string> col);
    void print();
};

//=============================================================================================
// Function: Table
// Purpose: Default constructor
//=============================================================================================
Table::Table(){
    //def
    _tablename = ' ';
    last_rec = 0;
    num_columns = 0;
    num_records = 0;
}

//=============================================================================================
// Function: Table
// Purpose: Second constructor, takes in table name as input
//=============================================================================================
Table::Table(string tablename){
    _tablename = tablename;
    num_columns = _columns.size();
    num_records = 0;
    last_rec = 0;
}

//=============================================================================================
// Function: Table
// Purpose: Third constructor, take in table name and columns (# fields)
//=============================================================================================
Table::Table(string tablename, vector<string> columns){
    //def
    _tablename = tablename;
    _columns = columns;
    num_columns = _columns.size();
    num_records = 0;
    last_rec = 0;
    buildIndexTrees();
}

//=============================================================================================
// Function: get_last_rec
// Purpose: getter, returns the last record inserted
//=============================================================================================
long Table::get_last_rec(){
    return last_rec;
}

//=============================================================================================
// Function: set_tablename
// Purpose: setter
//=============================================================================================
void Table::set_tablename(string name){
    _tablename = name;
}

//=============================================================================================
// Function: set_columns
// Purpose: setter
//=============================================================================================
void Table::set_columns(vector<string> col){
    _columns = col;
}

//=============================================================================================
// Function: getTableName
// Purpose: getter
//=============================================================================================
string Table::getTableName(){
    return _tablename;
}

//=============================================================================================
// Function: getRecord
// Purpose: getter
//=============================================================================================
Record Table::getRecord(int index){
    return _records[index];
}

//=============================================================================================
// Function: searchRecords
// Purpose: returns all records associated with fields that are inputted
//=============================================================================================
vector<long> Table::searchRecords(vector<int> col_indices, vector<string> str){
    cout<<"size of indices: "<<indices.size();
    //indices is a vector of Index objects (index tree)
    //So it's telling that Index tree to find this string ('Silvia', 'Bob', 'Michael')
    return indices[col_indices[0]].find(str[0]);
}

//=============================================================================================
// Function: column_names
// Purpose: getter
//=============================================================================================
string Table::column_names(int index){
    return _columns[index];
}

//=============================================================================================
// Function: addToIndexTree
// Purpose: insert records into index tree
//=============================================================================================
void Table::addToIndexTree(const Record &r){
    for(int i = 0; i<indices.size(); i++){
        indices[i].insert(r.get_field(i), r.rec_number());
        //cout<<endl<<"Table::addToIndexTree: Just inserted ";
        //cout<<r.get_field(i)<< "rec#: "<<r.rec_number();
    }
}

//=============================================================================================
// Function: buildIndexTrees
// Purpose: insert record numbers into indices vector
//=============================================================================================
void Table::buildIndexTrees(){
    for(int i=0; i<num_columns; i++){
        indices.push_back(Index(_columns[i]));
        //cout<<endl<<"Table::buildIndexTrees: Pushed "
         //   <<_columns[i]<<" tree"<<endl;
    }
}

//=============================================================================================
// Function: set_num_columns
// Purpose: setter
//=============================================================================================
void Table::set_num_columns(int num){
    num_columns = num;
    //cout<<"num: "<<num_columns;
}

//=============================================================================================
// Function: num_of_columns
// Purpose: getter
//=============================================================================================
int Table::num_of_columns(){
    return num_columns;
}

//=============================================================================================
// Function: num_of_records
// Purpose: getter
//=============================================================================================
int Table::num_of_records(){
    return _records.size();
}

//=============================================================================================
// Function: insert
// Purpose: Inserts record into index tree.
//          once pushing back this returns the index
//          to the index of this array that you pushed in
//=============================================================================================
int Table::insert(const Record &r){
    _records.push_back(r);
    addToIndexTree(r);
    last_rec=_records.size()-1; //position in array
    num_records++; //size of records = num records
    //return num_records;
    cout << "Table::insert - returning 0" <<endl;
    return 0;
}

//=============================================================================================
// Function: print
// Purpose: Simple print function for table
//=============================================================================================
void Table::print(){
    cout<<endl<<endl<<"----------------------"<<endl;
    cout<<"Table: "<<_tablename<<endl;
    for(int i=0; i<num_columns; i++){
        //cout<<setw(GAP)<<_columns[i];
        cout<<_columns[i]<<" ";
    }
    cout<<endl;

    for(int i=0; i<num_records; i++){
        cout<<_records[i];
        //_records[i].print();
    }
    cout<<"----------------------";
}


#endif // TABLE_H

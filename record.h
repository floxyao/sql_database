#ifndef RECORD_H
#define RECORD_H
#include "const.h"
#include "map.h"

//'Flo' 'Yao' '23' 'CS'
//=========================================================================================
//Class:    Record
//Purpose:  This record class has 2D array of fields (Fname Lname Age), number of fields,
//          and record number. So we can easily access this information when storing,
//          retrieving, searching for records in our SQL database.
//=========================================================================================
class Record{
private:
    char fields[REC_MAX][REC_MAX];
    long record_number;
    int _numfields;

public:
    Record();
    Record(vector<string> fieldlist);
    Record(vector<string> fieldlist, long rec_no);
    friend ostream& operator << (ostream& out, const Record &r);
    void print()const;
    long rec_number()const;
    void set_record_num(long num);
    void fill_record(string nameOfTable);
    string get_field(int index)const;

};

//=============================================================================================
// Function: Record
// Purpose: Default constructor. Initializes fields array, record number
//=============================================================================================
Record::Record(){
    //default
    for(int i=0; i<REC_MAX; i++){
        for(int j=0; j<REC_MAX; j++){
            fields[i][j]=' ';
        }
    }
    record_number = 0;
}

//=============================================================================================
// Function: Record
// Purpose: Second constructor, takes in a field list and lets its own private variable
//          to the inputted field list.
//=============================================================================================
Record::Record(vector<string> fieldlist){
    _numfields = fieldlist.size();
    for(int i=0; i<_numfields; i++){
        string name = fieldlist[i];
        strcpy(fields[i], name.c_str());
    }
}

//=============================================================================================
// Function: Record
// Purpose: Third constructor, takes in a field list, and record number.
//=============================================================================================
Record::Record(vector<string> fieldlist, long rec_no){
    _numfields = fieldlist.size();
    for(int i=0; i<_numfields; i++){
        string name = fieldlist[i];
        strcpy(fields[i], name.c_str());
    }
    record_number = rec_no;
}

//=============================================================================================
// Function: get_field
// Purpose: getter; returns the index to the field. E.G. Fname Lname Age - Age is index 2
//=============================================================================================
string Record::get_field(int index)const{
    return fields[index];
}

//=============================================================================================
// Function: set_record_num
// Purpose: setter
//=============================================================================================
void Record::set_record_num(long num){
    record_number = num;
}

//=============================================================================================
// Function: rec_number
// Purpose: getter; returns the record number.
//=============================================================================================
long Record::rec_number()const{
    return record_number;
}

//=============================================================================================
// Function: Overloaded binary operator
// Purpose: To make printing Records easy with cout
//=============================================================================================
ostream& operator << (ostream& out, const Record &r){
    //int size = r.fields
    for(int i=0; i<r._numfields; i++){
        out << r.fields[i] << endl;
    }
    return out;
}

void Record::print() const{
//    int size = _fields.size();
//    for(int i=0; i<size; i++){
//        cout<<setw(GAP)<<_fields[i];
//    }
//    for(int i=0; i<3; i++){
//        cout<<fields[0][i];
//    }
}


#endif // RECORD_H

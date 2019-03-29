#ifndef SQL_H
#define SQL_H
#include "table.h"
#include <fstream>
#include "command_funcs.h"
#include "sqlparser.h"
#include <iostream>
using namespace std;

//=========================================================================================
//Class:    SQL
//Purpose:  Performs the appropriate SQL command entered by the user.
//          SELECT: Selects fields from index trees and writes it to file
//          MAKE TABLE: Creates a new file with specified fields
//          INSERT INTO: Inserts fields into a given table name
//=========================================================================================
class SQL{
private:
    vector<Table> table_list;
    vector<string> table_strings;
    int num_tables;
public:
    SQL();

    void addTable(Table &table);
    void insertData(Table &insertInto, vector<Record> records, vector<int> field_indices);
    
    vector<int> getColumnIndex(Table &table, vector<string> column);
    vector<int> getFieldIndex(Table &table, vector<string> listOfFields);
    vector<Record> getRecords(Table &table, vector<long> rec_numbers);
    vector<Record> getRecords(Table &table);
    
    Table &getTable(string table_name);
    Table Insert(string tablename, vector<string> values);
    Table MakeTable(string tablename, vector<string> columns);
    Table interpret(Map<string, string> parsetree);
    Table Select(vector<string> listOfFields,
                      string tablename,
                      vector<string> column, vector<string> str);
    void run();
    int size();
    bool tableExists(string tablename);
    void load();
    void writeToFile(Table t);
    void makeFile(Table t);
    void print();
    bool has_only_spaces(const string& str);
};

//=========================================================================================
//Function:  SQL
//Purpose:  This is the default constructor. It loads all table names from file
//          and pushes it into a vector of table names to be referred to in the
//          SQL class.
//=========================================================================================
SQL::SQL(){
    cout<<endl<<"SQL::SQL() Fired"<<endl;
    load();

    cout<<endl<<"SQL() Load Success... Printing tables: "<<endl;
}

//=========================================================================================
//Function: Print
//Purpose:  Prints all Table objects
//=========================================================================================
void SQL::print(){
    for(int i=0; i<table_list.size(); i++){
        table_list[i].print(); cout<<endl;
    }
}

//*========================================================================================
//Function: tableExists
//Purpose:  Checks if the specified table exists
//Returns:  True if it exists, else false
//=========================================================================================
bool SQL::tableExists(string tablename){
    for(int i=0; i<table_list.size(); i++){
        cout<<table_strings[i]<<" ";
        if(tablename == table_strings[i]){
            return true;
        }
    }
    return false;
}

bool SQL::has_only_spaces(const string& str) {
   return str.find_first_not_of (' ') == str.npos;
}

//*========================================================================================
//Function: Load
//Purpose:  Loads all table names from file and pushes it into a vector of table names
//Pre-Cond: File exists
//Post-Cond: All table names, fields, records pushed into appropiate vectors
//=========================================================================================
void SQL::load(){
    ifstream tablefile;
    tablefile.open("tablefile.txt");
    if(tablefile.fail()){
        throw;
    }
    else{
        while(!tablefile.eof()){
            string tablename;
            getline(tablefile, tablename);
            if(tablefile){
                table_strings.push_back(tablename);
                table_list.push_back(Table(tablename));
            }
        }
    }
    tablefile.close();

    for(int i=0; i<table_list.size(); i++){
        ifstream recordfile;
        vector<string> fieldlist;
        string num_fields_str;
        string column_str;
        long rec_num=0;

        cout<<"table_strings |"<<table_strings[i]<<"|"<<endl;
        
        recordfile.open(table_strings[i]+".txt");
        getline(recordfile, num_fields_str);
        
        
        cout<< endl << "num fields: |"<<num_fields_str<<"|"<<endl;
        
        
        int num_fields = stoi(num_fields_str);
        table_list[i].set_num_columns(num_fields);

        for(int k=0; k<num_fields; k++){
            getline(recordfile, column_str);
            fieldlist.push_back(column_str); //Fname Lname Age
        }

        table_list[i].set_columns(fieldlist);
        table_list[i].buildIndexTrees();

        string fieldString;
        while(!recordfile.eof()){
            vector<string> data;
            for(int i=0; i<num_fields; i++){
                getline(recordfile, fieldString);
                if(!has_only_spaces(fieldString)){
                    data.push_back(fieldString);
                }
            }
            if(data.size()>0){
                Record new_record(data, rec_num);
                table_list[i].insert(new_record);
                rec_num++;
            }
//            rec_num++;
//            Record new_record(data, rec_num);
//            cout<<endl<<"New record: "<<endl<<new_record;
//            cout<<" rec#: "<<rec_num<<", inserting into table_list["<<i<<"] / "<<table_list.size()<<endl;
//            table_list[i].insert(new_record);
       }
        recordfile.close();
    }
}

//*========================================================================================
//Function: Select
//Purpose:  Selects all records, creates and writes those records into a new file
//Pre-Cond: Non empty list of fields, table name, and column and string vector
//Post-Cond: New table is populated with selected fields
//=========================================================================================
Table SQL::Select(vector<string> listOfFields,
                  string tablename,
                  vector<string> column,
                  vector<string> str){
    cout<<endl<<"SQL::Select entered"<<endl;

    // create the new table to return 
    Table newTable("New Table", listOfFields);
    Table table = getTable(tablename);

    // debug statements
    cout<<endl<<endl<<"Got table: "<<table.getTableName()<<" ";
    cout<<"table num of col: "<<table.num_of_columns(); cout<<endl;
    cout<<"table fields"<<table.column_names(0); cout<<endl;

    if(tableExists(tablename)){
        if(!column.empty() && !str.empty()){
            // there's a WHERE clause
            vector<int> column_indices = getColumnIndex(table, column);
            // getColumnIndex pushes
            // Fname = column 0
            // Age = column 2
            // Give this vector to table along with string literal to Table

            vector<int> field_indices = getFieldIndex(table, listOfFields);
            // SELECT Fname,Lname <-- these are the fields
            // gets all the index numbers corresponding to the fields
            
            for(int i = 0; i<field_indices.size(); i++){
                cout<<"field_indices: "<<field_indices[i]<<" ";
            }
            cout<<endl<<"after get field index"<<endl;
            //cout<<table.indices.size();

            vector<long> rec_nums = table.searchRecords(column_indices, str);
            // pass searchRecords the column indices, it'll take those
            // positions and directly go to the appropriate Index tree
            // located in that index, and find the record associated
            // with the string literal, returns vector<long> of all records
            
            cout<<endl<<"after search records"<<endl;
            for(int i=0; i<rec_nums.size(); i++){
                cout<<"rec nums: "<<rec_nums[i]<<" ";
            }

            vector<Record> records = getRecords(table, rec_nums);
            //gets the records cause we have the tablefile locations for them

            insertData(newTable, records, field_indices);

            newTable.print();
            return newTable;
        }
        else{
            cout<<endl<<"No where condition"<<endl;
            vector<int> field_indices = getFieldIndex(table, listOfFields);

            vector<Record> records = getRecords(table);

            insertData(newTable, records, field_indices);

            newTable.print();
            return newTable;
        }
    }
    else{
        cout<<"SQL::Select: Table doesn't exist"<<endl;
        return Table();
    }
}

//*========================================================================================
//Function: Make Table
//Purpose:  Creates a new table, writes the new table name and fields into file
//Pre-Cond: Table is not a duplicate
//Post-Cond: New table is written to file, new table object is pushed into table class
//Return(s): New table object, or NULL table if failed
//=========================================================================================
Table SQL::MakeTable(string tablename, vector<string> columns){
    Table newTable(tablename, columns);
    addTable(newTable);
    cout<<"SQL::MakeTable: Added new table: ";
    newTable.print();

    makeFile(newTable);
    return newTable;
}

//*========================================================================================
//Function: Insert
//Purpose:  Inserts fields into an existing table
//Pre-Cond: Table exists
//Post-Cond: New fields are updated into the table class and the table list on disk.
//Return(s): The table object you inserted into, or NULL table if failed
//=========================================================================================
Table SQL::Insert(string tablename, vector<string> values){
    Table t = getTable(tablename);
    cout<<endl<<"SQL::Insert: Table t num records = "<<t.num_of_records();
    cout<<endl<<"SQL::Insert: Table t num col = "<<t.num_of_columns();
    cout<<endl;

    if((values.size()==t.num_of_columns())){
        Record insertMe(values, t.num_of_records()+1);
        t.insert(insertMe); // insert into object
        writeToFile(t); // insert into file
        t.print(); // check output
        return t;
    }
    else{
        cout<<endl<<"SQL::Insert: Error"<<endl;
        return Table();
    }
}

//========================================================================================
//Function: Make File
//Purpose:  Creates a new table file on disk
//Pre-Cond: Table name is valid
//Post-Cond: Table file is successfully created with appropriate fields
//Return(s): None
//=========================================================================================
void SQL::makeFile(Table t){
    ifstream f;
    f.open(t.getTableName()+".txt");
    if(!f.fail()){
        cout<<endl<<"SQL::makeFile: Table already exists!"<<endl;
    }
    else{
        cout<<endl<<"SQL::makeFile: Creating table"<<endl;
        ofstream tablelist;

        tablelist.open("tablefile.txt");
        if(tablelist.fail()) throw;
        for(int i=0; i<table_list.size(); i++){
            tablelist << table_list[i].getTableName() << endl;
        }
        ofstream table;
        table.open(t.getTableName()+".txt");
        if(table.fail()) throw;
        string num_col_str = to_string(t.num_of_columns());
        table << num_col_str << endl;

        for(int i=0; i<t.num_of_columns(); i++){
            table << t.column_names(i) << endl;
        }
        tablelist.close();
        table.close();
    }
}

//========================================================================================
//Function: Write To File
//Purpose:  Writes a table name and their fields to file
//Pre-Cond: Table name is valid
//Post-Cond: Table file is successfully created with appropriate fields and columns
//Return(s): None
//=========================================================================================
void SQL::writeToFile(Table t){
    ofstream f;
    f.open(t.getTableName()+".txt");
    if(f.fail()) throw;

    f << t.num_of_columns() << endl;
    cout<<"pushing: "<<t.num_of_columns()<<endl;
    for(int i=0; i<t.num_of_columns(); i++){
        cout<<"pushing: "<<t.column_names(i)<<endl;
        f << t.column_names(i) << endl;
    }
    //cout<<endl<<"t num of records: "<<t.num_of_records();
    for(int i=0; i<t.num_of_records(); i++){
        //f << t.getRecord(i).get_field(j) <<endl;
        for(int j=0; j<t.num_of_columns(); j++){
            cout<<"pushing: "<<t.getRecord(i).get_field(j)<<endl;
            f << t.getRecord(i).get_field(j)<<endl;
        }
    }
    cout<<endl<<"SQL::writeToFile: closing file"<<endl;
    f.close();
}

//*========================================================================================
//Function: Run
//Purpose:  Runs the program; gets user input and makes a call to
//          set_string. Set_string returns true if command is valid
//=========================================================================================
void SQL::run(){
    char key = ' ';
    SQL_Parser p;

    // runs user-input in a loop until 'X' is entered
    do{
        print(); // just to check what my table looks like before i run commands
        
        // User input
        string command;
        cout<<endl<<">>";
        getline(cin, command);
        
        // Validate SQL command
        bool ok = p.set_string(command); // "splits" tokens and validates SQL command
        if (ok){                                                //using state machine
            cout<<"Main: Set_String Success"<<endl;
            interpret(p.getParseTree()); //interpret the resulting parse tree 
        }
        else {
            cout<<"INVALID COMMAND"<<endl;
        }
        
        // Reset for next input
        p.reset();
        SQL();
        cout<<endl<<"HIT ENTER TO TRY AGAIN"<<endl;

        cin.get();

    }while(key != 'x');

}

//*========================================================================================
//Function: interpret
//Purpose:  Interprets the parse tree and breaks it apart into its constituent fields
//          COMMAND: SELECT
//          FIELDS: Fname,Lname
//          COND: Yes
//          WHERE: Lname = 'Smith'
//          to pass into the appropriate functions: SELECT, MAKE, INSERT
//=========================================================================================
Table SQL::interpret(Map<string, string> parsetree){
    if(parsetree.get_values("COMMAND")[0] == "SELECT"){
        vector<string> fields = parsetree.get_values("Fields"); //Fname, Lname, Age
        vector<string> table = parsetree.get_values("TABLE"); //Table name
        vector<string> column; //COLUMN
        vector<string> str; //STRING
        
        //SELECT Fname,Lname FROM Students WHERE Fname = "Silvia"
        //  COMMAND  : SELECT
        //  Fields   : Fname,Lname
        //  STRING   : Silvia
        //  COLUMN   : Fname     (WHERE)
        //  OPERATOR : =
        //  TABLE    : Students

        //check if there is a WHERE CLAUSE
        if(parsetree.get_values("COLUMN").size()>0){
            column = parsetree.get_values("COLUMN");
            str = parsetree.get_values("STRING");
        }
        return Select(fields, table[0], column, str);
    }
    else if(parsetree.get_values("COMMAND")[0] == "INSERT"){
        //[COMMAND : INSERT] [TABLE : Students] [VALUES : FloYao23] good sql
        //INSERT INTO Students VALUES "Flo","Yao","23"
        vector<string> fields = parsetree.get_values("VALUES");
        vector<string> table = parsetree.get_values("TABLE");
        return Insert(table[0], fields);
    }
    else if(parsetree.get_values("COMMAND")[0] == "MAKE"){
        //[COMMAND : MAKE] [TABLE : Girls] [Fields : FnameLnameAge]
        //MAKE TABLE Girls FIELDS Fname,Lname,Age
        vector<string> fields = parsetree.get_values("Fields");
        vector<string> table = parsetree.get_values("TABLE");
        return MakeTable(table[0], fields);
    }else{
        return Table();
        
    }
}

//*========================================================================================
// Function: getColumnIndex
// Purpose: Retrieves conditions of the appropriate column
//          condition_index is the index of where the column is in our table columns
//          because our listOfFields won't always be ordered a certain way
//=========================================================================================
vector<int> SQL::getColumnIndex(Table &table, vector<string> column){
    vector<int> condition_index;
    int num_columns = column.size();
    for(int i=0; i<num_columns; i++){
        cout<<endl<<"SQL::getColumnIndex table.column_name["<<i<<"]: "<<table.column_names(i)<<endl;
        if(column[0] == table.column_names(i)){
            condition_index.push_back(i);
        }
    }
    return condition_index;
    cout<<"SQL::getColumnIndex: did not return anything"<<endl;
}

//*========================================================================================
// Function: getFieldIndex
// Purpose: Retrieves column of the appropriate fields
//=========================================================================================
vector<int> SQL::getFieldIndex(Table &table, vector<string> listOfFields){
    vector<int> column_indices;
    cout<<endl<<"SQL::getFieldIndex listOfFields: "<<listOfFields[0]<<endl;
    
    for(int i=0, j=0; i<table.num_of_columns(); i++){
        cout<<endl<<"SQL::getFieldIndex table.column_name["<<i<<"]: "<<table.column_names(i)<<endl;
        if(table.column_names(i) == listOfFields[j]){
            cout<<endl<<"SQL::getFieldIndex pushed i: "<<i<<endl;
            column_indices.push_back(i);
            j++;
        }
    }
    return column_indices;
}

//*========================================================================================
// Function: insertData
// Purpose: Retrieves columns of the appropriate fields
//          condition_index is the index of where the column is in our table columns
//          because our listOfFields won't always be ordered a certain way
//=========================================================================================
void SQL::insertData(Table &insertInto, vector<Record> records, vector<int> field_indices){
    int num_fields = field_indices.size();
    int num_of_records = records.size();

    for(int i=0; i<num_of_records; i++){
        vector<string> data;
        for(int j=0; j<num_fields; j++){
            string charToString = string(records[i].get_field(field_indices[j]));
            cout<<endl<<"SQL::insertData: Pushing: "<<charToString<<endl;
            data.push_back(charToString);
        }
        Record new_record(data, i);
        insertInto.insert(new_record);
    }
}

//*========================================================================================
// Function: getTable
// Purpose: Returns the table object of specified table name
//=========================================================================================
Table &SQL::getTable(string table_name){
    for(int i=0; i<table_list.size(); i++){
        if(table_name == table_list[i].getTableName()){
            cout<<endl<<"SQL::getTable: Returning table "<<table_list[i].getTableName();
            cout<<endl<<"SQL::getTable: this tables cols: "<<table_list[i].num_of_columns();
            return table_list[i];
        }
    }
    cout <<"SQL getTable - end of function"<<endl;
    return table_list[0]; //This should be looked at.
                          //  Perhaps an ERROR_TABLE that could be returned
                          // if table_name does not exist.
                          // or simply throw an exception. This would be better.
}

vector<Record> SQL::getRecords(Table &table){
    vector<Record> temp;
    int num_records = table.num_of_records();

    for(int i=0; i<num_records; i++){
        temp.push_back(table.getRecord(i));
    }
    return temp;
}

vector<Record> SQL::getRecords(Table &table, vector<long> rec_numbers){
    vector<Record> temp;
    int rec_num_size = rec_numbers.size();
    cout<<endl<<"table size: "<<table.num_of_records();

    cout<<endl<<"lets see whats in my table"<<endl;
    for(int i = 0; i<table.num_of_records(); i++){
        cout<<"record: "<<table.getRecord(i);
    }
    cout<<endl<<endl;
    for(int i=0; i<rec_num_size; i++){
        cout<<"record numbers: "<<rec_numbers[i]<<endl;
        cout<<"got record: "<<table.getRecord(rec_numbers[i]);
        temp.push_back(table.getRecord(rec_numbers[i]));
    }
    return temp;
}

void SQL::addTable(Table &table){
    table_list.push_back(table);
    num_tables++;
}

int SQL::size(){
    return table_list.size();
}



#endif // SQL_H

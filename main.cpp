#include <iostream>
#include "sql.h"
#include "index.h"
#include <cstring>
#include <fstream>
#include "stokenizer.h"
using namespace std;

void testTable();
void testSelect();
void testRecord();
void teststrCopy();
void testSQL();
void testToken();
void testParser();
void testMap();
void testRun();
void testTree();
void testIndexTrees();
void testfind();
void testFile();
void testInsert();

int main()
{
    cout<<"=====START====="<<endl<<endl;
    
    //teststrCopy();
    //testRecord();
    //testTable();
    //testSelect();
    //testSQL();
    //testToken();
    //testParser();
    //testMap();
    testRun();
    //testTree();
    //testfind();
    //testIndexTrees();
    //testFile();
    //testInsert();
    
    cout<<endl<<endl<<endl<<"======END======"<<endl;
    return 0;
}

void testInsert(){
    testFile();
}

void testFile(){
    vector<Table> table_list;
    vector<string> table_strings;
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
                cout<<"Pushed: "<<tablename<<endl;
            }
        }
    }
    tablefile.close();
    
    int num_tables = table_strings.size();
    for(int i=0; i<num_tables; i++){
        ifstream recordfile;
        vector<string> fieldlist;
        string num_fields_str;
        string column_str;
        long rec_num=0;
        
        cout<<"table name: "<<table_strings[i]<<endl;
        recordfile.open(table_strings[i]+".txt");
        getline(recordfile, num_fields_str);
        cout<<num_fields_str;
        
        for(int i=0;i<3;i++){
            getline(recordfile, column_str);
            cout<<"Pushed col: "<<column_str<<endl;
            fieldlist.push_back(column_str); //Fname Lname Age
        }
        
        cout << endl << "num_fields_str " << num_fields_str << endl;
        int num_fields = stoi(num_fields_str);
        string fieldString;
        
        while(!recordfile.eof()){
            vector<string> data;
            for(int i=0; i<num_fields; i++){
                getline(recordfile, fieldString);
                cout<<endl<<"Pushed field: "<<fieldString<<endl;
                data.push_back(fieldString);
                //rec_num = data.size()-num_fields;
            }
            rec_num++;
            Record new_record(data, rec_num);
            cout<<endl<<"New record: "<<endl<<new_record;
            cout<<" rec#: "<<rec_num<<", inserting into table_list["<<i<<"] / "<<table_list.size()<<endl;
            table_list[i].insert(new_record);
        }
    }
}

void testSQL(){
    SQL sql;
    Table t;
    vector<string> s;
    vector<string> q;
    t = sql.Select({"Fname"}, "Students", {"Fname"}, {"Silvia"});
    t.print();
    //    cout<<"Main print: ";
    //    t.print();
    //sql.Insert("Employees", {"Willy","Kong","30"});
    //sql.MakeTable("Monkeys", {"Fname","Lname","Age"});
}

void testRun(){
    SQL s;
    s.run();
    
    //SELECT Lname FROM Students WHERE Fname = "Silvia"
    //INSERT INTO Girls VALUES "Flo","Yao","23"
    //MAKE TABLE Girls FIELDS Fname,Lname,Age
    
    //s.Select({"Fname"}, "Students", {"Lname"}, {"Wong"});
    //s.Insert("Students", {"Flo", "Yao", "23"});
    //s.MakeTable("Girls", {"Fname","Lname", "Age"});
}
void testfind(){
    Index tree;
    tree.insert("Flo", 1);
    tree.insert("Flo",3);
    tree.insert("Silvia",2);
    tree.print();
    vector<long> v = tree.find("Flo");
    cout<<v[0];
    cout<<v[1];
}

void testIndexTrees(){
    SQL s;
    //    Table newtable;
    //    newtable = s.Select({"Fname"}, "Students", {"Lname"}, {"Wong"});
    //    newtable.print();
    
    s.Insert("Students", {"Flo", "Yao", "23"});
    Table t = s.MakeTable("Girls", {"Fname","Lname", "Age"});
    
    
    //    Table t("Girls", {"Fname", "Lname", "Age"});
    //    Record r1(vector<string>{"Silvia", "Wong", "23"}, 0);
    //    Record r2(vector<string>{"Willy", "Wong", "21"}, 1);
    //    Record r3(vector<string>{"Silvia", "Yao", "19"}, 2);
    //    t.insert(r1);
    //    t.insert(r2);
    //    t.insert(r3);
    //    vector<long> v = t.searchRecords({0}, {"Silvia"});
    //    cout<<endl<<endl<<v[0]<<" "<<v[1];
}

void testTree(){
    Map<string, string > m;
    //t.get_vector("")
    
    Pair<string, string> p1("COMMAND", "SELECT");
    Pair<string, string> p2("Fields", "Fname");
    Pair<string, string> p4("COLUMN", "Fname");
    Pair<string, string> p3("Table", "Students");
    
    m.Insert(p1);
    m.Insert(p2);
    m.Insert(p3);
    m.Insert(p4);
    
    m.Print();
    
    //    string s  = m.get_values("COMMAND");
    //    cout<<s;
}

void testParser(){
    SQL_Parser p;
    bool ok = p.set_string("SELECT Fname FROM Students WHERE Age = \"23\"");
    //bool ok = p.set_string("INSERT INTO Students VALUES \"Flo\", \"Yao\", \"23\"");
    //bool ok = p.set_string("MAKE TABLE Girls FIELDS Fname, Lname, Age");
    if (ok){
        //cout<<endl<<"PRINTING PARSE TREE: "<<endl;
        //p.printParse();
        cout<<"good sql"<<endl;
    }
    else{
        cout<<"bad, bad sql"<<endl;
    }
}

void testToken(){
    //STokenizer stk("SELECT Fname, Lname FROM Students WHERE Fname = \"Flo\"");
    //STokenizer stk("SELECT Fname FROM Students");
    STokenizer stk("INSERT INTO Students VALUES \"Flo\", \"Yao\", \"23\"");
    Token tok;
    stk >> tok;
    while(!stk.done()){
        if (tok.type_string()!="SPACE")
            cout<<"Token: ["<<tok.token_string()<<"]"<<setw(25)<<right<<"Type: ["<<tok.type_string()<<"]"<<endl;
        //        cin.get();
        stk >> tok;
    }
    
}

void teststrCopy(){
    string s = "hey";
    int max = 3;
    char arr[max];
    strcpy(arr, s.c_str());
    
    for(int i=0; i<3; i++){
        cout<<arr[i];
    }
}

void testMap(){
    Map<string, string> m;
    Pair<string, string> p1("COMMAND", "SELECT");
    Pair<string, string> p2("Fields", "Fname");
    Pair<string, string> p3("Table", "Students");
    Pair<string, string> p4("Fields", "Lname");
    
    m.Insert(p1);
    m.Insert(p2);
    m.Insert(p3);
    m.Insert(p4);
    
    m.Print();
}



void testRecord(){
    //    Table t;
    //    t._columns.push_back("Fname");
    //    t._columns.push_back("Lname");
    //    t._columns.push_back("Age");
    //    Index fname_index;
    
    //    vector<string> list;
    //    list.push_back("Fname");
    //    list.push_back("Lname");
    //    //    vector<string> condition;
    //    //    condition.push_back("Fname");
    //    //    condition.push_back("=");
    
    //    SQL sql;
    //    sql.insertTable(t);
    //    sql.Select(list, "Fname", "Silvia");
}

//write record into a separate text file
//position*# of bytes is your index number
//file_record does that for you and then we
//can push it into the index tree
void testTable(){
    //    vector<string> col = {"Fname", "Lname", "Age"};
    //    vector<string> data1 = {"Silvia", "Yao", "23"};
    //    vector<string> data2 = {"Silvia", "Lee", "20"};
    //    vector<string> data3 = {"Rahim", "Hentakill", "12"};
    //    vector<string> data4 = {"Silvia", "Wong", "30"};
    //    Record one(data1);
    //    Record two(data2);
    //    Record three(data3);
    //    Record four(data4);
    
    //    Index name_index;
    
    //    name_index.insert(one.getName(), 0);
    //    name_index.insert(two.getName(), 1);
    //    name_index.insert(three.getName(), 2);
    //    name_index.insert(four.getName(), 3);
    //    cout<<endl;
    //    name_index.print();
    
    //    cout<<endl;
    //    Table studentTable("Students", col);
    //    studentTable.insert(data1);
    //    studentTable.insert(data2);
    //    studentTable.insert(data3);
    //    studentTable.insert(data4);
    
    //    cout<<endl;
    //    studentTable.print();
    
    //    cout<<endl<<endl<<"Finding..."<<endl;
    //    name_index.find("Rahim", studentTable);
    
}

//void testSelect(){
////    vector<string> query = {"SELECT", "Fname", ",", "Lname",
////                             "FROM", "Students", "WHERE",
////                             "Fname", "=", "Harry"};

//    vector<string> query = {"SELECT", "Fname", "FROM", "Students"};
//    int pos=0;

//    tnode* selectPtr = get_select(query, pos);

//    //testPrint(selectPtr);
//    SQL sql;
//    //sql.Select(selectPtr);

//    cout<<endl<<endl;

////    tnode* field_ptr = get_fields(selectPtr);
////    testPrint(field_ptr);


//    //Table t=test_table();
//    //cout<<"Table: "<<t.tablename; cout<<endl;
//    //t.print();
//}


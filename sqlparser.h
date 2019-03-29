#ifndef SQLPARSER_H
#define SQLPARSER_H
#include <iostream>
#include "map.h"
#include "stokenizer.h"
using namespace std;

//=========================================================================================
//Class:    SQL Parser
//Purpose:  Translates user input into a parse tree. Tokens in parse tree
//          are used to create state machine adjacency table.
//=========================================================================================
class SQL_Parser
{
private:
    char command[C_MAX];
    vector<string> inputq;          //user input
    Map<string, string> parsetree;  //cmd interpreter spits out this tree
    Map<string, int> keywords;      //translates [select] [where] [from] [table]
                                    //  to column numbers for the state machine adjacency table

public:
    enum keyword_enums  {EXTRA, SELECT, FROM, WHERE,
                         INSERT, INTO, MAKE, TABLE,
                         COMMA, QUOTE, STAR, SYMBOL,
                         OPERATOR, STRING, ANDOR,
                         VALUES, FIELDS, TABLE2};
    
    SQL_Parser();
    
    static const int MAX_P = 30;
    int table[MAX_P][MAX_P];
    
    bool set_string(string c);
    bool get_parse_tree();
    bool is_fail(int state);
    bool is_success(int state);
    
    vector<string> split(char c[]);
    Map<string, string>& getParseTree();
    int get_start_state(string token);
    void insert_keywords();
    void reset();
    void printParse();
    void make_table();
    void init_table();
    void print_table();
    void mark_success(int table[][MAX_P], int state);
    void mark_fail(int table[][MAX_P], int state);
    void mark_cell(int row, int table[][MAX_P], int token, int state);
    //void mark_cell(int row, int table[][COL_MAX], const char column[], int state);
};

//=========================================================================================
//Function: SQL Parser
//Purpose:  This is the default constructor. It initializes and
//          populates our state machine adjacency table.
//=========================================================================================
SQL_Parser::SQL_Parser(){
    init_table();
    insert_keywords();
    make_table();
}

void SQL_Parser::reset(){
    cout<<endl<<"SQL_Parser::reset: called reset"<<endl;
    parsetree.clear();
}

void SQL_Parser::printParse(){
    parsetree.Print();
}

//=========================================================================================
//Function: getParseTree
//Purpose:  returns parse tree
//=========================================================================================
Map<string, string> &SQL_Parser::getParseTree(){
    return parsetree;
}

//*========================================================================================
// Function:  make_table()
// Purpose:   Creates adjacency table for state machine
//
// CONSIDER: This could have been better if exported the table from file.
//              - can visually see matrix, to easier track state transitions.
// 
//=========================================================================================
void SQL_Parser::make_table(){
    //keep calling mark_cell(table, start state, token column, go to state)
    mark_fail(table, 0);
    mark_fail(table, 1);
    mark_fail(table, 2);
    mark_fail(table, 3);
    mark_success(table, 4);
    mark_fail(table, 5);
    mark_fail(table, 6);
    mark_fail(table, 7);
    mark_fail(table, 8);
    mark_fail(table, 9);
    mark_success(table, 10);
    mark_fail(table, 11);
    mark_fail(table, 12);
    mark_fail(table, 13);
    mark_fail(table, 14);
    mark_fail(table, 15);
    mark_fail(table, 16);
    mark_fail(table, 17);
    mark_success(table, 18);
    mark_fail(table, 19);
    mark_fail(table, 20);
    mark_fail(table, 21);
    mark_fail(table, 22);
    mark_fail(table, 23);
    mark_success(table, 24);
   // mark_success(table, 25);

    //START_SELECT = 0
    mark_cell(0, table, SELECT, 1);
    mark_cell(1, table, SYMBOL, 2);
    mark_cell(1, table, STAR, 3);
    mark_cell(2, table, FROM, 3);
    mark_cell(2, table, COMMA, 1);
    mark_cell(3, table, SYMBOL, 4);
    mark_cell(4, table, COMMA, 3);
    mark_cell(4, table, WHERE, 5);
    mark_cell(5, table, SYMBOL, 6);
    mark_cell(6, table, OPERATOR, 7);
    mark_cell(7, table, QUOTE, 8);
    mark_cell(8, table, SYMBOL, 9);
    mark_cell(9, table, QUOTE, 10);
    mark_cell(10, table, COMMA, 5);

    //START INSERT INTO = 11
    mark_cell(11, table, INSERT, 12);
    mark_cell(12, table, INTO, 13);
    mark_cell(13, table, SYMBOL, 14);
    mark_cell(14, table, VALUES, 15);
    mark_cell(15, table, QUOTE, 16);
    mark_cell(16, table, SYMBOL, 17);
    mark_cell(17, table, QUOTE, 18);
    mark_cell(18, table, COMMA, 15);

    //START MAKE TABLE = 19
    mark_cell(19, table, MAKE, 20);
    mark_cell(20, table, TABLE, 21);
    mark_cell(21, table, SYMBOL, 22);
    mark_cell(22, table, FIELDS, 23);
    mark_cell(23, table, SYMBOL, 24);
    mark_cell(24, table, COMMA, 23);
}

//*========================================================================================
// Function:  get_start_state
// Purpose:   returns the starting state of the inputted token
// Pre-Cond:  token is validated by state machine. 
// Post-Cond: return the correct start state
// Returns:   the appropiate value of the starting state
//=========================================================================================
int SQL_Parser::get_start_state(string token){
    if(token == "SELECT"){
        return START_SELECT;
    }
    else if(token == "INSERT"){
        return START_INSERT;
    }
    else if(token == "MAKE"){
        return START_MAKE;
    }
    cout<<endl<<"SQL_Parser::getStartState: no start state"<<endl;
    return -1;
}

//=========================================================================================
// Function:  get_parse_tree
// Purpose:   Receives inputq (filled with tokens) as input & puts it through state machine
//            switch cases correspond to states in the state table
// Pre-Cond:  State table marked with appropriate states, inputq contains tokens
// Post-Cond: Parse tree generated
// Returns:   true if state is a success (state = 1 is success, state = -1 is fail)
//=========================================================================================
bool SQL_Parser::get_parse_tree(){
    int state;

    for(int i=0; i<inputq.size() && state != -1; i++){
        string token = inputq[i];

        // valid queries start with commands e.g. SELECT, INSERT, MAKE...
        if(i==0){
            state = get_start_state(token);
        }
        vector<int> value = keywords.get_values(token); //token = [select], [where], [from], [table]...
                                                        //  value is the column number for the state machine adjacency table
        int column;

        // SYMBOL = everything that is not a key word (Fname, Lname, Fieldname, name of person)
        if(value.size() == 0){
            column = SYMBOL;
        }
        else{
            column = value[0];
        }
        
        if(state >= 0 ){
            state = table[state][column];
        }
        else{
            return false;
        }

        // cases are states
        switch(state){
        case 1:  parsetree.insertAt("COMMAND", token);  break;
        case 2:  parsetree.insertAt("Fields",  token);  break;
        //case 3:  parsetree.insertAt("Fields",  token);  break; //STAR
        case 4:  parsetree.insertAt("TABLE",   token);  break;
        case 6:  parsetree.insertAt("COLUMN" , token);  break;
        case 7:  parsetree.insertAt("OPERATOR",token);  break;
        case 9:  parsetree.insertAt("STRING",  token);  break;
        case 12: parsetree.insertAt("COMMAND", token);  break;
        case 14: parsetree.insertAt("TABLE",   token);  break;
        case 17: parsetree.insertAt("VALUES",  token);  break;
        case 20: parsetree.insertAt("COMMAND", token);  break;
        case 22: parsetree.insertAt("TABLE",   token);  break;
        case 24: parsetree.insertAt("Fields",  token);  break;
        default: cout<<endl<<"default"<<endl;           break;
        }
    }
    
    // end state is either success or fail
    if(is_success(state)){
        parsetree.Print();
        return true;
    }
    else{
        return false;
    }
}

//*========================================================================================
// Function:  set_string();
// Purpose:   Parses user input into string tokens and prints them out
// Pre-Cond:  String input 'c' is not null
// Post-Cond: Strings are successfully tokenized
//=========================================================================================
bool SQL_Parser::set_string(string c){
    strcpy(command, c.c_str()); // user input is a string, command is a char[]

    inputq = split(command); //inputq is a vector of token names
    
    for(int i= 0;i<inputq.size(); i++){
        cout<<"|"<<inputq[i]<<"| "; // test output
    }
    return get_parse_tree(); //get_parse_tree takes the inputq => state machine validates T or F
    cout<<endl;
}

//*========================================================================================
//Function: insert_keywords
//Purpose:  Populate adjacency matrix with appropriate column values
//          e.g. SELECT is in column 14, FROM is on column 15
//=========================================================================================
void SQL_Parser::insert_keywords(){
    keywords.Insert(Pair<string,int>("SELECT", SELECT));
    keywords.Insert(Pair<string,int>("FROM",   FROM));
    keywords.Insert(Pair<string,int>("WHERE",  WHERE));
    keywords.Insert(Pair<string,int>("VALUES", VALUES));
    keywords.Insert(Pair<string,int>("STRING", STRING));
    keywords.Insert(Pair<string,int>("ANDOR",  ANDOR));
    keywords.Insert(Pair<string,int>("TABLE",  TABLE));
    keywords.Insert(Pair<string,int>("INSERT", INSERT));
    keywords.Insert(Pair<string,int>("EXTRA",  EXTRA));
    keywords.Insert(Pair<string,int>("FIELDS", FIELDS));
    keywords.Insert(Pair<string,int>("MAKE",   MAKE));
    keywords.Insert(Pair<string,int>("INTO",   INTO));
    keywords.Insert(Pair<string,int>("*",      STAR));
    keywords.Insert(Pair<string,int>(",",      COMMA));
    keywords.Insert(Pair<string,int>("<",      OPERATOR));
    keywords.Insert(Pair<string,int>(">",      OPERATOR));
    keywords.Insert(Pair<string,int>("\"",     QUOTE));
    keywords.Insert(Pair<string,int>("=",      OPERATOR));
}

//*=========================================================================================
//Function: split
//Purpose:  Tokenizes commands and stores into a vector of strings
//=========================================================================================
vector<string> SQL_Parser::split(char c[]){
    vector<string> temp;
    STokenizer stk(c);
    Token t;

    stk >> t;
    while(!stk.done()){ // while there are still tokens left to tokenize
        if(t.type_string()!="SPACE"){ // if the token inputted is not a space, it is a token. 
            temp.push_back(t.token_string()); // push into vector of string names of the token
        }
        stk >> t; // accept another token
    }
    return temp;
}

//=========================================================================================
//Function: init_table
//Purpose:  Initializes matrix
//=========================================================================================
void SQL_Parser::init_table(){
    for(int i=0; i<MAX_P; i++){
        for(int j=0; j<MAX_P; j++){
            table[i][j] = -1;
        }
    }
}

//=========================================================================================
//Function: print_table
//Purpose:  Prints table
//=========================================================================================
void SQL_Parser::print_table(){
    for(int i=0; i<MAX_P; i++){
        cout << i << ": ";
        for(int j=0; j<MAX_P; j++){
            if(table[i][j] != -1){
                cout<<" "; //alignment
            }
            cout<<table[i][j]<<"   ";
        }
        cout<<endl;
    }
}

//=========================================================================================
//Function: mark_cell
//Purpose:  Marks the appropriate column with the correct state for the token specified
//=========================================================================================
void SQL_Parser::mark_cell(int row, int table[][MAX_P], int token, int state){
    table[row][token] = state;
}

//=========================================================================================
//Function: mark_fail
//Purpose:  Marks the cell as a fail state
//=========================================================================================
void SQL_Parser::mark_fail(int table[][MAX_P], int state){
    table[state][0]=0;
}

//=========================================================================================
//Function: mark_success
//Purpose:  Marks the cell as a success state
//=========================================================================================
void SQL_Parser::mark_success(int table[][MAX_P], int state){
    table[state][0]=1;
}

//=========================================================================================
//Function: is_fail
//Purpose:  checks if the state is a fail state
//=========================================================================================
bool SQL_Parser::is_fail(int state){
    return table[state][0] == -1;
}

//=========================================================================================
//Function: is_success
//Purpose:  checks if the state is a success state
//=========================================================================================
bool SQL_Parser::is_success(int state){
    return table[state][0] == 1;
}

#endif // SQLPARSER_H

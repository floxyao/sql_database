#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <string>
#include "../tokenizer/token_const.h"
using namespace std;

//=========================================================================================
//Class:    Token
//Purpose:  This token class has a string (name of the token) and its type, so each
//          token knows exactly what type it is so that the caller knows what to do with
//          each token. E.G. If this token is a space, then theres a token preceding.
//          If this token is a WORD then it's valid.
//=========================================================================================
class Token
{
public:
    Token();
    Token(string str, int type);

    friend ostream& operator << (ostream& out, const Token& printMe);
    void set_type(int type); // returns type as integer
    int get_type();
    string type_string(); //returns type as string
    string token_string(); // returns token string

private:
    string _token;
    int _type;
};

Token::Token(){
    //cout<<endl<<"[TOKEN DEFAULT CTOR]"<<endl;
    _token = " ";
    _type = 0;
}

Token::Token(string str, int type){
    _token = str;
    set_type(type);
}

ostream& operator << (ostream& out, const Token& printMe){
    out << printMe._token;
    return out;
}

int Token::get_type(){
    return _type;
}

void Token::set_type(int type){
    switch(type){
    case NUMBER:
        _type = NUMBER;
        break;
    case WORD:
        _type = WORD;
        break;
    case SPACE:
        _type = SPACE;
        break;
    case SYMBOL:
        _type = SYMBOL;
        break;
    case UNRECOGNIZED:
        _type = UNRECOGNIZED;
        break;
    default:
        cout<<"Token set_type default";
        break;
    }
}

string Token::type_string(){
    if(_type==NUMBER){
        return "NUMBER";
    }
    if(_type==WORD){
        return "WORD";
    }
    if(_type==SPACE){
        return "SPACE";
    }
    if(_type==SYMBOL){
        return "SYMBOL";
    }
    if(_type==UNRECOGNIZED){
        return "UNRECOGNIZED";
    }
    //cout<<"Token type_string empty"<<endl;
    return " ";
}

string Token::token_string(){
    return _token;
}

#endif // TOKEN_H

//STokenizer gets the string, sends it to the outside
//privates: token, type

//Main
//STokenizer stk
//stk>>token;
//cout<<token;

//FTokenizer ftk
//takes the token that stk gets for him and creates a new block and puts it in
//until the block is full then creates the new block


//while ftokenizer.more() <- is there more?

#ifndef COMMAND_FUNCS_H
#define COMMAND_FUNCS_H
#include "node.h"
#include <vector>

//=================================================================================
// This file contains declarations and definitions of functions for the SQL class.
// It's separated into another header file
//================================================================================+
tnode* get_where(vector<string>& v, int &curr_pos);
tnode* get_from(vector<string>& v, int &curr_pos);
tnode* get_columns(vector<string>& v, int &curr_pos);
tnode* get_select(vector<string>& v, int &curr_pos);
void testPrint(tnode* root, int indent);


//=================================================================================
// DEFINITIONS
//================================================================================+
tnode* get_fields(tnode* select_ptr){
    return select_ptr->child[0];
}

tnode* get_tableName(tnode* select_ptr){
    return select_ptr->child[1];
}

tnode* get_condition(tnode* select_ptr){
    return select_ptr->child[2];
}

void testPrint(tnode* root, int indent=0){
    int extra_indent = 4;
    int i;

    cout << setw(indent) << " ";
    cout<<root->name<<endl;

    for(i=0; i<root->child_count; ++i){
        testPrint(root->child[i], indent+extra_indent);
    }
}


tnode* get_where(vector<string>& v, int &curr_pos){
    tnode* where_ptr = new tnode("WHERE");
    tnode* condition_ptr = new tnode("=");
    tnode* column_ptr = new tnode("COLUMN");
    int cond_i=0;
    int col_i=0;

    while(v[curr_pos] != "="){
        if(v[curr_pos]==","){
            curr_pos++;
        }
        else{
            column_ptr->child[col_i]=new tnode(v[curr_pos]);
            col_i++;
            curr_pos++;
        }
    }
    column_ptr->child_count=col_i;
    //set column pointer child count

    condition_ptr->child[cond_i]=column_ptr;
    //point condition pointer to column ptr

    cond_i++;
    //increment its child count

    curr_pos++;
    //move pos

    //not passing in numUsed how to know we're at the end
    while(curr_pos < v.size()){ //12 would be numUsed of the string
        condition_ptr->child[cond_i]=new tnode(v[curr_pos]);
        cond_i++;
        curr_pos++;
    }

    condition_ptr->child_count=cond_i;

    where_ptr->child[0]=condition_ptr;
    where_ptr->child_count=1;

    return where_ptr;
}

tnode* get_from(vector<string>& v, int &curr_pos){
    //cout<<endl<<"v[c]"<<v[curr_pos]<<endl;
    tnode* from_ptr = new tnode("FROM");

    int i=0;

    //bandaided, need condition for commas too
    while(curr_pos < v.size()){
        from_ptr->child[i]=new tnode(v[curr_pos]);
        i++;
        curr_pos++;

        if(v[curr_pos] == "WHERE"){
            curr_pos++;
            break;
        }
    }
//    while(v[curr_pos] != "WHERE" || curr_pos <= v.size()){
//        from_ptr->child[i]=new tnode(v[curr_pos]);
//        i++;
//        curr_pos++;
//    }
    from_ptr->child_count=i;
    return from_ptr;
}

tnode* get_columns(vector<string>& v, int &curr_pos){
    tnode* column_ptr = new tnode("COLUMN");
    int i=0;
    while(v[curr_pos] != "FROM"){
        //cout<<endl<<"i start: "<<curr_pos;
        if(v[curr_pos]==","){
            curr_pos++;
        }
        else{
            column_ptr->child[i]=new tnode(v[curr_pos]);
            //cout<<endl<<"current: "<<v[curr_pos]<<endl;
            i++;
            curr_pos++;
        }
        //cout<<endl<<"i is: "<<i;

    }
    column_ptr->child_count=i;
    //cout<<endl<<"current position: "<<curr_pos<<endl;
    curr_pos++;
    return column_ptr;
}


tnode* get_select(vector<string>& v, int &curr_pos){
    tnode* select_ptr = new tnode(v[curr_pos]);
    curr_pos++;
    //cout<<endl<<"curr: "<<curr_pos;
    select_ptr->child[0]=get_columns(v, curr_pos);
    //cout<<endl<<"outside get column"<<endl;
    //cout<<endl<<"outside curr_pos is: "<<curr_pos;
    select_ptr->child[1]=get_from(v, curr_pos);
    //cout<<endl<<"outside get from"<<endl;
    //if there is a where condition
    if(curr_pos < v.size()){
        select_ptr->child[2]=get_where(v, curr_pos);
        select_ptr->child_count = 3;
    }
    else{
        select_ptr->child_count=2;
    }
    return select_ptr;
}



#endif // COMMAND_FUNCS_H

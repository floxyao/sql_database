#ifndef TOKEN_CONST_H
#define TOKEN_CONST_H

const int ROW_MAX = 100;
const int COL_MAX = 256;
const int BUFFER_MAX = 100;
const int SUCCESS = 0;
const int FAIL = -1;
const int NUMBER = 0;
const int WORD = 1;
const int SPACE = 2;
const int UNRECOGNIZED = 3;
const int SYMBOL = -1;
const int START_DIGIT = 0;
const int START_DECIMAL = 1;
const int START_SPACE = 4;
const int START_LETTER = 6;
const int START_APOSTROPHE = 8;
const int START_COMMA = 10;
const int START_EXCLAMATION = 11;
const int START_QUESTION = 12;
const int START_COPERATOR = 13;
const int LAST_INDEX = 50;
const int FIRST_INDEX = 0;
const char DIGITS[] = "0123456789";
const char UPPER_ALPHA[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char LOWER_ALPHA[] = "abcdefghijklmnopqrstuvwxyz";
const char SPACES[] = {' ', '\t', '\n', '\0'};
const char PERIOD[] = ".";
const char APOSTROPHE[] = "'";
const char COMMA[] = ",";
const char EXCLAMATION[] = "!";
const char QUESTION[] = "?";
const char COPERATOR[] = "><=";


#endif // TOKEN_CONST_H

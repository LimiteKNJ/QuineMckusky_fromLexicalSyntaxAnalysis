#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "err.h"

// get Line MAX 
#define ID_LENGTH 20
#define LIST_LENGTH 100

// dataType
#define T_NULL -1

#define T_IDENTIFIER 1
#define T_NUMBER 2

#define T_PLUS 11
#define T_MINUS 12
#define T_MUL 13
#define T_DIV 14
#define T_MOD 15
#define T_EQUALS 16

#define T_AND 21
#define T_OR 22
#define T_NOT 23

#define T_LPAREN 31
#define T_RPAREN 32
#define T_LBRACE 33
#define T_RBRACE 34
#define T_LBRACKET 35
#define T_RBRACKET 36

#define T_SPACE 51
#define T_COMMA 52
#define T_SEMICOLON 53
#define T_UPPER 54
#define T_DOUBLEQT 55

#define T_ENDS 100


// token
typedef struct tokenData {
	int type; // dataType
	char value[ID_LENGTH]; // data
} tokenData;

tokenData token[LIST_LENGTH];
int tListIndex;

/* Text */
void printTexts(char* string);
void readTexts(char* string);

/* Read Token */
void storeTokenData(int type, char* string);
void printTokenList(tokenData* tokenList);
char* getTokenType(int type);

void freeTokens(tokenData* list);
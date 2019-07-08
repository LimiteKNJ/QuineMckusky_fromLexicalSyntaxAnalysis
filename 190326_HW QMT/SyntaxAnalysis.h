#include"LexicalAnalysis.h"

// QuineMaclisky Syntex
#define Q_NULL 0
#define Q_FUNC 1
#define Q_MIN 2
#define Q_DTM 3
#define Q_PLUS 4
#define Q_EQUALS 5

// Syntex Type
#define S_VARIABLE 1
#define S_CONST 2
#define S_MNUMBER 3
#define S_DNUMBER 4
#define S_OPERATOR 5

#define S_POPEN 7
#define S_PCLOSE 8

#define S_FUNCTION_NAME 11
#define S_MINFUNC 12
#define S_DTMFUNC 13

#define P_OPENED 1
#define P_CLOSED 0
#define F_MIN 20
#define F_DTM 21

tokenData synData[LIST_LENGTH];
int qsynTable[LIST_LENGTH];
int sListIndex; int parenStatus; int funcStatus;
int syncnt;

/* Read Token */
void readLexToken(tokenData* tokenList);
void storeSyn(int type, char* data);;

void printSynList(tokenData* tokenList);
char* getSynType(int type);


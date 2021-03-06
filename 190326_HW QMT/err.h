#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define TOK_NOT_FOUND 51

#define NOT_SUPPORT_OP_MINUS 61
#define NOT_SUPPORT_OP_MUL 62
#define NOT_SUPPORT_OP_DIV 63
#define NOT_SUPPORT_OP_MOD 64
#define NOT_SUPPORT_OP_AND 65
#define NOT_SUPPORT_OP_OR 66
#define NOT_SUPPORT_OP_NOT 67
#define NOT_SUPPORT_BRACE 68
#define NOT_SUPPORT_BRACKET 69

#define PARENT_NOT_CLOSED 71
#define PARENT_NOT_OPENED 72
#define PARENT_IN_PLUS 73
#define PARENT_IN_EQUALS 74

#define FUNC_NOT_FOUND 81
#define FUNC_OUT_OF_RANGE 82
#define FUNC_MIN_OUT_OF_RANGE 83
#define FUNC_DTM_OUT_OF_RANGE 84

#define VAR_NOT_FOUND 91
#define VAR_OUT_OF_RANGE 92
#define VAR_NOT_INPUT_NUM 93

#define NUM_NOT_FOUND 101
#define NUM_OUT_OF_RANGE 102
#define NUM_DTM_OUT_OF_RANGE 103

#define OP_PLUS_NOT_FOUND 111
#define OP_EQUALS_NOT_FOUND 112
#define OP_EQUALS_LOCATION_ER 113
#define OP_EQUALS_OVER_2 114

int errNum;
void err(int errNum);
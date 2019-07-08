#include"SyntaxAnalysis.h"

#define VAR_MAX 4
#define NUM_MAX 16

typedef struct TNODE {
	int num_table[16];
	int bit[4];
	struct TNODE* next;
} TNODE;

int selTable[NUM_MAX];	// Select Table
int dtmTable[NUM_MAX];
int epiTable[NUM_MAX];	 // EPI Table Group
int** epi;				// EPI Table
int epiTableHeight;		// EPI Table Height

// 변수 개수
extern int qsynTable[LIST_LENGTH];
extern int errNum;
int funccnt; int mincnt; int dtmcnt;
int pcnt; int ecnt; int varcnt;


// read Token
void readFuncName();
void readVar();
void readNumber();
void checkSyn();
int isvarCnt(int num);

/* Table 관련 */
void makeTable(TNODE* column); // table 작성
void mergeTable(TNODE* column_1, TNODE* column_2); // 합치기
void delOverlap(TNODE* head); // 중복제거
int checkBit(int bit[]); // 1bit 비교

/* PI 관련 */
void correctPI(TNODE* column_one, TNODE* column_two, TNODE* PI); // PI 결정
void circuitType(TNODE* PI); // PI 회로
void correctEPI(); // EPI 결정

// Print Result
void printTable(TNODE* head);
void printFunc(TNODE* PI);
void printPI(TNODE* PI, int **arr, int result);

void freeNode(TNODE* node);
void freeTable(int* table);

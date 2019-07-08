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

// ���� ����
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

/* Table ���� */
void makeTable(TNODE* column); // table �ۼ�
void mergeTable(TNODE* column_1, TNODE* column_2); // ��ġ��
void delOverlap(TNODE* head); // �ߺ�����
int checkBit(int bit[]); // 1bit ��

/* PI ���� */
void correctPI(TNODE* column_one, TNODE* column_two, TNODE* PI); // PI ����
void circuitType(TNODE* PI); // PI ȸ��
void correctEPI(); // EPI ����

// Print Result
void printTable(TNODE* head);
void printFunc(TNODE* PI);
void printPI(TNODE* PI, int **arr, int result);

void freeNode(TNODE* node);
void freeTable(int* table);

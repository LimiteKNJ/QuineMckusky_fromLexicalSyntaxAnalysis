#include"main.h"

int main(void) {
	TNODE* column_0 = (TNODE*)malloc(sizeof(TNODE));
	TNODE* column_1 = (TNODE*)malloc(sizeof(TNODE));
	TNODE* column_2 = (TNODE*)malloc(sizeof(TNODE));
	TNODE* column_3 = (TNODE*)malloc(sizeof(TNODE));
	TNODE* column_4 = (TNODE*)malloc(sizeof(TNODE));
	TNODE* PI = (TNODE*)malloc(sizeof(TNODE));
	memset(column_0, 0, sizeof(TNODE));
	memset(column_1, 0, sizeof(TNODE));
	memset(column_2, 0, sizeof(TNODE));
	memset(column_3, 0, sizeof(TNODE));
	memset(column_4, 0, sizeof(TNODE));
	memset(PI, 0, sizeof(TNODE));
	int result;

	// input data
	printf_s("============================\n");
	printf_s("::: QueneMaclisky Tablar Program ::: \n");
	printf_s("-> <func>(<var>) = <min | sumMin | sum_min>(<num>) + <dtm | sumDtm | sum_dtm>(<num>) \n");
	printf_s("-> <exit | quit | break> : Exit Program\n\n");
	while (1) {
		errNum = 0;
		char str[INPUT_MAX] = { 0, };
		int i; int ch;

		printf_s("============================\n");
		printf_s("Input : ");
		fgets(str, INPUT_MAX, stdin);

		/* Get Token */
		printTexts(str);
		readTexts(str);
		readLexToken(token);
		if (!strcmp(synData[0].value, "exit") || !strcmp(synData[0].value, "break") ||
			!strcmp(synData[0].value, "quit"))	break; // exit, quit, break
		if (!strcmp(synData[0].value, "") && errNum == FALSE) { // Exception : 아무 입력 없을 시
			errNum = TOK_NOT_FOUND; err(TOK_NOT_FOUND);
		}

		if (errNum == FALSE) readFuncName();
		if (errNum == FALSE) readVar();
		if (errNum == FALSE) readNumber();
		if (errNum == FALSE) checkSyn();

		if (errNum == FALSE) { // Exception : 소괄호 닫히지 않을 시
			for (int i = 0; i < NUM_MAX; i++) // Table -> Node
				column_0->num_table[i] = selTable[i];

			/* Make Table */
			makeTable(column_0);
			printTable(column_0);
			printf_s("\n");

			/* 1차 병합 */
			freeNode(column_1);
			mergeTable(column_0, column_1);
			printTable(column_1);
			result = 1;
			printf_s("\n");

			/* 2차 병합 */
			if (column_1->next != NULL) {
				freeNode(column_2);
				mergeTable(column_1, column_2);
				if (column_2->next != NULL) {
					delOverlap(column_2);
					printTable(column_2);
					printf_s("\n");
					result = 2;
				}
			}
			
			/* 3차 병합 */
			if (column_2->next != NULL) {
				freeNode(column_3);
				mergeTable(column_2, column_3);
				if (column_3->next != NULL) {
					delOverlap(column_3);
					printTable(column_3);
					printf_s("\n");
					result = 3;
				}
			}
				
			/* 4차 병합 */
			if (column_3->next != NULL) {
				freeNode(column_4);
				mergeTable(column_3, column_4);
				if (column_4->next != NULL) {
					delOverlap(column_4);
					printTable(column_4);
					printf_s("\n");
					result = 4;
				}
			}
	
			/* PI 결정 */
			freeNode(PI);
			if (result == 1) correctPI(column_1, column_1, PI);
			else if (result == 2) correctPI(column_1, column_2, PI);
			else if (result == 3) correctPI(column_2, column_3, PI);
			else correctPI(column_3, column_4, PI);
			printf_s(":: PI ::\n");
			printTable(PI);
			printf_s("\n");
			
			/* EPI 작성 */
			circuitType(PI);
			printPI(PI, epi, result);
			printf_s("\n");
			
			correctEPI();
			printPI(PI, epi, result);
			printf_s("\n");
			
			/* 간소화 식 출력 */
			printFunc(PI);
		}

		/* Memory Free 및 초기화 */
		freeNode(column_0);
		freeNode(column_1);
		freeNode(column_2);
		freeNode(column_3);
		freeNode(column_4);

		freeTokens(token);
		freeTokens(synData);
		freeTable(selTable);
		freeTable(epiTable);
		free(epi); epi = NULL;

		memset(numTable, -1, (sizeof(int)) * NUM_MAX);
		memset(funcName, '\0', sizeof(char) * ID_LENGTH);

	} printf_s("============================\n");
	printf_s("Program End \n"); return 0;
}
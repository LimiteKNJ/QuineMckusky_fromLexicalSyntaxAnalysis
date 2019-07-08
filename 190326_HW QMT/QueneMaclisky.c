#include"QueneMaclisky.h"

// extern int qsynTable[LIST_LENGTH];
char funcName[ID_LENGTH] = "";
char varTable[VAR_MAX+1][ID_LENGTH] = { '\0', };
int numTable[NUM_MAX] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
int dtmNumTable[NUM_MAX] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

/* Memory free() */
void freeNode(TNODE* node) {
	TNODE* p = node;
	TNODE* s = p->next;

	while (s != NULL) {
		p->next = s->next;	free(s);
		s = p->next;
	}
}

void freeTable(int* table) {
	for (int i = 0; i < NUM_MAX; i++) {
		table[i] = 0;
	}
}

/* token Read → Tabular method */
void readFuncName() {
	tListIndex = 0; int i;
	while (synData[tListIndex].type != NULL) {
		if (synData[tListIndex].type == S_FUNCTION_NAME) {
			for (i = 0; synData[tListIndex].value[i] != '\0'; i++)
				funcName[i] = synData[tListIndex].value[i];
			funcName[i + 1] = '\0'; i = 0;
		} tListIndex++;
	}
}

void readVar() {
	tListIndex = 0; varcnt = 0; int i;
	while (synData[tListIndex].type != NULL) {
		if (synData[tListIndex].type == S_VARIABLE) {
			for (i = 0; synData[tListIndex].value[i] != '\0'; i++)
				varTable[varcnt][i] = synData[tListIndex].value[i];
			varTable[varcnt][i+1] = '\0'; i = 0;
			varcnt++;
		} tListIndex++;
	}

	// Exception : if variable is not found
	// ex. func() = <expr>
	if (varTable[0][0] == '\0') {
		errNum = VAR_NOT_FOUND;
		err(VAR_NOT_FOUND);
	}

	// Excption : if variable is over 5 stocks
	// ex. func(x,y,z,a,b,...) = <expr>
	if (varcnt > 4) { 
		errNum = VAR_OUT_OF_RANGE;
		err(VAR_OUT_OF_RANGE);
	}
}

void readNumber() {
	// Number Table
	int* tnumTable = (int*)malloc(sizeof(int)*NUM_MAX);
	int* tdtmNumTable = (int*)malloc(sizeof(int)*NUM_MAX);
	memset(tnumTable, -1, sizeof(int)*NUM_MAX);
	memset(tdtmNumTable, -1, sizeof(int)*NUM_MAX);

	tListIndex = 0; int i = 0; int j = 0;
	while (synData[tListIndex].type != NULL) {
		if (synData[tListIndex].type == S_MNUMBER ||
			synData[tListIndex].type == S_DNUMBER) {
				tnumTable[i] = atoi(synData[tListIndex].value);
				i++;
		} tListIndex++;
	}

	// Don't Care Condition Table
	tListIndex = 0; i = 0;
	while (synData[tListIndex].type != NULL) {
		if (synData[tListIndex].type == S_DNUMBER) {
				tdtmNumTable[i] = atoi(synData[tListIndex].value);
				i++;
		} tListIndex++;
	}

	/* variable count to num range */
	j = 0;
	for (i = 0; i < NUM_MAX; i++) {
		if (tnumTable[0] == -1) { // ex. func(a,b,c) = 
			errNum = NUM_NOT_FOUND;
			err(NUM_NOT_FOUND);
			break;
		} if (isvarCnt(tnumTable[i])) {
			numTable[j] = tnumTable[i];
			j++;
		} else { // Exception : 변수 개수에 따른 입력값 초과
			errNum = NUM_OUT_OF_RANGE;
			err(NUM_OUT_OF_RANGE);
			break;
		}
	}

	j = 0;
	for (i = 0; i < NUM_MAX; i++) {
		if (errNum == NUM_NOT_FOUND) break; // ex. func(a,b,c) = min(<numList>)
		if (isvarCnt(tdtmNumTable[i])) {
			dtmNumTable[j] = tdtmNumTable[i];
			j++;
		} else { // Exception : 변수 개수에 따른 입력값 초과
			errNum = NUM_DTM_OUT_OF_RANGE;
			err(NUM_DTM_OUT_OF_RANGE);
			break;
		}
	}

	/* Number Table -> 중복 없는 Number Table */
	i = 0;
	while (numTable[i] != -1){
		if(numTable[i] < NUM_MAX && numTable[i] >= 0)
			selTable[numTable[i]] = 1;
		i++;
	}

	i = 0;
	while (dtmNumTable[i] != -1) {
		if (dtmNumTable[i] < NUM_MAX && dtmNumTable[i] >= 0)
			dtmTable[dtmNumTable[i]] = 1;
		i++;
	}

	free(tnumTable);
	free(tdtmNumTable);
}

/* expr Check */
void checkSyn() {
	for (syncnt = 0; qsynTable[syncnt] != Q_NULL; syncnt++) {
		if (qsynTable[syncnt] == Q_EQUALS) ecnt++;
		if (syncnt % 2 == 0) {
			if (qsynTable[syncnt] == Q_FUNC) funccnt++;
			if (qsynTable[syncnt] == Q_MIN) mincnt++;
			if (qsynTable[syncnt] == Q_DTM) dtmcnt++;
		}
		if (syncnt % 2 == 1) {
			if (qsynTable[syncnt] == Q_PLUS) pcnt++;
		}
	}

	// Exception : 함수 선언 X
	if (funccnt == 0) { // ex. () = <expr>
		errNum = FUNC_NOT_FOUND;
		err(FUNC_NOT_FOUND);
	}

	// Exception : 2개 이상의 함수 선언
	if (funccnt > 1) { // ex. func() + func() = <expr>
		errNum = FUNC_OUT_OF_RANGE;
		err(FUNC_OUT_OF_RANGE);
	}

	// Exception : + 연산자 사용 X
	if (pcnt == 0 && funccnt != 1) { // ex. func() = min() dtm()
		errNum = OP_PLUS_NOT_FOUND;
		err(OP_PLUS_NOT_FOUND);
	}

	// Exception : = 연산자 사용 X
	if (ecnt == 0) { // ex. func() min() + dtm()
		errNum = OP_EQUALS_NOT_FOUND;
		err(OP_EQUALS_NOT_FOUND);
	}

	// ex. func() = min() + dtm() or min() + dtm() = func() ... 이외에 대한 예외처리
	if (ecnt == 1 && !(qsynTable[1] == Q_EQUALS || qsynTable[syncnt - 2] == Q_EQUALS)) {
		errNum = OP_EQUALS_LOCATION_ER;
		err(OP_EQUALS_LOCATION_ER);
	}

	// ex. func() = min() + dtm() = or min() + dtm() = func() = ... 이외에 대한 예외처리
	if (ecnt == 2 && !((qsynTable[1] == Q_EQUALS && qsynTable[syncnt - 1] == Q_EQUALS) ||
		(qsynTable[syncnt - 3] == Q_EQUALS && qsynTable[syncnt - 1] == Q_EQUALS))) {
		errNum = OP_EQUALS_LOCATION_ER;
		err(OP_EQUALS_LOCATION_ER);
	}

	// Exception : = 연산자 중복 사용
	if (ecnt > 2) { // ex. func() == min() + dtm() =
		errNum = OP_EQUALS_OVER_2;
		err(OP_EQUALS_OVER_2);
	}

	funccnt = 0; mincnt = 0; dtmcnt = 0;
	pcnt = 0; ecnt = 0;
}

/* var에 따른 개수 및 입력 상수 제한 */
int isvarCnt(int num) {
	if (varcnt == 1 && num < 2) {
		return TRUE;
	} else if (varcnt == 2 && num < 4) {
		return TRUE;
	} else if (varcnt == 3 && num < 8) {
		return TRUE;
	} else if (varcnt == 4 && num < 16) {
		return TRUE;
	} else return FALSE;
}

/* Table 작성 */
void makeTable(TNODE* column) {
	TNODE* p = column;
	TNODE* s = p->next;
	int temp;

	for (int i = 0; i < NUM_MAX; i++) {
		if (selTable[i] == 1) {
			TNODE* newdata = (TNODE*)malloc(sizeof(TNODE));

			// 처음 방을 제외한 전부 -1로 초기화
			newdata->num_table[0] = i;
			for (int j = 1; j < NUM_MAX; j++) {
				newdata->num_table[j] = -1;
			} temp = i;

			/* dec -> bin 연산 */
			// 변수 개수에 따른 bit 출력 자리수 제한 
			if (varcnt == 1) {
				for (int j = 0; j >= 0; j--) { // bit 값 출력
					newdata->bit[j] = temp % 2;
					temp /= 2;
				}
			}

			if (varcnt == 2) {
				for (int j = 1; j >= 0; j--) { // bit 값 출력
					newdata->bit[j] = temp % 2;
					temp /= 2;
				}
			}

			if (varcnt == 3) {
				for (int j = 2; j >= 0; j--) { // bit 값 출력
					newdata->bit[j] = temp % 2;
					temp /= 2;
				}
			}

			if(varcnt == 4){
				for (int j = 3; j >= 0; j--) { // bit 값 출력
					newdata->bit[j] = temp % 2;
					temp /= 2;
				}
			}

			while (s != NULL && checkBit(newdata->bit) >= checkBit(s->bit)) {
				p = p->next;
				s = p->next;
			}

			// NODE 연결
			newdata->next = s;
			p->next = newdata;
			p = column;
			s = p->next;
		}
	}
}

// bit의 1의 개수를 계산하기 위함
int checkBit(int bit[]){ 
	int cnt = 0;
	for (int i = 0; i < 4; i++)
		if (bit[i] == 1) cnt++;
	return cnt;
}

/* Table 출력 */
void printTable(TNODE* table){
	TNODE* s = table->next;

	while (s != NULL){
		printf_s("( "); // 사용된 numList
		for (int i = 0; i < NUM_MAX; i++){
			if (s->num_table[i] != -1) printf_s("%2d ", s->num_table[i]);
		} printf_s(") ");

		// numList에 해당하는 bit 출력
		if (varcnt == 1) {
			for (int i = 0; i < 1; i++) {
				if (s->bit[i] != -1) printf_s("%2d", s->bit[i]);
				else printf(" -");
			}
		}

		if (varcnt == 2) {
			for (int i = 0; i < 2; i++) {
				if (s->bit[i] != -1) printf_s("%2d", s->bit[i]);
				else printf(" -");
			}
		}

		if (varcnt == 3) {
			for (int i = 0; i < 3; i++) {
				if (s->bit[i] != -1) printf_s("%2d", s->bit[i]);
				else printf(" -");
			}
		}

		if (varcnt == 4) {
			for (int i = 0; i < 4; i++) {
				if (s->bit[i] != -1) printf_s("%2d", s->bit[i]);
				else printf(" -");
			}
		} printf_s("\n");

		s = s->next;
	}
}

/* 결정된 PI 출력 */
void printPI(TNODE* PI, int **arr, int result) {  
	TNODE* s = PI->next;
	int j = 0;
	int max;

	/* PI Graph 개수 */
	if (result == 1) max = 2;
	else if (result == 2) max = 4;
	else if (result == 3) max = 8;
	else max = 16;

	while (s != NULL) {
		printf_s("("); // PI Num
		for (int i = 0; i < max; i++) {
			if (s->num_table[i] != -1) printf_s("%2d ", s->num_table[i]);
			else printf("  ,");
		} printf_s(") ");

		/* Draw PI Graph */
		// 변수 개수에 따른 num 출력 범위 제한
		if (varcnt == 1) {
			for (int i = 0; i < NUM_MAX / 8; i++) { 
				if (arr[j][i] == 1 || arr[j][i] == 2) printf_s(" x,");
				else printf("  ,");
			}
		}

		if (varcnt == 2) {
			for (int i = 0; i < NUM_MAX / 4; i++) { 
				if (arr[j][i] == 1 || arr[j][i] == 2) printf_s(" x,");
				else printf("  ,");
			}
		}

		if (varcnt == 3) {
			for (int i = 0; i < NUM_MAX / 2; i++) {
				if (arr[j][i] == 1 || arr[j][i] == 2) printf_s(" x,");
				else printf("  ,");
			}
		}

		if (varcnt == 4) {
			for (int i = 0; i < NUM_MAX; i++) { 
				if (arr[j][i] == 1 || arr[j][i] == 2) printf_s(" x,");
				else printf("  ,");
			}
		} printf_s("\n"); j++;
		s = s->next;
	}
}

/* Table 병합 */ 
void mergeTable(TNODE* column_1, TNODE* column_2) {
	TNODE* p = column_1->next;	if (p == NULL) return; // not column 
	TNODE* s = p->next;
	TNODE* p_2 = column_2;
	TNODE* s_2 = p_2->next;
	int cnt = 0;
	int term = 0;

	while (p->next != NULL){ 
		while (s != NULL && checkBit(p->bit) - checkBit(s->bit) >= -1){  
			for (int i = 0; i < 4; i++) // 서로 간 bit 차이 count
				if (p->bit[i] - s->bit[i] != 0) cnt++;

			if (cnt == 1){ // 서로 간 bit 차이가 1개 발생 시
				TNODE* newdata = (TNODE*)malloc(sizeof(TNODE));
				memset(newdata, 0, sizeof(TNODE));

				for (int i = 0; i < 4; i++){ 
					if (p->bit[i] == s->bit[i]) newdata->bit[i] = s->bit[i]; // 같은 bit
					else newdata->bit[i] = -1; // 차이 발생 bit
				}

				/* numtable 병합 */
				for (int i = 0; i < 8; i++){
					if (p->num_table[i] != -1){
						newdata->num_table[i] = p->num_table[i];
						term = i;
					} else {
						term = i;
						i = 8; // break;
					}
				}
				for (int i = 0; term + i < NUM_MAX; i++){
					if (s->num_table[i] != -1) newdata->num_table[term + i] = s->num_table[i];
					else newdata->num_table[term + i] = -1;
				}

				// 병합 결과 저장
				newdata->next = s_2; 
				p_2->next = newdata;
				p_2 = newdata;

				freeNode(newdata);
			}
			s = s->next; 
			cnt = 0;
		}
		p = p->next; 
		s = p->next;
	}
}

/* 중복된 Table의 제거 */
void delOverlap(TNODE* head){
	TNODE* temp = head->next;
	TNODE* p = head->next;
	TNODE* s = p->next;
	int cnt = 0;

	while (p != NULL) {
		while (s != NULL && checkBit(p->bit) - checkBit(s->bit) == 0) { 
			for (int i = 0; i < 4; i++)  // 서로간 동일 bit count
				if (p->bit[i] == s->bit[i]) cnt++;

			if (cnt == 4) { // 완전하게 동일한 bit
				temp->next = s->next;
				free(s);
				s = temp->next;
			} else { // 이외의 경우
				temp = temp->next; 
				if (temp != NULL) s = temp->next;
			} cnt = 0;
		}

		temp = p->next; 
		p = p->next;
		if (p != NULL) s = p->next;
	}
}

/* PI 결정 */
// 적합한 를 결정하기 위한
void correctPI(TNODE* column_one, TNODE* column_two, TNODE* PI) {
	TNODE* s_o = column_one->next;
	TNODE* s_t = column_two->next;
	TNODE* p = PI;
	TNODE* s = p->next;
	int i = 0;
	int j = 0;
	int cnt = 0;

	// 병합 한 단계 이전 항의 PI 연결 시
	while (s_o != NULL) { 
		while (s_t != NULL) {
			// 사용되어진 Node 검색 후 제거 작업
			while (s_o->num_table[j] != -1 && j++ < NUM_MAX) {
				while (s_t->num_table[i] != -1 && i++ < NUM_MAX)
					if (s_o->num_table[j - 1] == s_t->num_table[i - 1]) cnt++;
				i = 0;
			} if (j == cnt) s_t = NULL; 

			if (s_t != NULL && s_t->next == NULL) { // 결정된 PI Node 연결
				TNODE* newdata = (TNODE*)malloc(sizeof(TNODE));
				memset(newdata, 0, sizeof(TNODE));
				for (i = 0; i < 16; i++) newdata->num_table[i] = s_o->num_table[i];
				for (i = 0; i < 4; i++)	newdata->bit[i] = s_o->bit[i];
				newdata->next = s; 
				p->next = newdata;
				p = p->next;

				freeNode(newdata);
			}
			cnt = 0;
			j = 0;
			i = 0;
			if (s_t != NULL) s_t = s_t->next;
		}
		s_o = s_o->next;
		s_t = column_two->next;
	}
	
	// 최종 병합 항의 PI 연결 시
	s_t = column_two->next;
	while (s_t != NULL) { // 결정된 PI Node 연결
		TNODE* newdata = (TNODE*)malloc(sizeof(TNODE));
		memset(newdata, 0, sizeof(TNODE));
		for (int i = 0; i < 16; i++) newdata->num_table[i] = s_t->num_table[i];
		for (int i = 0; i < 4; i++)	newdata->bit[i] = s_t->bit[i];
		newdata->next = s;
		p->next = newdata;
		p = p->next;
		s_t = s_t->next;

		freeNode(newdata);
	}
}

/* EPI 결정을 위한 테이블 작성 */
void circuitType(TNODE* PI){
	TNODE* s = PI->next;
	int cnt = 0;
	int i = 0;
	int j = 0;

	while (s != NULL){ // EPI 그룹 작성
		while (s->num_table[i] != -1 && i++ < NUM_MAX)
			epiTable[s->num_table[i - 1]] = 1;
		epiTableHeight++;
		s = s->next;
	}
	epi = (int**)malloc(sizeof(int*)*epiTableHeight);
	for (i = 0; i < epiTableHeight; i++) {
		epi[i] = (int*)malloc(sizeof(int) * NUM_MAX);
		memset(epi[i], 0, sizeof(int)*NUM_MAX);
	}

	s = PI->next; 
	while (s != NULL) { // EPI 회로 작성
		while (s->num_table[j] != -1 && j++ < NUM_MAX) {
			for (i = 0; i < NUM_MAX; i++)
				if (s->num_table[j - 1] == i)
					if (dtmTable[i] == 1) continue; // Don't Care Condition
					else epi[cnt][i] = 1;
		}
		cnt++;
		j = 0;
		s = s->next;
	}
}

/* EPI 결정 */
void correctEPI() {
	int cnt = 0;

	for (int j = 0; j < NUM_MAX; j++) { // EPI 추출
		for (int i = 0; i < epiTableHeight; i++)
			if (epi[i][j] == 1) cnt++;

		if (cnt == 1) {
			for (int i = 0; i < epiTableHeight; i++)
				if (epi[i][j] == 1) epi[i][j] = 2;
		} cnt = 0;
	}

	for (int j = 0; j < epiTableHeight; j++) {// EPI에서 가로제거작업
		for (int i = 0; i < NUM_MAX; i++){
			if (epi[j][i] == 2)	{
				for (int k = 0; k < NUM_MAX; k++)
					if (epi[j][k] == 1) epi[j][k] = -1;
			}
		}
	}

	for (int j = 0; j < NUM_MAX; j++) { // EPI 제거된 가로에서 세로제거
		for (int i = 0; i < epiTableHeight; i++) {
			if (epi[i][j] == -1) {
				for (int k = 0; k < epiTableHeight; k++)
					if (epi[k][j] == 1) epi[k][j] = -2;
			}
		}
	} cnt = 0;

	for (int j = 0; j < epiTableHeight; j++) { // 제거된 것을 제외한 나머지 EPI 추출
		for (int i = 0; i < NUM_MAX; i++)
			if (epi[j][i] == -1 || epi[j][i] == -2) cnt++;

		if (cnt == 0) {
			for (int i = 0; i < NUM_MAX; i++)
				if (epi[j][i] == 1) epi[j][i] = 2;
		} else {
			for (int i = 0; i < NUM_MAX; i++)
				if (epi[j][i] == 1) epi[j][i] = -2;
			cnt = 0;
		}
	}
}

/* Func 표현식 출력 */
void printFunc(TNODE *PI) {
	TNODE* s = PI->next;
	int cnt = 0; int expn = 0;
	int EPI = 0;

	// 사용자 함수명 출력
	printf_s("%s (",funcName);
	for (int i = 0; i < varcnt; i++) {
		if(i == varcnt - 1) printf_s("%s", varTable[i]);
		else printf_s("%s,", varTable[i]);
	} printf_s(") =");

	while (s != NULL) {
		for (int i = 0; i < NUM_MAX; i++)
			if (epi[cnt][i] == 2) EPI = 1;

		if (EPI == 1) { // 사용자 변수 출력
			if (expn != 0 && cnt != 0) printf_s(" + "); // "+" 연산자 처음 출력 방지
			for (int i = 0; i < varcnt; i++) {
				if (s->bit[i] == 0) printf_s("%s'", varTable[i]);
				else if (s->bit[i] == 1) printf("%s", varTable[i]);
			} EPI = 0; expn++;
		} s = s->next;
		cnt++;
	} printf("\n");
}
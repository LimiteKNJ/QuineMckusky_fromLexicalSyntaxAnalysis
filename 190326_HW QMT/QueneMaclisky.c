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

/* token Read �� Tabular method */
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
		} else { // Exception : ���� ������ ���� �Է°� �ʰ�
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
		} else { // Exception : ���� ������ ���� �Է°� �ʰ�
			errNum = NUM_DTM_OUT_OF_RANGE;
			err(NUM_DTM_OUT_OF_RANGE);
			break;
		}
	}

	/* Number Table -> �ߺ� ���� Number Table */
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

	// Exception : �Լ� ���� X
	if (funccnt == 0) { // ex. () = <expr>
		errNum = FUNC_NOT_FOUND;
		err(FUNC_NOT_FOUND);
	}

	// Exception : 2�� �̻��� �Լ� ����
	if (funccnt > 1) { // ex. func() + func() = <expr>
		errNum = FUNC_OUT_OF_RANGE;
		err(FUNC_OUT_OF_RANGE);
	}

	// Exception : + ������ ��� X
	if (pcnt == 0 && funccnt != 1) { // ex. func() = min() dtm()
		errNum = OP_PLUS_NOT_FOUND;
		err(OP_PLUS_NOT_FOUND);
	}

	// Exception : = ������ ��� X
	if (ecnt == 0) { // ex. func() min() + dtm()
		errNum = OP_EQUALS_NOT_FOUND;
		err(OP_EQUALS_NOT_FOUND);
	}

	// ex. func() = min() + dtm() or min() + dtm() = func() ... �̿ܿ� ���� ����ó��
	if (ecnt == 1 && !(qsynTable[1] == Q_EQUALS || qsynTable[syncnt - 2] == Q_EQUALS)) {
		errNum = OP_EQUALS_LOCATION_ER;
		err(OP_EQUALS_LOCATION_ER);
	}

	// ex. func() = min() + dtm() = or min() + dtm() = func() = ... �̿ܿ� ���� ����ó��
	if (ecnt == 2 && !((qsynTable[1] == Q_EQUALS && qsynTable[syncnt - 1] == Q_EQUALS) ||
		(qsynTable[syncnt - 3] == Q_EQUALS && qsynTable[syncnt - 1] == Q_EQUALS))) {
		errNum = OP_EQUALS_LOCATION_ER;
		err(OP_EQUALS_LOCATION_ER);
	}

	// Exception : = ������ �ߺ� ���
	if (ecnt > 2) { // ex. func() == min() + dtm() =
		errNum = OP_EQUALS_OVER_2;
		err(OP_EQUALS_OVER_2);
	}

	funccnt = 0; mincnt = 0; dtmcnt = 0;
	pcnt = 0; ecnt = 0;
}

/* var�� ���� ���� �� �Է� ��� ���� */
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

/* Table �ۼ� */
void makeTable(TNODE* column) {
	TNODE* p = column;
	TNODE* s = p->next;
	int temp;

	for (int i = 0; i < NUM_MAX; i++) {
		if (selTable[i] == 1) {
			TNODE* newdata = (TNODE*)malloc(sizeof(TNODE));

			// ó�� ���� ������ ���� -1�� �ʱ�ȭ
			newdata->num_table[0] = i;
			for (int j = 1; j < NUM_MAX; j++) {
				newdata->num_table[j] = -1;
			} temp = i;

			/* dec -> bin ���� */
			// ���� ������ ���� bit ��� �ڸ��� ���� 
			if (varcnt == 1) {
				for (int j = 0; j >= 0; j--) { // bit �� ���
					newdata->bit[j] = temp % 2;
					temp /= 2;
				}
			}

			if (varcnt == 2) {
				for (int j = 1; j >= 0; j--) { // bit �� ���
					newdata->bit[j] = temp % 2;
					temp /= 2;
				}
			}

			if (varcnt == 3) {
				for (int j = 2; j >= 0; j--) { // bit �� ���
					newdata->bit[j] = temp % 2;
					temp /= 2;
				}
			}

			if(varcnt == 4){
				for (int j = 3; j >= 0; j--) { // bit �� ���
					newdata->bit[j] = temp % 2;
					temp /= 2;
				}
			}

			while (s != NULL && checkBit(newdata->bit) >= checkBit(s->bit)) {
				p = p->next;
				s = p->next;
			}

			// NODE ����
			newdata->next = s;
			p->next = newdata;
			p = column;
			s = p->next;
		}
	}
}

// bit�� 1�� ������ ����ϱ� ����
int checkBit(int bit[]){ 
	int cnt = 0;
	for (int i = 0; i < 4; i++)
		if (bit[i] == 1) cnt++;
	return cnt;
}

/* Table ��� */
void printTable(TNODE* table){
	TNODE* s = table->next;

	while (s != NULL){
		printf_s("( "); // ���� numList
		for (int i = 0; i < NUM_MAX; i++){
			if (s->num_table[i] != -1) printf_s("%2d ", s->num_table[i]);
		} printf_s(") ");

		// numList�� �ش��ϴ� bit ���
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

/* ������ PI ��� */
void printPI(TNODE* PI, int **arr, int result) {  
	TNODE* s = PI->next;
	int j = 0;
	int max;

	/* PI Graph ���� */
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
		// ���� ������ ���� num ��� ���� ����
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

/* Table ���� */ 
void mergeTable(TNODE* column_1, TNODE* column_2) {
	TNODE* p = column_1->next;	if (p == NULL) return; // not column 
	TNODE* s = p->next;
	TNODE* p_2 = column_2;
	TNODE* s_2 = p_2->next;
	int cnt = 0;
	int term = 0;

	while (p->next != NULL){ 
		while (s != NULL && checkBit(p->bit) - checkBit(s->bit) >= -1){  
			for (int i = 0; i < 4; i++) // ���� �� bit ���� count
				if (p->bit[i] - s->bit[i] != 0) cnt++;

			if (cnt == 1){ // ���� �� bit ���̰� 1�� �߻� ��
				TNODE* newdata = (TNODE*)malloc(sizeof(TNODE));
				memset(newdata, 0, sizeof(TNODE));

				for (int i = 0; i < 4; i++){ 
					if (p->bit[i] == s->bit[i]) newdata->bit[i] = s->bit[i]; // ���� bit
					else newdata->bit[i] = -1; // ���� �߻� bit
				}

				/* numtable ���� */
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

				// ���� ��� ����
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

/* �ߺ��� Table�� ���� */
void delOverlap(TNODE* head){
	TNODE* temp = head->next;
	TNODE* p = head->next;
	TNODE* s = p->next;
	int cnt = 0;

	while (p != NULL) {
		while (s != NULL && checkBit(p->bit) - checkBit(s->bit) == 0) { 
			for (int i = 0; i < 4; i++)  // ���ΰ� ���� bit count
				if (p->bit[i] == s->bit[i]) cnt++;

			if (cnt == 4) { // �����ϰ� ������ bit
				temp->next = s->next;
				free(s);
				s = temp->next;
			} else { // �̿��� ���
				temp = temp->next; 
				if (temp != NULL) s = temp->next;
			} cnt = 0;
		}

		temp = p->next; 
		p = p->next;
		if (p != NULL) s = p->next;
	}
}

/* PI ���� */
// ������ �� �����ϱ� ����
void correctPI(TNODE* column_one, TNODE* column_two, TNODE* PI) {
	TNODE* s_o = column_one->next;
	TNODE* s_t = column_two->next;
	TNODE* p = PI;
	TNODE* s = p->next;
	int i = 0;
	int j = 0;
	int cnt = 0;

	// ���� �� �ܰ� ���� ���� PI ���� ��
	while (s_o != NULL) { 
		while (s_t != NULL) {
			// ���Ǿ��� Node �˻� �� ���� �۾�
			while (s_o->num_table[j] != -1 && j++ < NUM_MAX) {
				while (s_t->num_table[i] != -1 && i++ < NUM_MAX)
					if (s_o->num_table[j - 1] == s_t->num_table[i - 1]) cnt++;
				i = 0;
			} if (j == cnt) s_t = NULL; 

			if (s_t != NULL && s_t->next == NULL) { // ������ PI Node ����
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
	
	// ���� ���� ���� PI ���� ��
	s_t = column_two->next;
	while (s_t != NULL) { // ������ PI Node ����
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

/* EPI ������ ���� ���̺� �ۼ� */
void circuitType(TNODE* PI){
	TNODE* s = PI->next;
	int cnt = 0;
	int i = 0;
	int j = 0;

	while (s != NULL){ // EPI �׷� �ۼ�
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
	while (s != NULL) { // EPI ȸ�� �ۼ�
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

/* EPI ���� */
void correctEPI() {
	int cnt = 0;

	for (int j = 0; j < NUM_MAX; j++) { // EPI ����
		for (int i = 0; i < epiTableHeight; i++)
			if (epi[i][j] == 1) cnt++;

		if (cnt == 1) {
			for (int i = 0; i < epiTableHeight; i++)
				if (epi[i][j] == 1) epi[i][j] = 2;
		} cnt = 0;
	}

	for (int j = 0; j < epiTableHeight; j++) {// EPI���� ���������۾�
		for (int i = 0; i < NUM_MAX; i++){
			if (epi[j][i] == 2)	{
				for (int k = 0; k < NUM_MAX; k++)
					if (epi[j][k] == 1) epi[j][k] = -1;
			}
		}
	}

	for (int j = 0; j < NUM_MAX; j++) { // EPI ���ŵ� ���ο��� ��������
		for (int i = 0; i < epiTableHeight; i++) {
			if (epi[i][j] == -1) {
				for (int k = 0; k < epiTableHeight; k++)
					if (epi[k][j] == 1) epi[k][j] = -2;
			}
		}
	} cnt = 0;

	for (int j = 0; j < epiTableHeight; j++) { // ���ŵ� ���� ������ ������ EPI ����
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

/* Func ǥ���� ��� */
void printFunc(TNODE *PI) {
	TNODE* s = PI->next;
	int cnt = 0; int expn = 0;
	int EPI = 0;

	// ����� �Լ��� ���
	printf_s("%s (",funcName);
	for (int i = 0; i < varcnt; i++) {
		if(i == varcnt - 1) printf_s("%s", varTable[i]);
		else printf_s("%s,", varTable[i]);
	} printf_s(") =");

	while (s != NULL) {
		for (int i = 0; i < NUM_MAX; i++)
			if (epi[cnt][i] == 2) EPI = 1;

		if (EPI == 1) { // ����� ���� ���
			if (expn != 0 && cnt != 0) printf_s(" + "); // "+" ������ ó�� ��� ����
			for (int i = 0; i < varcnt; i++) {
				if (s->bit[i] == 0) printf_s("%s'", varTable[i]);
				else if (s->bit[i] == 1) printf("%s", varTable[i]);
			} EPI = 0; expn++;
		} s = s->next;
		cnt++;
	} printf("\n");
}
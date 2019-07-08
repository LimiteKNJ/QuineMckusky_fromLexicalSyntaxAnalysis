#include "SyntaxAnalysis.h"

/* 구문 분석기 */
void readLexToken(tokenData* tokenList) {

	char* temp = (char*)malloc(sizeof(char)*ID_LENGTH);
	memset(temp, 0, sizeof(char)*ID_LENGTH);
	memset(qsynTable, 0, sizeof(int)*LIST_LENGTH);
	tListIndex = 0; sListIndex = 0; syncnt = 0;
	int i = 0; parenStatus = P_CLOSED;

	while (tokenList[tListIndex].type != NULL) {
		if (tokenList[tListIndex].type == T_IDENTIFIER && parenStatus == P_CLOSED) { // FuncName
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; 
			if (!strcmp(temp, "sumMin") || !strcmp(temp, "sum_min") || !strcmp(temp, "min")) {
				storeSyn(S_MINFUNC, temp); funcStatus = F_MIN; qsynTable[syncnt] = Q_MIN;
			}
			else if (!strcmp(temp, "sumDtm") || !strcmp(temp, "sum_dtm") || !strcmp(temp, "dtm")) {
				storeSyn(S_DTMFUNC, temp); funcStatus = F_DTM; qsynTable[syncnt] = Q_DTM;
			}
			else {
				storeSyn(S_FUNCTION_NAME, temp); funcStatus = 0;
				qsynTable[syncnt] = Q_FUNC;
			} syncnt++;
		}

		if (tokenList[tListIndex].type == T_IDENTIFIER && parenStatus == P_OPENED) { // variable
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_VARIABLE, temp);
		}

		if (tokenList[tListIndex].type == T_NUMBER && parenStatus == P_CLOSED) { // const
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_CONST, temp);
		}

		if (tokenList[tListIndex].type == T_NUMBER && parenStatus == P_OPENED && funcStatus == 0) {
			errNum = VAR_NOT_INPUT_NUM;
			err(VAR_NOT_INPUT_NUM);
			break;
		}

		if (tokenList[tListIndex].type == T_NUMBER && parenStatus == P_OPENED && funcStatus == F_MIN) { // min_num
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_MNUMBER, temp);
		}

		if (tokenList[tListIndex].type == T_NUMBER && parenStatus == P_OPENED && funcStatus == F_DTM) { // dtm_num
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_DNUMBER, temp);
		}

		if (tokenList[tListIndex].type == T_PLUS && parenStatus == P_CLOSED) { // +
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_OPERATOR, temp);
			qsynTable[syncnt] = Q_PLUS; syncnt++;
		}

		if (tokenList[tListIndex].type == T_PLUS && parenStatus == P_OPENED) {
			errNum = PARENT_IN_PLUS;
			err(PARENT_IN_PLUS);
			break;
		}

		if (tokenList[tListIndex].type == T_MINUS) {
			errNum = NOT_SUPPORT_OP_MINUS;
			err(NOT_SUPPORT_OP_MINUS);
			break;
		}

		if (tokenList[tListIndex].type == T_MUL) {
			errNum = NOT_SUPPORT_OP_MUL;
			err(NOT_SUPPORT_OP_MUL);
			break;
		}

		if (tokenList[tListIndex].type == T_DIV) {
			errNum = NOT_SUPPORT_OP_DIV;
			err(NOT_SUPPORT_OP_DIV);
			break;
		}

		if (tokenList[tListIndex].type == T_MOD) {
			errNum = NOT_SUPPORT_OP_MOD;
			err(NOT_SUPPORT_OP_MOD);
			break;
		}
		
		if (tokenList[tListIndex].type == T_EQUALS && parenStatus == P_CLOSED) { // =
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_OPERATOR, temp);
			qsynTable[syncnt] = Q_EQUALS; syncnt++;
		}

		if (tokenList[tListIndex].type == T_EQUALS && parenStatus == P_OPENED) { // exception
			errNum = PARENT_IN_EQUALS;
			err(PARENT_IN_EQUALS);
			break;
		}

		if (tokenList[tListIndex].type == T_AND) {
			errNum = NOT_SUPPORT_OP_AND;
			err(NOT_SUPPORT_OP_AND);
			break;
		}

		if (tokenList[tListIndex].type == T_OR) {
			errNum = NOT_SUPPORT_OP_OR;
			err(NOT_SUPPORT_OP_OR);
			break;
		}

		if (tokenList[tListIndex].type == T_NOT) {
			errNum = NOT_SUPPORT_OP_NOT;
			err(NOT_SUPPORT_OP_NOT);
			break;
		}

		if (tokenList[tListIndex].type == T_LPAREN && parenStatus == P_OPENED) { // excption : not closed
			errNum = PARENT_NOT_CLOSED;
			err(PARENT_NOT_CLOSED);
			break;
		}

		if (tokenList[tListIndex].type == T_LPAREN && parenStatus == P_CLOSED) { // (
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; 
			i = 0; 
			storeSyn(S_POPEN, temp);
			parenStatus = P_OPENED;
		}

		if (tokenList[tListIndex].type == T_RPAREN && parenStatus == P_CLOSED) { // excption : not opened
			errNum = PARENT_NOT_OPENED;
			err(PARENT_NOT_OPENED);
			break;
		}

		if (tokenList[tListIndex].type == T_RPAREN && parenStatus == P_OPENED) { // )
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_PCLOSE, temp);
			parenStatus = P_CLOSED; funcStatus = 0;
		}

		if (tokenList[tListIndex].type == T_LBRACE || tokenList[tListIndex].type == T_RBRACE) {
			errNum = NOT_SUPPORT_BRACE;
			err(NOT_SUPPORT_BRACE);
			break;
		}

		if (tokenList[tListIndex].type == T_LBRACKET || tokenList[tListIndex].type == T_RBRACKET) {
			errNum = NOT_SUPPORT_BRACKET;
			err(NOT_SUPPORT_BRACKET);
			break;
		}

		if (tokenList[tListIndex].type == T_ENDS) {
			break;
		}

		/* Delimiter */
		if (tokenList[tListIndex].type == T_COMMA);
		if (tokenList[tListIndex].type == T_SEMICOLON);
		if (tokenList[tListIndex].type == T_UPPER);
		if (tokenList[tListIndex].type == T_DOUBLEQT);
		if (tokenList[tListIndex].type == T_SPACE);

		tListIndex++;
	}


	if (parenStatus != P_CLOSED && errNum == NULL) {
		errNum = PARENT_NOT_CLOSED;
		err(PARENT_NOT_CLOSED);
	}

	/* free temp */
	if (temp != NULL) {
		free(temp);
		temp = NULL;
	}
}

// 구문 분석 후 저장
void storeSyn(int type, char* data) {
	if (synData[sListIndex].type == NULL) { // 초기 Token 저장
		synData[sListIndex].type = type;
		for (int i = 0; data[i] != '\0'; i++)
			synData[sListIndex].value[i] = data[i];
	} else { // 이후의 Token 저장
		synData[sListIndex].type = type;
		for (int i = 0; synData[sListIndex].value[i] != '\0'; i++) // token.value 초기화
			synData[sListIndex].value[i] = '\0';
		for (int i = 0; data[i] != '\0'; i++)
			synData[sListIndex].value[i] = data[i];
	} sListIndex++;
}

/* Test 부분 */
// 구문 분석후 결과 추출
void printSynList(tokenData* tokenList) {
	for (int i = 0; i < LIST_LENGTH; i++) {
		if (tokenList[i].type == NULL) break;
		printf_s("[ %s / %s ]", getSynType(tokenList[i].type), tokenList[i].value);
		if (i % 5 == 4) printf_s("\n");
	} printf_s("\n");
}

// Type 결과 추출
char* getSynType(int type) {
	switch (type) {
		case S_VARIABLE: return "Variable"; break;
		case S_CONST: return "Construction"; break;
		case S_MNUMBER: return "Minterm Number"; break;
		case S_DNUMBER: return "Determine Number"; break;
		case S_OPERATOR: return "Operator"; break;
		case S_POPEN: return "Parentheses Open"; break;
		case S_PCLOSE: return "Parentheses Close"; break;
		case S_FUNCTION_NAME: return "Function Name"; break;
		case S_MINFUNC: return "Minterm Function Name"; break;
		case S_DTMFUNC: return "Determine Function Name"; break;
		default: return "Not Found"; break;
	}
}
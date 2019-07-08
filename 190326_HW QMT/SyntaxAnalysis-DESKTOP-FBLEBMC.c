#include "SyntaxAnalysis.h"

void readLexToken(tokenData* tokenList) {

	char* temp = (char*)malloc(sizeof(char)*ID_LENGTH);
	memset(temp, 0, sizeof(char)*ID_LENGTH);
	tListIndex = 0; sListIndex = 0;
	int i = 0; parenStatus = P_CLOSED;

	while (tokenList[tListIndex].type != NULL) {
		if (tokenList[tListIndex].type == T_IDENTIFIER && parenStatus == P_CLOSED) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; 
			if (!strcmp(temp, "sumMin") || !strcmp(temp, "sum_min") || !strcmp(temp, "min")) {
				storeSyn(S_MINFUNC, temp); funcStatus = F_MIN;
			}
			else if (!strcmp(temp, "sumDtm") || !strcmp(temp, "sum_dtm") || !strcmp(temp, "dtm")) {
				storeSyn(S_DTMFUNC, temp); funcStatus = F_DTM;
			}
			else {
				storeSyn(S_FUNCTION_NAME, temp); funcStatus = 0;
			}
		}

		if (tokenList[tListIndex].type == T_IDENTIFIER && parenStatus == P_OPENED) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_VARIABLE, temp);
		}
		
		if (tokenList[tListIndex].type == T_NUMBER && parenStatus == P_CLOSED) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_CONST, temp);
		}

		if (tokenList[tListIndex].type == T_NUMBER && parenStatus == P_OPENED && funcStatus == 0) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_CONST, temp);
		}

		if (tokenList[tListIndex].type == T_NUMBER && parenStatus == P_OPENED && funcStatus == F_MIN) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_MNUMBER, temp);
		}

		if (tokenList[tListIndex].type == T_NUMBER && parenStatus == P_OPENED && funcStatus == F_DTM) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_DNUMBER, temp);
		}

		if (tokenList[tListIndex].type == T_PLUS) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_OPERATOR, temp);
		}

		if (tokenList[tListIndex].type == T_MINUS) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_OPERATOR, temp);
		}

		if (tokenList[tListIndex].type == T_MUL) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_OPERATOR, temp);
		}

		if (tokenList[tListIndex].type == T_DIV) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_OPERATOR, temp);
		}

		if (tokenList[tListIndex].type == T_MOD) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_OPERATOR, temp);
		}
		
		if (tokenList[tListIndex].type == T_EQUALS) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_OPERATOR, temp);
		}

		if (tokenList[tListIndex].type == T_AND) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_OPERATOR, temp);
		}

		if (tokenList[tListIndex].type == T_OR) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_OPERATOR, temp);
		}

		if (tokenList[tListIndex].type == T_NOT) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_OPERATOR, temp);
		}

		if (tokenList[tListIndex].type == T_LPAREN && parenStatus == P_OPENED) {
			printf_s("Error : Parenthses is not closed.\n");
			break;
		}

		if (tokenList[tListIndex].type == T_LPAREN && parenStatus == P_CLOSED) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_POPEN, temp);
			parenStatus = P_OPENED;
		}

		if (tokenList[tListIndex].type == T_RPAREN && parenStatus == P_CLOSED) {
			printf_s("Error : Parenthses is not opened.\n");
			break;
		}

		if (tokenList[tListIndex].type == T_RPAREN && parenStatus == P_OPENED) {
			for (i = 0; tokenList[tListIndex].value[i] != '\0'; i++)
				temp[i] = tokenList[tListIndex].value[i];
			temp[i] = '\0'; i = 0; storeSyn(S_PCLOSE, temp);
			parenStatus = P_CLOSED; funcStatus = 0;
		}

		/* Delimiter */
		if (tokenList[tListIndex].type == T_COMMA);
		if (tokenList[tListIndex].type == T_SEMICOLON);
		if (tokenList[tListIndex].type == T_UPPER);
		if (tokenList[tListIndex].type == T_DOUBLEQT);
		if (tokenList[tListIndex].type == T_SPACE);

		tListIndex++;
	} // printSynList(synData);


	if (parenStatus == P_CLOSED)
		readTokenSuccess = TRUE;
	else {
		printf_s("Error : Parenthses is not closed.\n");
		readTokenSuccess = FALSE;
	}

	if (temp != NULL) {
		free(temp);
		temp = NULL;
	}
}

void storeSyn(int type, char* data) {
	if (synData[sListIndex].type == NULL) {
		synData[sListIndex].type = type;
		for (int i = 0; data[i] != '\0'; i++)
			synData[sListIndex].value[i] = data[i];
	}
	else {
		synData[sListIndex].type = type;
		for (int i = 0; synData[sListIndex].value[i] != '\0'; i++)
			synData[sListIndex].value[i] = '\0';
		for (int i = 0; data[i] != '\0'; i++)
			synData[sListIndex].value[i] = data[i];
	} sListIndex++;
}

void printSynList(tokenData* tokenList) {
	for (int i = 0; i < LIST_LENGTH; i++) {
		if (tokenList[i].type == NULL) break;
		printf_s("[ %s / %s ]", getSynType(tokenList[i].type), tokenList[i].value);
		if (i % 5 == 4) printf_s("\n");
	} printf_s("\n");
}

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
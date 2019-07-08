#include "LexicalAnalysis.h"

// ���� ���
void printTexts(char* string) {
	printf_s("============================\n");
	printf_s("> %s", string);
}

/* ���� �м��� */
void readTexts(char* string) {

	char* temp = (char*)malloc(sizeof(char)*ID_LENGTH);
	memset(temp, '\0', sizeof(char)*ID_LENGTH);
	int i = 0; int index = 0;
	tListIndex = 0;

	for (i = 0; string[i] != '\0'; i++) {

		if ((string[i] >= 'A' && string[i] <= 'Z') ||
			(string[i] >= 'a' && string[i] <= 'z') ||
			string[i] == '_') { temp[index] = string[i];
			while (1) { i++; index++;
				if ((string[i] >= 'A' && string[i] <= 'Z') ||
					(string[i] >= 'a' && string[i] <= 'z') ||
					(string[i] >= '0' && string[i] <= '9') ||
					string[i] == '_') temp[index] = string[i];
				else break;
			} temp[index] = '\0'; index = 0;
			storeTokenData(T_IDENTIFIER, temp);
		}

		if (string[i] >= '0' && string[i] <= '9') {
			temp[index] = string[i];
			while (1) {	i++; index++;
				if (string[i] >= '0' && string[i] <= '9') temp[index] = string[i];
				else break;
			} temp[index] = '\0'; index = 0;
			storeTokenData(T_NUMBER, temp);
		}

		if (string[i] == '(') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_LPAREN, temp);
		}

		if (string[i] == '{' ) {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_LBRACE, temp);
		}

		if (string[i] == '[') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_LBRACKET, temp);
		}

		if (string[i] == ')') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_RPAREN, temp);
		}

		if (string[i] == '}') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_RBRACE, temp);
		}

		if (string[i] == ']') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_RBRACKET, temp);
		}

		if (string[i] == '+') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_PLUS, temp);
		}

		if (string[i] == '-') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_MINUS, temp);
		}

		if (string[i] == '*') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_MUL, temp);
		}

		if (string[i] == '/') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_DIV, temp);
		}

		if (string[i] == '%') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_MOD, temp);
		}

		if (string[i] == '=') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_EQUALS, temp);
		}

		if (string[i] == '&') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_AND, temp);
		}

		if (string[i] == '|') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_OR, temp);
		}

		if (string[i] == '!') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_NOT, temp);
		}

		if (string[i] == '.') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_COMMA, temp);
		}

		if (string[i] == ',') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_SEMICOLON, temp);
		}

		if (string[i] == '\'') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_UPPER, temp);
		}

		if (string[i] == '\"') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_DOUBLEQT, temp);
		}

		if (string[i] == ';') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_ENDS, temp);
		}

		if (string[i] == ' ') {
			temp[index] = string[i]; index++;
			temp[index] = '\0'; index = 0;
			storeTokenData(T_SPACE, temp);
		} 
	}

	// free temp
	if (temp != NULL) {
		free(temp);
		temp = NULL;
	}
}

// ���� �м��� ��� ����
void storeTokenData(int type, char* string) {
	if (token[tListIndex].type == NULL) { // �ʱ� Token ����
		token[tListIndex].type = type; 
		for (int i = 0; string[i] != '\0'; i++)
			token[tListIndex].value[i] = string[i];
	} else { // ������ Token ����
		token[tListIndex].type = type;
		for (int i = 0; token[tListIndex].value[i] != '\0'; i++) // token.value �ʱ�ȭ
			token[tListIndex].value[i] = '\0';
		for (int i = 0; string[i] != '\0'; i++)
			token[tListIndex].value[i] = string[i];
	} tListIndex++;
}

/* Test ���� */
// ���� �м��� ��� ���
void printTokenList(tokenData* tokenList) {
	for (int i = 0; i < LIST_LENGTH; i++) {
		if (tokenList[i].type == NULL) break;
		printf_s("[ %s / %s ]", getTokenType(tokenList[i].type), tokenList[i].value);
		if (i % 5 == 4) printf_s("\n");
	} printf_s("\n");
}

// ���� �м��� Type Check
char* getTokenType(int type) {
	switch (type) {
		case T_IDENTIFIER : return "Identifier"; break;
		case T_NUMBER : return "Number"; break;
		case T_LPAREN : return "Parentheses"; break;
		case T_RPAREN : return "Parentheses"; break;
		case T_LBRACE : return "Brace"; break;
		case T_RBRACE: return "Brace"; break;
		case T_LBRACKET: return "Bracket"; break;
		case T_RBRACKET: return "Bracket"; break;
		case T_PLUS : return "Operator '+'"; break;
		case T_MINUS : return "Operator '-'"; break;
		case T_MUL: return "Operator '*'"; break;
		case T_DIV: return "Operator '/'"; break;
		case T_MOD: return "Operator '%'"; break;
		case T_EQUALS : return "Operator '='"; break;
		case T_AND: return "Operator '&'"; break;
		case T_OR: return "Operator '|'"; break;
		case T_NOT: return "Operator '!'"; break;
		case T_COMMA: return "Separator '.'"; break;
		case T_SEMICOLON: return "Separator ','"; break;
		case T_UPPER : return "Separator '''"; break;
		case T_DOUBLEQT: return "Separator '\"'"; break;
		case T_SPACE : return "Separator ' '"; break;
		case T_ENDS : return "End of Statement"; break;
		default: return "Not Found"; break;
	}
}

/* Free Token Structure */
void freeTokens(tokenData* list) {
	for (int i = 0; i < LIST_LENGTH; i++) {
		list[i].type = NULL;
		memset(list[i].value, '\0', sizeof(char) * ID_LENGTH);
	}
}
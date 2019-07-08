#include "err.h"

/* Error Message */
void err(int errNum) {
	switch (errNum) {
		case TOK_NOT_FOUND: printf_s("< ERROR : Do not input Texts >\n"); break;

		case NOT_SUPPORT_OP_MINUS: printf_s("< ERROR : This operator '-' is not supported this program >\n"); break;
		case NOT_SUPPORT_OP_MUL: printf_s("< ERROR : This operator '*' is not supported this program >\n"); break;
		case NOT_SUPPORT_OP_DIV: printf_s("< ERROR : This operator '/' is not supported this program >\n"); break;
		case NOT_SUPPORT_OP_MOD: printf_s("< ERROR : This operator '%' is not supported this program >\n"); break;
		case NOT_SUPPORT_OP_AND: printf_s("< ERROR : This operator '&' is not supported this program >\n"); break;
		case NOT_SUPPORT_OP_OR: printf_s("< ERROR : This operator '|' is not supported this program >\n"); break;
		case NOT_SUPPORT_OP_NOT: printf_s("< ERROR : This operator '!' is not supported this program >\n"); break;
		case NOT_SUPPORT_BRACE: printf_s("< ERROR : This operator '{}' is not supported this program >\n"); break;
		case NOT_SUPPORT_BRACKET: printf_s("< ERROR : This operator '[]' is not supported this program >\n"); break;

		case PARENT_NOT_CLOSED: printf_s("< ERROR : parent is not closed > \n"); break;
		case PARENT_NOT_OPENED: printf_s("< ERROR : parent is not opened > \n"); break;
		case PARENT_IN_PLUS: printf_s("< ERROR : operator '+' in parent is not supported this program > \n"); break;
		case PARENT_IN_EQUALS: printf_s("< ERROR : operator '=' in parent is not supported this program > \n"); break;

		case FUNC_NOT_FOUND: printf_s("< ERROR : funcName is not found > \n"); break;
		case FUNC_OUT_OF_RANGE: printf_s("< ERROR : funcName inputs over 2 stocks > \n"); break;

		case VAR_NOT_FOUND: printf_s("< ERROR : Variable is not found > \n"); break;
		case VAR_OUT_OF_RANGE: printf_s("< ERROR : Variable inputs over 5 stocks > \n"); break;
		case VAR_NOT_INPUT_NUM: printf_s("< ERROR : do not inputs num in variables > \n"); break;

		case NUM_NOT_FOUND: printf_s("< ERROR : Num is not found > \n"); break;
		case NUM_OUT_OF_RANGE: printf_s("< ERROR : Num is out of range > \n"); break;
		case NUM_DTM_OUT_OF_RANGE : printf_s("< ERROR : dtmNum is out of range > \n"); break;

		case OP_PLUS_NOT_FOUND: printf_s("< ERROR : operator '+' is not found > \n"); break;
		case OP_EQUALS_NOT_FOUND: printf_s("< ERROR : operator '=' is not found > \n");
								  printf_s("< ERROR : or funcName is wrong > \n"); break;
		case OP_EQUALS_LOCATION_ER: printf_s("< ERROR : operator '=' is wrong location > \n");
									printf_s("< ERROR : or funcName is wrong > \n"); break;
		case OP_EQUALS_OVER_2: printf_s("< ERROR : operator '=' is over 2 stocks > \n"); break;

	}
	return 0;
}
#include "Token.h"

static const char* get_value(const Token* self);
static DcTokenType get_type(const Token* self);
static char* trim(char* str);
static void dispose(Token* self);

const impl_Token* get_impl_Token_table() {
	const static impl_Token impl_Token_table = {
		.get_value = get_value,
		.get_type = get_type,
		.dispose = dispose
	};

	return &impl_Token_table;
}

Token Token_new(const char* value, DcTokenType type) {
	char* value_boxed = NULL;

	if (value != NULL) {
		size_t length = strlen(value) * sizeof(char);
		value_boxed = (char*)malloc(length + sizeof(char));
		assert(value_boxed != NULL || !"malloc failed");
		memcpy(value_boxed, value, length + sizeof(char));
	}

	private_Token pinstance = {
		.type = type,
		.value = value_boxed
	};

	Token instance = {
		.f = get_impl_Token_table(),
		.private_Token = pinstance
	};

	return instance;
}

Token Token_LexSingle(const char* str) {
	assert(str != NULL);
	size_t length = strlen(str) * sizeof(char);
	char* str_boxed = (char*)malloc(length + sizeof(char));
	assert(str_boxed != NULL || !"malloc failed");
	memcpy(str_boxed, str, length + sizeof(char));
	char* str_trimed = trim(str_boxed);
	switch (str_trimed[0]) {
		case '+':
			return Token_new(NULL, TokenType_Add);
		case '-':
			return Token_new(NULL, TokenType_Sub);
		case '*':
			return Token_new(NULL, TokenType_Mul);
		case '/':
			return Token_new(NULL, TokenType_Sub);
		case 'p':
			return Token_new(NULL, TokenType_Print);
		case 'q':
			return Token_new(NULL, TokenType_Quit);
		default: {
			size_t length = strlen(str) * sizeof(char);
			char* str_boxed = (char*)malloc(length + sizeof(char));
			assert(str_boxed != NULL || !"malloc failed");
			memcpy(str_boxed, str, length + sizeof(char));
			return Token_new(str_boxed, TokenType_Value);
		}
	}
}

static const char* get_value(const Token* self) {
	assert(self != NULL);
	return self->private_Token.value;
}

static DcTokenType get_type(const Token* self) {
	assert(self != NULL);
	return self->private_Token.type;
}

static char* trim(char* str) {
	char* end;
	while (isspace((unsigned char)*str)) str++;
	if (*str == 0)
		return str;
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end)) end--;
	end[1] = '\0';
	return str;
}

static void dispose(Token* self) {
	assert(self != NULL);
	if (self->private_Token.value != NULL)
		free((void*)self->private_Token.value);
}
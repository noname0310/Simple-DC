#pragma once
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

struct private_Token;
typedef struct private_Token private_Token;

struct impl_Token;
typedef struct impl_Token impl_Token;

struct Token;
typedef struct Token Token;

enum DcTokenType;
typedef enum DcTokenType DcTokenType;

struct private_Token {
    const char* value;
    DcTokenType type;
};

struct impl_Token {
    const char* (*const get_value)(const Token* self);
    DcTokenType (*const get_type)(const Token* self);
	void (*const dispose)(Token* self);
};

struct Token {
    const impl_Token* const f;
    private_Token private_Token;
};

enum DcTokenType {
    TokenType_Add,
    TokenType_Sub,
    TokenType_Mul,
    TokenType_Div,
    TokenType_Print,
    TokenType_Quit,
    TokenType_Value
};

Token Token_LexSingle(const char* str);

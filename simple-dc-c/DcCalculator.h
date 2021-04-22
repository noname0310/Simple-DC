#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Result.h"
#include "DcResult.h"
#include "Token.h"

struct private_DcCalculator;
typedef struct private_DcCalculator private_DcCalculator;

struct impl_DcCalculator;
typedef struct impl_DcCalculator impl_DcCalculator;

struct DcCalculator;
typedef struct DcCalculator DcCalculator;

struct private_DcCalculator {
	int* stack;
	int count;
	int capacity;
};

struct impl_DcCalculator {
	DcResult (*const step)(DcCalculator* self, const Token* token);
	void (*const dispose)(DcCalculator* self);
};

struct DcCalculator {
	const impl_DcCalculator* const f;
	private_DcCalculator private_DcCalculator;
};

const impl_DcCalculator* get_impl_DcCalculator_table();
DcCalculator DcCalculator_new();

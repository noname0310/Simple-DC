#include "DcCalculator.h"

static DcResult step(DcCalculator* self, const Token* token);
static DcResult check_stack_size(const DcCalculator* self);
static void push(DcCalculator* self, int value);
static int pop(DcCalculator* self);
static int peek(const DcCalculator* self);
static void dispose(DcCalculator* self);

const impl_DcCalculator* get_impl_DcCalculator_table() {
	static impl_DcCalculator impl_Vector2_table = {
		.step = step,
		.dispose = dispose
	};

	return &impl_Vector2_table;
}

DcCalculator DcCalculator_new() {
	const default_capacity = 8;

	private_DcCalculator pinstance = {
		.stack = (int*)malloc(sizeof(int) * default_capacity),
		.count = 0,
		.capacity = default_capacity
	};

	DcCalculator instance = {
		.f = get_impl_DcCalculator_table(),
		.private_DcCalculator = pinstance
	};

	return instance;
}

DcResult step(DcCalculator* self, const Token* token) {
	assert(self != NULL);
	assert(token != NULL);
	
	switch (token->f->get_type(token))
	{
		case TokenType_Add: {
			DcResult dcResult = check_stack_size(self);
			if (dcResult.f->get_result(&dcResult) == Result_Err) return dcResult;
			dcResult.f->dispose(&dcResult);
			push(self, pop(self) + pop(self));
			return DcResult_new(Result_Ok, NULL);
		}
		case TokenType_Sub: {
			DcResult dcResult = check_stack_size(self);
			if (dcResult.f->get_result(&dcResult) == Result_Err) return dcResult;
			dcResult.f->dispose(&dcResult);
			push(self, pop(self) - pop(self));
			return DcResult_new(Result_Ok, NULL);
		}
		case TokenType_Mul: {
			DcResult dcResult = check_stack_size(self);
			if (dcResult.f->get_result(&dcResult) == Result_Err) return dcResult;
			dcResult.f->dispose(&dcResult);
			push(self, pop(self) * pop(self));
			return DcResult_new(Result_Ok, NULL);
		}
		case TokenType_Div: {
			DcResult dcResult = check_stack_size(self);
			if (dcResult.f->get_result(&dcResult) == Result_Err) return dcResult;
			dcResult.f->dispose(&dcResult);
			push(self, pop(self) / pop(self));
			return DcResult_new(Result_Ok, NULL);
		}
		case TokenType_Print:
			if (self->private_DcCalculator.count != 0) {
				char buf[11];
				snprintf(buf, sizeof(buf), "%d", peek(self));
				return DcResult_new(Result_Ok, buf);
			}
			else
				return DcResult_new(Result_Ok, "EOF");
		case TokenType_Quit:
			return DcResult_new(Result_Ok, "q");
		case TokenType_Value: {
			char* endptr;
			long ret = strtol(token->f->get_value(token), &endptr, 10);
			if (token->f->get_value(token) != endptr)
			{
				push(self, (int)ret);
				return DcResult_new(Result_Ok, NULL);
			}
			else
				return DcResult_new(Result_Err, "Invalid token");
		}
		default:
			return DcResult_new(Result_Err, "Invalid token");
	}
}

static DcResult check_stack_size(const DcCalculator* self) {
	assert(self != NULL);
    if (self->private_DcCalculator.count < 2)
        return DcResult_new(Result_Err, "stack.Count < 2");
    else
        return DcResult_new(Result_Ok, NULL);
}

static void push(DcCalculator* self, int value) {
	assert(self != NULL);
	if (self->private_DcCalculator.count == self->private_DcCalculator.capacity) {
		void* t = self->private_DcCalculator.stack;
		self->private_DcCalculator.stack = (int*)realloc(self->private_DcCalculator.stack, self->private_DcCalculator.capacity * 2);
		if (self->private_DcCalculator.stack == NULL) {
			free(t);
			assert(self->private_DcCalculator.stack != NULL || !"malloc failed");
		}
	}
	self->private_DcCalculator.stack[self->private_DcCalculator.count] = value;
	self->private_DcCalculator.count += 1;
}

static int pop(DcCalculator* self) {
	assert(self != NULL);
	assert(self->private_DcCalculator.count != 0);
	int value = self->private_DcCalculator.stack[self->private_DcCalculator.count - 1];
	self->private_DcCalculator.count -= 1;
	return value;
}

static int peek(const DcCalculator* self) {
	assert(self != NULL);
	assert(self->private_DcCalculator.count != 0);
	return self->private_DcCalculator.stack[self->private_DcCalculator.count - 1];
}

static void dispose(DcCalculator* self) {
	assert(self != NULL);
	free(self->private_DcCalculator.stack);
}

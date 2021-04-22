#include "DcResult.h"

static Result get_result(const DcResult* self);
static const char* get_message(const DcResult* self);
static void dispose(DcResult* self);

const impl_DcResult* get_impl_DcResult_table() {
	static impl_DcResult impl_DcResult_table = {
		.get_result = get_result,
		.get_message = get_message,
		.dispose = dispose
	};

	return &impl_DcResult_table;
}

DcResult DcResult_new(Result result, const char* message) {
	char* message_boxed = NULL;

	if (message != NULL) {
		size_t length = strlen(message) * sizeof(char);
		message_boxed = (char*)malloc(length + sizeof(char));
		assert(message_boxed != NULL || !"malloc failed");
		memcpy(message_boxed, message, length + sizeof(char));
	}

	private_DcResult pinstance = {
		.result = result,
		.message = message_boxed
	};

	DcResult instance = {
		.f = get_impl_DcResult_table(),
		.private_DcResult = pinstance
	};

	return instance;
}

static Result get_result(const DcResult* self) {
	assert(self != NULL);
	return self->private_DcResult.result;
}

static const char* get_message(const DcResult* self) {
	assert(self != NULL);
	return self->private_DcResult.message;
}

static void dispose(DcResult* self) {
	assert(self != NULL);
	if (self->private_DcResult.message != NULL)
		free((void*)self->private_DcResult.message);
}
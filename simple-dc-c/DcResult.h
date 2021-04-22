#pragma once
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Result.h"

struct private_DcResult;
typedef struct private_DcResult private_DcResult;

struct impl_DcResult;
typedef struct impl_DcResult impl_DcResult;

struct DcResult;
typedef struct DcResult DcResult;

struct private_DcResult {
    Result result;
    const char* message;
};

struct impl_DcResult {
    Result (*const get_result)(const DcResult* self);
    const char* (*const get_message)(const DcResult* self);
	void (*const dispose)(DcResult* self);
};

struct DcResult {
    const impl_DcResult* const f;
    private_DcResult private_DcResult;
};

const impl_DcResult* get_impl_DcResult_table();
DcResult DcResult_new(Result result, const char* message);

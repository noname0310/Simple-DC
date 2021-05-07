#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <stdbool.h>
#include "DcCalculator.h"
#include "Token.h"
#include "Result.h"

#define MAX_INPUT_LEN 257
static char* read_line() {
	char wstr[MAX_INPUT_LEN] = { 0 };

	HANDLE conin = GetStdHandle(STD_INPUT_HANDLE);
	assert(conin != INVALID_HANDLE_VALUE);
	bool flush_result = FlushConsoleInputBuffer(conin);
	assert(flush_result != false);
	unsigned long read = 0;
	bool read_result = ReadConsoleA(conin, wstr, MAX_INPUT_LEN, &read, NULL);
	assert(read_result != false);
	int readbyte = read * sizeof(char);

	if (wstr[read - 2] == L'\r') {
		wstr[read - 2] = L'\0';
		char* result = malloc((size_t)(readbyte - 1));
		assert(result != NULL || !"malloc failed");
		memcpy_s(result, (size_t)(readbyte - 1), wstr, (size_t)(readbyte - 1));
		return result;
	}
	else if (wstr[read - 1] == '\n') {
		wstr[read - 1] = L'\0';
		char* result = malloc(readbyte);
		assert(result != NULL || !"malloc failed");
		memcpy_s(result, readbyte, wstr, readbyte);
		return result;
	}
	else if (wstr[read - 1] != '\0') {
		char* result = malloc(readbyte);
		assert(result != NULL || !"malloc failed");
		memcpy_s(result, readbyte, wstr, readbyte);
		return result;
	}
	else {
		char* result = malloc(readbyte);
		assert(result != NULL || !"malloc failed");
		memcpy_s(result, readbyte, wstr, readbyte);
		return result;
	}
}

int main()
{
	DcCalculator dcCalculator = DcCalculator_new();
	for (;;) {
		char* input = read_line();
		char* inputrc = input;
		char* context = NULL;
		char* token = strtok_s(inputrc, " ", &context);
		while (token != NULL) {
			Token input = Token_LexSingle(token);
			DcResult result = dcCalculator.f->step(&dcCalculator, &input);
			if (result.f->get_message(&result) != NULL) {
				printf(result.f->get_result(&result) == Result_Ok
					? "%s\n"
					: "Err: %s\n", result.f->get_message(&result));
				if (result.f->get_message(&result)[0] == 'q')
					exit(0);
			}
			else if (result.f->get_result(&result) == Result_Err)
				printf("Err\n");

			if (result.f->get_result(&result) == Result_Err)
				break;
			result.f->dispose(&result);
			input.f->dispose(&input);

			token = strtok_s(context, " ", &context);
		}
		free(input);
	}
	dcCalculator.f->dispose(&dcCalculator);
}

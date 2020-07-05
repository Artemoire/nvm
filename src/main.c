#include <stdio.h>
#include <string.h>

#include "mem.h"
#include "tot_file.h"
#include "compiler.h"
#include "nvm.h"

int str_ends_with(const char* str, const char* suffix)
{
	if (!str || !suffix)
		return 0;
	size_t lenstr = strlen(str);
	size_t lensuffix = strlen(suffix);
	if (lensuffix > lenstr)
		return 0;
	return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

int main(int argc, char* argv[])
{
	tot_FbufReader r;
	nvm_Compiler c;
	nvm_Op* p;
	nvm_VM vm;
	int result;

	if (argc != 2) {
		fprintf(stderr, "usage: nvm input_file\n");
		return 0;
	}

	if (!str_ends_with(argv[1], ".nop")) {
		fprintf(stderr, "Input error: nop script filenames must end with '.nop'. This is the way...\n");
		return 0;
	}

	tot_FbufReader_init(&r);
	if (!tot_FbufReader_open(&r, argv[1])) return 0;

	nvm_Compiler_init(&c, &r);
	p = nvm_Compiler_compile(&c);
	if (p == NULL) {
		tot_FbufReader_close(&r);
		return 0;
	}

	nvm_VM_init(&vm, p);
	result = nvm_VM_run(&vm);
	tot_FbufReader_close(&r);
	free(p);

	if (result < 0) {
		return 0;
	}

	fprintf(stdout, "Executed %d NOPs.", result);

	return 1;
}
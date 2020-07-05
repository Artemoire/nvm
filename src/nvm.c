#include <stdio.h>

#include "common.h"
#include "nvm.h"

void nvm_VM_init(nvm_VM* vm, nvm_Op* program)
{
	vm->ip = program;
}

int nvm_VM_run(nvm_VM* vm)
{
	int ops = 0;

	if (vm->ip == NULL) {
		return -1;
	}

	for (;;) {
		nvm_Op op = *vm->ip;

		switch (op)
		{
		case nvm_Op_NOP:
			vm->ip++;
			ops++;
			break;
		case nvm_Op_RET:
			return ops;
		default:
			fprintf(stderr, "Runtime error: unknown op %d", op);
			break;
		}
	}

}
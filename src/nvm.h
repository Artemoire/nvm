#ifndef nvm_nvm_h
#define nvm_nvm_h

#include "ops.h"

typedef struct {
	nvm_Op* ip;
} nvm_VM;

void nvm_VM_init(nvm_VM* vm, nvm_Op* program);
int nvm_VM_run(nvm_VM* vm);


#endif
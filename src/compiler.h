#ifndef nvm_compiler_h
#define nvm_compiler_h

#include "ops.h"
#include "tot_file.h"

typedef struct {
	tot_FbufReader* reader;
	unsigned char buffIdx;
	unsigned int currCh;
	unsigned int currLn;
} nvm_Compiler;

void nvm_Compiler_init(nvm_Compiler* c, tot_FbufReader* reader);
nvm_Op* nvm_Compiler_compile(nvm_Compiler* c);

#endif
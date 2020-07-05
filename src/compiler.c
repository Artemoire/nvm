#include <stdio.h>

#include "mem.h"
#include "common.h"
#include "compiler.h"

typedef enum {
	state_ENTRY,
	state_N,
	state_O,
	state_P,
	state_W,
	state_ERR
} c_state;

void nvm_Compiler_init(nvm_Compiler* c, tot_FbufReader* reader)
{
	c->buffIdx = 0;
	c->currCh = 1;
	c->currLn = 1;
	c->reader = reader;
}

static bool is_white(char c)
{
	return (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\f' || c == '\v');
}

static void skip_white(nvm_Compiler* c)
{
	for (;;) {
		char k = c->reader->buff[c->buffIdx];

		if (k == '\0') {
			return;
		}

		if (k == '\n') {
			c->currCh = 1;
			c->currLn++;
		}
		else if (k == ' ' || k == '\r' || k == '\t' || k == '\f' || k == '\v') {
			c->currCh++;
		}
		else {
			return;
		}

		c->buffIdx++;

		if (c->buffIdx > 3) {
			c->buffIdx = 0;
			if (!tot_FbufReader_update(c->reader)) return;
		}
	}
}

nvm_Op* compile(nvm_Compiler* c)
{
	nvm_Op* compiled;
	c_state state = state_ENTRY;
	unsigned int nopCount = 0;

	if (!tot_FbufReader_update(c->reader)) return NULL; // prime the reader

	if (is_white(c->reader->buff[c->buffIdx])) {
		state = state_W;
	}

	for (;;)
	{
		char k = c->reader->buff[c->buffIdx];

		switch (state)
		{
		case state_ENTRY:
		{
			if (k == 'n' || k == 'N') {
				state = state_N;
				c->currCh++;
				c->buffIdx++;
			}
			else if (k != '\0') {
				state = state_ERR;
			}
			break;
		}
		case state_N:
		{
			if (k == 'o' || k == 'O') {
				state = state_O;
				c->currCh++;
				c->buffIdx++;
			}
			else if (k != '\0') {
				state = state_ERR;
			}
			break;
		}
		case state_O:
		{
			if (k == 'p' || k == 'P') {
				state = state_P;
				c->currCh++;
				c->buffIdx++;
			}
			else if (k != '\0') {
				state = state_ERR;
			}
			break;
		}
		case state_P:
		{

			nopCount++;
			if (is_white(k)) {
				state = state_W;
			}
			else if (k == '\0') {
				state = state_ENTRY;
			}
			else {
				state = state_ERR;
			}
			break;
		}
		case state_W:
		{
			skip_white(c);
			state = state_ENTRY;
			break;
		}
		case state_ERR:
		{
			printf("Error compiling source: Unexpected char '%c' (Ln: %d Ch: %d).", k, c->currLn, c->currCh);
			return NULL;
		}
		default:
		{
			printf("Error compiling source: Invalid compiler state.");
			return NULL;
		}
		}

		if (k == '\0') {
			switch (state)
			{
			case state_ENTRY:
				compiled = ALLOCATE(sizeof(nvm_Op) * (nopCount + 1));
				for (unsigned int i = 0; i < nopCount; ++i)
				{
					compiled[i] = nvm_Op_NOP;
				}
				compiled[nopCount] = nvm_Op_RET;
				return compiled;
			case state_N:
				fprintf(stderr, "Error compiling source: Unterminated 'nop' keyword at (Ln: %d Ch: %d)", c->currLn, c->currCh);
				return NULL;
			case state_O:
				fprintf(stderr, "Error compiling source: Unterminated 'nop' keyword at (Ln: %d Ch: %d)", c->currLn, c->currCh);
				return NULL;
			case state_P:
				// Should be unreachable
				fprintf(stderr, "Error compiling source: Invalid compiler state.");
				return NULL;
			case state_W:
				// Should be unreachable
				fprintf(stderr, "Error compiling source: Invalid compiler state.");
				return NULL;
			case state_ERR:
				// Should be unreachable
				fprintf(stderr, "Error compiling source: Invalid compiler state.");
				return NULL;
			default:
				// Should be unreachable
				fprintf(stderr, "Error compiling source: Invalid compiler state.");
				return NULL;
			}
		}

		if (c->buffIdx > 3) {
			c->buffIdx = 0;
			if (!tot_FbufReader_update(c->reader)) return NULL;
		}
	}
}

nvm_Op* nvm_Compiler_compile(nvm_Compiler* c)
{
	return compile(c);
}
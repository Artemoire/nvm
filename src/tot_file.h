#ifndef tot_file_h
#define tot_file_h

#include <stdbool.h>

#ifndef TOT_FBUFF_SIZE
#define TOT_FBUFF_SIZE 4
#endif

typedef struct
{
	char buff[TOT_FBUFF_SIZE];
	FILE* _stream;
} tot_FbufReader;

void tot_FbufReader_init(tot_FbufReader* reader);
bool tot_FbufReader_open(tot_FbufReader* reader, const char* path);
bool tot_FbufReader_update(tot_FbufReader* reader);
void tot_FbufReader_close(tot_FbufReader* reader);

#endif
#include <stdio.h>
#include <string.h>

#include "tot_file.h"

void tot_FbufReader_init(tot_FbufReader* reader)
{
	reader->_stream = NULL;
}

bool tot_FbufReader_open(tot_FbufReader* reader, const char* filename)
{	
	if (reader->_stream != NULL) {
		fprintf(stderr, "Error opening file: Another file already opened\n");
		return false;
	}

	errno_t err = fopen_s(&reader->_stream, filename, "rb");

	if (err) {
		reader->_stream = NULL;
		fprintf(stderr, "Error opening file: %s\n", strerror(err));
		return false;
	}
	
	return true;
}

bool tot_FbufReader_update(tot_FbufReader* reader)
{
	if (reader->_stream == NULL) {
		fprintf(stderr, "Error reading file: No file opened\n");
		return false;
	}
	for (int i = 0; i < TOT_FBUFF_SIZE; i++) {
		
		int k = fgetc(reader->_stream);
		if (k == EOF) {
			reader->buff[i] = '\0';
			return true;
		}
		reader->buff[i] = (char)k;
	}

	return true;
}

void tot_FbufReader_close(tot_FbufReader* reader) {
	if (reader->_stream == NULL) return;
	fclose(reader->_stream);
}
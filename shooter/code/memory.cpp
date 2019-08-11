#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

static void memoryBytesCopy(void *destination, void *source, int length)
{
	char *charDestination = (char *)destination;
	char *charSource = (char *)source;

	for (int i = 0; i < length; ++i)
	{
		charDestination[i] = charSource[i];
	}
}

static void memoryBytesSet(void *destination, char source, int length)
{
	char *charDestination = (char *)destination;

	for (int i = 0; i < length; ++i)
	{
		charDestination[i] = source;
	}
}

char * readEntireFile(const char *path)
{
	FILE *file = fopen(path, "rb");
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);
	char *data = (char *)malloc(size + 1);
	memoryBytesSet(data, 0, size + 1);
	fread(data, size, 1, file);
	return data;
}


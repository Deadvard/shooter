#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <memory>

struct String16
{
	uint32 length;
	char data[16];
};

struct StringPair16
{
	String16 key;
	String16 value;
};

struct String
{
	int length;
	char *data;
};

String16 string16(const char * cstr)
{
	String16 result;
	int i = 0;
	while (cstr[0] && i < 16)
	{
		result.data[i] = cstr[i];
		++i;
	}

	result.length = i;
	return result;
}

bool32 stringIsEqual(String16 *a, String16 *b)
{
	int length = a->length < b->length ? a->length : b->length;
	for (int i = 0; i < length; ++i)
	{
		if (a->data[i] != b->data[i]) return false;
	}

	return true;
}

int stringToInt(String16 *str)
{
	int result = 0;
	int len = strlen(str->data);
	for (int i = 0; i < str->length; ++i)
	{
		result = result * 10 + (str->data[i] - '0');
	}
	return result;
}

inline bool isEndOfLine(char c)
{
	return ((c == '\n') ||
		(c == '\r'));
}

inline bool isWhiteSpace(char c)
{
	return ((c == ' ') ||
		(c == '\t') ||
		isEndOfLine(c));
}

static void removeAllWhiteSpace(char **at)
{
	while ((**at) && isWhiteSpace(**at))
	{
		++(*at);
	}
}

static String getString(char **at)
{
	String result;
	result.data = *at;
	while ((**at) && !isWhiteSpace(**at))
	{
		++(*at);
	}
	result.length = (int)((*at) - result.data);
	return result;
}

static void getPair(char **at, StringPair16 *pair)
{
	String key;
	String value;
	key.length = 0;
	value.length = 0;

	while (**at)
	{
		removeAllWhiteSpace(at);
		if (**at == '=')
		{
			++(*at);
			removeAllWhiteSpace(at);
			value = getString(at);
			break;
		}
		else
		{
			key = getString(at);
		}
	}

	for (int i = 0; i < 16; ++i)
	{
		if (i < key.length)
		{
			pair->key.data[i] = key.data[i];
		}
		else
		{
			pair->key.data[i] = 0;
		}

		if (i < value.length)
		{
			pair->value.data[i] = value.data[i];
		}
		else
		{
			pair->value.data[i] = 0;
		}
	}
	pair->key.data[15] = 0;
	pair->value.data[15] = 0;
	pair->key.length = key.length < 15 ? key.length : 15;
	pair->value.length = value.length < 15 ? value.length : 15;
}

void parse(char *fileContents, StringPair16 *pairs, uint32 size)
{
	char *at = fileContents;

	uint32 i = 0;
	while (at[0] && i < size)
	{
		getPair(&at, &pairs[i++]);
	}
}

#endif

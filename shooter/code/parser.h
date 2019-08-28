#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <memory>

struct String16
{
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
	char *text;
};

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
	result.text = *at;
	while ((**at) && !isWhiteSpace(**at))
	{
		++(*at);
	}
	result.length = (int)((*at) - result.text);
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
			pair->key.data[i] = key.text[i];
		}
		else
		{
			pair->key.data[i] = 0;
		}

		if (i < value.length)
		{
			pair->value.data[i] = value.text[i];
		}
		else
		{
			pair->value.data[i] = 0;
		}
	}
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

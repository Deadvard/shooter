#ifndef INPUT_H
#define INPUT_H

#include "SDL/SDL.h"
#include "parser.h"

struct Keyboard
{
	String16 virtualKeys[128];
	bool32  isPressed[128];
};

bool32 isPressed(Keyboard *keyboard, String16 *virtualKey)
{
	for (int i = 0; i < 128; ++i)
	{
		if (string16IsEqual(virtualKey, &keyboard->virtualKeys[i]))
		{
			return keyboard->isPressed[i];
		}
	}
	return false;
}

void bindKeys(Keyboard *keyboard, StringPair16 *pairs, uint32 length)
{	
	for (uint32 i = 0; i < length; ++i)
	{
		for (int j = 0; j < 128; ++j)
		{
			if (string16IsEqual(&pairs[i].value, &string16(SDL_GetKeyName(j))))
			{
				if (j >= 'A' && j <= 'Z') j += 32;
				keyboard->virtualKeys[j] = pairs[i].key;
				break;
			}
		}
	}
}

#endif

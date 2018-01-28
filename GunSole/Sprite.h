#pragma once
#include <Windows.h>

class Sprite {
public:
	// con/destructor(s)
	Sprite(const char *const fileName, const COORD spriteSize, const unsigned short numOf = 1, const COORD position = { 0, 0 }, const unsigned short theColour = 0x0F);
	Sprite(const COORD position, const unsigned short theColour = 0x0F);
	~Sprite();
	// method(s)
	const void Reload();
	const char AtXY(const short x, const short y) const;
	const void SetColour(const unsigned short theColour = 0x0F);
	// helper(s)
	COORD bufferSize;						// the size of the all the frames of the buffer
	unsigned numOfChars;					// the number of characters in the sprite
	// constant(s)
	const COORD spawnPos;					// the position that it spawns at
	const char *name;						// the name of the file
	const unsigned short numOfFrames;		// the number of differnt animation frames
	const COORD size;						// the size of the sprite
	unsigned short colour;					// the colour of the sprite
	// variable(s)
	COORD pos;								// the current position of the sprite
	COORD framePosition;					// the current frame of animation
	CHAR_INFO *chars;						// the one dimensional array of all the characters
};
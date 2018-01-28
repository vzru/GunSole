#include <fstream>
#include <string>
#include "Sprite.h"
#include "Global.h"

Sprite::Sprite(const COORD position, const unsigned short theColour) :
	size({ 1, 1 }), numOfFrames(1), numOfChars(1), bufferSize({ 1, 1 }), framePosition({ 0, 0 }),											// constant assignment
	spawnPos(position), colour(theColour)																									// direct assignment
{
	Reload();																																// load the sprite
}
Sprite::Sprite(const char *const fileName, const COORD spriteSize, const unsigned short numOf, const COORD position, const unsigned short theColour) :
	framePosition({ 0, 0 }),																												// constant assignment
	name(fileName), size(spriteSize), spawnPos(position), colour(theColour), numOfFrames(numOf),											// direct assignment
	numOfChars(numOf * spriteSize.X * spriteSize.Y), bufferSize({ spriteSize.X, spriteSize.Y * (short)numOf }) 								// compound assignment
{
	Reload();																																// load the sprite
}
Sprite::~Sprite() {}

const void Sprite::Reload()
{
	pos = spawnPos;
	chars = new CHAR_INFO[numOfChars];
	if (name)
	{
		std::ifstream infile(name);
		for (size_t i = 0; i < numOfChars; ++i)
		{
			char foo;
			infile.get(foo);
			switch(foo)
			{
			case '\n':
				--i;
				break;
			case '%':
				foo = Global::Char::Ground;
			default:
				chars[i].Char.AsciiChar = foo;
				chars[i].Attributes = colour;
				break;
			}
		}
		infile.close();
	}
	else
	{
		chars[0].Char.AsciiChar = Global::Char::Bullet;
		chars[0].Attributes = colour;
	}
}

const char Sprite::AtXY(const short x, const short y) const
{
	if (x >= -1 && y >= -1 && x <= size.X + 1 && y <= size.Y + 1)
		return chars[x + y * size.X].Char.AsciiChar;
	return 0;
}

const void Sprite::SetColour(const unsigned short theColour)
{
	for (size_t i = 0; i < numOfChars; ++i)
		chars[i].Attributes = theColour;
}
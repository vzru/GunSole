#pragma once
#include "Sprite.h"

class RenderManager
{
	HANDLE frameBuffer[2];									// the (2) frame buffers
	bool currentBuffer;										// the current buffer
	const COORD startPoint{ 0, 0 };							// the start position { 0, 0 }
public:
	// con/destructor(s)
	RenderManager(const COORD size);						// also sets the size
	~RenderManager();
	// method(s)
	const void swapBuffers();								// swap the visible screen buffer
	const void renderSprite(const Sprite *const S) const;	// render the sprite pointed to onto the non-visible screen buffer
	const void clearScreen() const;							// clear the screen
	const COORD setSize(const COORD size);					// set the size of the window and screen buffer
	// variables
	COORD size;												// what size is it set to
};
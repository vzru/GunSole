#include "RenderManager.h"

// the con/destructor(s)
RenderManager::RenderManager(const COORD size)
{
	frameBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	frameBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	setSize(size);
}
RenderManager::~RenderManager()
{
	CloseHandle(frameBuffer[0]);
	CloseHandle(frameBuffer[1]);
}

const void RenderManager::swapBuffers()
{
	SetConsoleActiveScreenBuffer(frameBuffer[currentBuffer]);
	currentBuffer = !currentBuffer;
}

const void RenderManager::renderSprite(const Sprite *const S) const
{
	SMALL_RECT info{ S->pos.X, S->pos.Y, S->pos.X + S->size.X, S->pos.Y + S->size.Y - 1 };
	WriteConsoleOutputA(frameBuffer[currentBuffer], S->chars, S->bufferSize, S->framePosition, &info);
}

const void RenderManager::clearScreen() const
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(frameBuffer[currentBuffer], &csbi);
	DWORD conSize = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD charsWritten;
	FillConsoleOutputCharacter(frameBuffer[currentBuffer], (TCHAR)' ', conSize, startPoint, &charsWritten);
	FillConsoleOutputAttribute(frameBuffer[currentBuffer], csbi.wAttributes, conSize, startPoint, &charsWritten);

}
const COORD RenderManager::setSize(const COORD size)
{
	SMALL_RECT info{ 0, 0, size.X, size.Y };				// helper variable
	SetConsoleScreenBufferSize(frameBuffer[0], size);		// set the size of the first buffer
	SetConsoleWindowInfo(frameBuffer[0], TRUE, &info);		// set the size of the first screen
	SetConsoleScreenBufferSize(frameBuffer[1], size);		// set the size of the second buffer
	SetConsoleWindowInfo(frameBuffer[1], TRUE, &info);		// set the size of the second screen
	SetConsoleTitleA((LPCSTR)"GunSole");					// set the title of the window to be the name of the game
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);			/// maximize the screen (glitchy)
	swapBuffers();											/// show the other screen (glitchy)
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);			/// maximize that screen (glitchy)
	swapBuffers();											/// show the other one again (glitchy)
	this->size = size;										// set the size variable to be what the size was set to
	return size;											// return the new size
}

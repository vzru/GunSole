#include <Windows.h>
#include "Input.h"

const void Input::clearInStream(const size_t i)
{
	using namespace Input;
		using namespace Controls;

	GetAsyncKeyState(LEAVE);
	GetAsyncKeyState(PAUSE);
	GetAsyncKeyState(RETURN);
	GetAsyncKeyState(Q);
	GetAsyncKeyState(D);
	GetAsyncKeyState(C);

	if (i != -1)
	{
		GetAsyncKeyState(LEFT[i]);
		GetAsyncKeyState(RIGHT[i]);
		GetAsyncKeyState(JUMP[i]);
		GetAsyncKeyState(SHOOT[i]);
		GetAsyncKeyState(BLOCK[i]);
	}
}
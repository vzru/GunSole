// include default libraries
#include <Windows.h>
#include <vector>
// include user-defined libraries
#include "Global.h"
#include "RenderManager.h"
#include "Input.h"

namespace Global
{
	// defining variables, needed in order to use extern
	GameStates gameState = INTRO;

	// defining the function which are used for encapsulation
	const void menu(RenderManager *const RM, const Sprite *const menu, Sprite *const info)
	{
		RM->renderSprite(menu);						// print the menu onto the screen
		RM->swapBuffers();							// show the screen
		while (gameState == MENU)					// wait for user input
		{
			if (GetAsyncKeyState(Input::Q))			// if the user input Q
				gameState = QUIT;					// quit the game
			else if (GetAsyncKeyState(Input::C))	// if the user input C
			{
				info->framePosition = { 0, 0 };
				RM->renderSprite(info);
				RM->swapBuffers();
				while (!GetAsyncKeyState(Input::Controls::RETURN)) {}
				gameState = CAPTURE;				// start capture the flag
			}
			else if (GetAsyncKeyState(Input::D))	// if the user input D
			{
				info->framePosition = { 0, info->size.Y };
				RM->renderSprite(info);
				RM->swapBuffers();
				while (!GetAsyncKeyState(Input::Controls::RETURN)) {}
				gameState = DEATH;					// start death match
			}
		}
	}
	const void paused(RenderManager *const RM, Sprite *const pause)	
	{
		pause->framePosition = { 0, gameState == DPAUSE ? pause->size.Y : 0 };
		RM->renderSprite(pause);									// print the controls onto the pause screen
		RM->swapBuffers();											// show the screen
		while (gameState == DPAUSE || gameState == CPAUSE)			// wait for input
		{
			if (GetAsyncKeyState(Input::Controls::RETURN))			// if the return (enter) key is pressed
				gameState = gameState == DPAUSE ? DEATH : CAPTURE;	// return to the previously running game mode
			if (GetAsyncKeyState(Input::Controls::LEAVE))			// if the leave (escape) key is pressed
				gameState = MENU;									// set the state of the game to the menu
		}

	}
	const void won(RenderManager *const RM, const Sprite *const win)
	{
		RM->renderSprite(win);										// render the win screen
		RM->swapBuffers();											// show the screen
		while (!GetAsyncKeyState(Input::Controls::RETURN)) {}		// wait till the user presses enter
		gameState = MENU;											// change the game state back to the menu
	}
	const void checkifpause()
	{
		if (GetAsyncKeyState(Input::Controls::PAUSE)				// if the pause (space) key is pressed
			&& (gameState == CAPTURE || gameState == DEATH))		// and the game is being played
			gameState = 											// set the state of the game to ...
				gameState == DEATH									// if the game is in death mode
				? DPAUSE : CPAUSE;									// set the game to paused death, otherwise set the game to paused capture
	}
}
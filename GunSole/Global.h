#pragma once
#include "RenderManager.h"

// global variables
namespace Global
{
	// symbol(s)
	namespace Char
	{
		constexpr char Ground = char(219);				// █ block symbol for the ground
		constexpr char Bullet = '*';					// · dot symbol for bullet
		constexpr char Wall = '|';						// | vertical bar symbol for wall
		constexpr char BulletWall = ':';				// : full colon for wall that lets bullets through
		constexpr char Ceiling = '=';					// = equals sign for ceiling
		constexpr char Platform = '_';					// _ underscore symbol for platforms
		constexpr unsigned short numOfSpikes = 5;		// the number of different spike characters
		constexpr char Spike[numOfSpikes]{				// array of the spike characters
			'>','<','\\','/','^'						// left angle, right angle, backslash comined with slash, and caret for left, right, up, and alternataive up spikes
		};
	}
	// maximum(s)
	namespace Max
	{
		constexpr unsigned short Score[]{ 3, 5, 10 };	// the score needed for the player to win
		constexpr unsigned short Jumps = 2;				// the maximum number of jumps each player can do before landing
		constexpr unsigned short Bullets = 6;			// the maximum number of bullets each player can shoot at a time
		constexpr unsigned short Players = 2;			// the number of players at a time
	}
	// delay/timer length(s)
	constexpr unsigned short shotDelay = 4;				// the time until the player is allowed to shoot again immediately after shooting (or blocking) (in frames)
	constexpr unsigned short respawnDelay = 50;			// the time a player takes to respawn after dying (in frames)
	constexpr unsigned short invincibleTime = 75;		// the time the player will be invincible (in frames)
	constexpr unsigned short totalDelay = 25;			// the time to delay between frames (in miliseconds)
	// constant(s)
	constexpr unsigned short groundPos = 60;		// vertical position of the ground in symbols from the top
	constexpr float jumpStrength = 3.5;				// initial velocity upwards when jumping (and double jumping)
	constexpr float gravityStrength = 0.5;			// the constant downards gravity force, affects how fast the players fall

	// encapsulation function(s)
	const void menu(RenderManager *const RM, const Sprite *const menu, Sprite *const info);		// load the main menu
	const void paused(RenderManager *const RM, Sprite *const pause);							// pause the game
	const void won(RenderManager *const RM, const Sprite *const end);							// show the end of the narrative
	const void checkifpause();																	// determine if the game should be paused
	
	// information on the state of the game
	const enum GameStates {
		QUIT,		// shows the end screen and then closes
		INTRO,		// shows the intro screen and then goes to the main menu
		MENU,		// I don't have the menu working yet so it will just start the game after system("PAUSE")
		DEATH,		// the death match mode
		CAPTURE,	// the capture the flag mode
		DPAUSE,		// death mode pause screen
		CPAUSE,		// capture the flag mode pause screen
		WIN,		// when a player wins
	}; extern GameStates gameState;		// what state the game is currently in
}
/*		This is a game made by LiquidByte Studios	  */

// load user-defined libraries
#include "Global.h"
#include "Input.h"
#include "Player.h"
#include "Bullet.h"

using namespace Global;

// making the sprite information
Sprite *LevelSprite = new Sprite("Sprites/LevelSprites.txt", { 200, 80 }, 2);

namespace Screen {
	Sprite *Intro = new Sprite("Sprites/IntroScreen.txt", LevelSprite->size);
	Sprite *Logo = new Sprite("Sprites/LogoScreen.txt", LevelSprite->size);
	Sprite *Menu = new Sprite("Sprites/MenuScreen.txt", LevelSprite->size);
	Sprite *Info = new Sprite("Sprites/InfoScreens.txt", LevelSprite->size, 2);
	Sprite *Pause = new Sprite("Sprites/PauseScreens.txt", LevelSprite->size, 2);
	Sprite *Win = new Sprite("Sprites/WinScreens.txt", LevelSprite->size, 2);
	Sprite *Quit = new Sprite("Sprites/QuitScreen.txt", LevelSprite->size);
}

// making the render manager
RenderManager *RM = new RenderManager(LevelSprite->size);

// constructing each player
Player *player[]{																				// the array of players
	new Player("Sprites/ScoreSprites.txt", { 10, 6 }, 10, { 1, 1 },								// the first player's score's sprite
		"Sprites/PlayerSprites.txt", { 5, 4 }, 56, { 15, groundPos },							// the first player's character's sprite
		0x0C),																					// the rest of the first player's information
	new Player("Sprites/ScoreSprites.txt",{ 10, 6 }, 10, { LevelSprite->size.X - 11, 1 },		// the first player's score's sprite
		"Sprites/PlayerSprites.txt", { 5, 4 }, 56, { LevelSprite->size.X - 20, groundPos },		// the first player's character's sprite
		0x0A, true)																				// the rest of the first player's information
};

// foreward declaring functions
const void intro();		// introducting things
const void init();		// initializing things
const void update();	// updateing things
const int quit();		// ending things

// main code loop
const int main()
{
	RM->setSize(LevelSprite->size);					// set the size of the screen (again)

	// main game loop
	while (gameState != QUIT)						// run the game until the user wants to quit
	{
		switch (gameState)							// do different things based on which state the game is in
		{
		case INTRO:									// before the main menu
			intro();								// introduce the game
			break;
		case MENU:									// in the main menu
			init();									// initialize the values
			menu(RM, Screen::Menu, Screen::Info);	// show the main menu and info screens based on which mode was picked
			LevelSprite->framePosition = { 0, gameState == DEATH ? LevelSprite->size.Y : 0 };		// change the level to one without the flags for death match
			break;
		case CPAUSE:								// in a paused capture the flag
		case DPAUSE:								// in a paused death match mode
			paused(RM, Screen::Pause);				// do if the game is paused
			break;
		case CAPTURE:								// in capture the flag mode
		case DEATH:									// in death match mode
			update();								// move and kill the players and bullets and make the bullets and print the level, score, player, and bullets
			checkifpause();							// check to see if the game should be paused
			break;
		case WIN:									// if a player has won
			Screen::Win->framePosition = { 0, player[0]->score.value < player[1]->score.value ? Screen::Win->size.Y : 0 };		// change the screen to the one who won
			Screen::Win->SetColour(player[player[0]->score.value < player[1]->score.value]->colour);		// set the colour of the screen to whoever won
			won(RM, Screen::Win);					// call the function that tells the player who won
			break;
		}
		Input::clearInStream();						// clear the input stream
		Sleep(totalDelay);							// waiting a bit in order to slow down the framerate
	}
	return quit();									// end the program
}

const void intro()
{
	RM->renderSprite(Screen::Intro);		// print the intro screen
	RM->swapBuffers();						// show the screen
	Sleep(1500);							// wait 1.5 second
	RM->renderSprite(Screen::Logo);			// print the logo screen
	RM->swapBuffers();						// show the screen
	Sleep(1500);							// wait 1.5 second
	gameState = MENU;						// go to the menu
}

const void init()
{
	RM->setSize(LevelSprite->size);		// reseting the size of the screen

	// reloading the sprites and screens
	LevelSprite->Reload();				// reset the level sprite(s)
	Screen::Win->Reload();				// reset the win screens
	Screen::Logo->Reload();				// reset the logo screen
	Screen::Menu->Reload();				// reset the quit screen
	Screen::Pause->Reload();			// reset the pause screen

	// initialize each player
	for (size_t i = 0; i < Max::Players; ++i)		// loop through each player
		player[i]->Reset(i);						// reset the player's information
}

const int quit()
{
	RM->renderSprite(Screen::Quit);					// print the quiting screen
	RM->swapBuffers();								// schow the screen
	Sleep(2500);									// wait for a bit
	delete LevelSprite, delete RM;					// delete the level and render manager
	delete Screen::Intro; delete Screen::Logo;		// delete starting screens
	delete Screen::Menu; delete Screen::Pause;		// delete other screensd
	delete Screen::Win, delete Screen::Quit;		// delete ending screens
	delete player[0]; delete player[1];				// delete the players
	return 0;										// returning the code to quit with

}

const void update()
{
	// moving stuff
	for (size_t i = 0; i < Max::Players; ++i)					// loop through each player
	{
		player[i]->Move(i, LevelSprite, player[i ? 0 : 1]);		// move the player
		if (player[i]->score.value >= Max::Score[1])			// if the player's score is one of {3,5,10} or greater
			gameState = WIN;									// the player wins
	}
	// printing stuff
	RM->renderSprite(LevelSprite);						// print the level
	for (size_t i = 0; i < Max::Players; ++i)			// loop through each player
	{
		for (const auto & j : player[i]->bullet)		// iterate through the bullet vector
			RM->renderSprite(j);						// print the bullet
		if (!player[i]->isDead)							// if the player is not dead
			RM->renderSprite(player[i]);				// print the player
		RM->renderSprite(&player[i]->score);			// print the score
	}
	RM->swapBuffers();									// show the updated screen
}
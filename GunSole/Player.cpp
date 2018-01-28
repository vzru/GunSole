#include <Windows.h>
#include "Global.h"
#include "Input.h"
#include "Player.h"
#include "Bullet.h"

// the con/destructor(s)
Player::Player(const char *const scoreFileName, const COORD scoreSize, const unsigned short numOfScores, const COORD scorePos,									// the score sprite parameters
	const char *const fileName, const COORD spriteSize, const unsigned short numOf, const COORD position, const unsigned short theColour, const bool isLeft)		// the character sprite parameters
	: score(scoreFileName, scoreSize, numOfScores, scorePos, theColour),					// construct the score sprite
	Sprite(fileName, spriteSize, numOf, position, theColour), spawnFacingLeft(isLeft)		// construct the character sprite
{}
Player::~Player()
{
	for (auto j : bullet)		// loop through each bullet
		delete j;				// delete the bullet
	bullet.clear();				// clear the vector of all the bullets
}
Player::Score::Score(const char *const fileName, const COORD spriteSize, const unsigned short numOf, const COORD position, const unsigned short theColour)		// the score sprite parameters
	: Sprite(fileName, spriteSize, numOf, position, theColour), value(0)		// construct the score sprite and initialize the score to zero
{}
Player::Score::~Score() {}

using namespace Global;

const void Player::Move(const size_t i, const Sprite *const S, Player *const otherPlayer)
{
	// movement stuff
	if (isDead)							// if the player is dead
		Input::clearInStream(i);		// clear their input stream
	else								// otherwise
	{
		unsigned short collide = 0;				// declare the variable that will bitwisely hold the information on what the player has collided with
		{
			// horizontal movement checking
			for (short j = pos.Y; j < pos.Y + size.Y; ++j)							// loop through the height of the player
			{
				const char symbol1 = S->AtXY(pos.X - 1, j);							// holds the character of the level at the position on the left of the player
				const char symbol2 = S->AtXY(pos.X + size.X, j);					// holds the character of the level at the position on the right of the player
				if (symbol1 == 0 || symbol2 == 0)									// if either character is off the level
					Kill(i);														// kill the player
				else if (symbol1 != ' ' || symbol2 != ' ')							// if either side of the player has something other than a blank
				{
					if (symbol1 == Char::Wall || symbol1 == Char::BulletWall		// if on the left is a wall
						|| symbol1 == Char::Ceiling || symbol1 == Char::Ground		// or a floor
						|| symbol1 == Char::Ceiling)								// or a roof
					{
						collide |= 0x001;											// collide with it
						goto h_end;													// end the horizontal movement checking
					}
					if (symbol2 == Char::Wall || symbol2 == Char::BulletWall		// if on the right is a wall
						|| symbol2 == Char::Ceiling || symbol2 == Char::Ground		// or a floor
						|| symbol2 == Char::Ceiling)								// or a roof
					{
						collide |= 0x002;											// collide with it
						goto h_end;													// end the horizontal movement checking
					}
					for (size_t k = 0; k < Char::numOfSpikes; ++k)					// loop through each type of spike
					{
						if (symbol1 == Char::Spike[k])								// if on the left is a spike
						{
							collide |= 0x004;										// collide with it
							goto h_end;												// end the horizontal movement checking
						}
						if (symbol2 == Char::Spike[k])								// if on the right is a spike
						{
							collide |= 0x008;										// collide with it
							goto h_end;												// end the horizontal movement checking
						}
					}
				}
			}
		h_end:																						// end of the horizontal movement checking
			// horizontal movement
			if (GetAsyncKeyState(Input::Controls::LEFT[i])				// if the left key is pressed
				&& !isBlocking)											// and the player is not blocking
			{
				isFacingLeft = isMoving = true;							// turn left and start moving
				if (collide & 0x004)									// if there is something deadly there
				{
					if (SafeKill(i, otherPlayer->score))				// if he dies from  getting safely killed
						goto m_end;										// end the movement checking
				}
				else if (~collide & 0x001								// if there isn't anything else there
					&& pos.X > 0)										// and they are not at the left edge of the screen
					--pos.X;											// move him left
			}
			else if (GetAsyncKeyState(Input::Controls::RIGHT[i])		// if the right key is pressed
				&& !isBlocking)											// and the player is not blocking
			{
				isFacingLeft = false;									// turn right
				isMoving = true;										// start moving
				if (collide & 0x008)									// if there is something deadly there
				{
					if (SafeKill(i, otherPlayer->score))				// if he dies from getting safely killed
						goto m_end;										// end the movement checking
				}
				else if (~collide & 0x002								// if there isn't anything else there
					&& pos.X + size.X < S->size.X)						// and they are not at the right edge of the screen
					++pos.X;											// move him right
			}
			else														// otherwise
				isMoving = false;										// set him to be not moving
		}
		{
			// jumping movement
			if ((GetAsyncKeyState(Input::Controls::JUMP[i]) & 0x001)		/// if the jump key is pressed (glitchy)
				&& !isBlocking												// and the player is not blocking
				&& (!isJumping												// and they are not jumping
					|| (numOfJumps < Max::Jumps))							// or they haven't jumped too many times
				&& yVel > jumpStrength / -2.0f)								// and they didn't just start a jump
			{
				yVel = -jumpStrength;										// jump impulse
				++numOfJumps;												// increase the number of times jumped
				isJumping = true;											// make him be jumping
			}
			// vertical movement checking
			short height = -1;																// declare the height variable used in vertical movement checking
			if (yVel < 0)																	// if they are moving upwards
				for (short j = pos.Y; j > pos.Y + yVel; --j)								// loop through above the player based on how fast he is moving up
					for (short k = pos.X; k < pos.X + size.X; ++k)							// loop through above the player left to right
					{
						const char symbol = S->AtXY(k, j - 1);								// holds the character at that position above the player
						if (symbol == 0)													// if that character is off the level
							Kill(i);														// kill the player
						else if (symbol != ' ')												// if the character is not a blank
						{
							if (symbol == Char::Ground || symbol == Char::Ceiling			// if it is not a roof
								|| symbol == Char::Wall || symbol == Char::BulletWall)		// and it is not a wall
							{
								collide |= 0x10;											// collide with it
								height = j;													// set the collision height to where that character is
								goto v_end;													// end the vertical movement checking
							}
							for (size_t l = 0; l < Char::numOfSpikes; ++l)					// loop through each type of spike
								if (symbol == Char::Spike[l])								// if there is that spike there
								{
									collide |= 0x40;										// collide with it
									height = j;												// set the collision height to where the character is
									goto v_end;												// end the vertical movement checking
								}
						}
					}
			else																			// otherwise
				for (short j = pos.Y; j < pos.Y + yVel; ++j)								// loop through below the player based on how fast he is moving down
					for (short k = pos.X; k < pos.X + size.X; ++k)							// loop through below the player left to right
					{
						const char symbol = S->AtXY(k, j + size.Y);							// holds the character at that position below the player
						if (symbol == 0)													// if that character is off the level
							Kill(i);														// kill the player
						else if (symbol != ' ')												// if the character is not a blank
						{
							if (symbol == Char::Ground || symbol == Char::Platform			// if it is not a floor
								|| symbol == Char::Wall || symbol == Char::BulletWall)		// and it is not a wall
							{
								collide |= 0x020;											// collide with it
								height = j;													// set the collision height to where that character is
								goto v_end;													// end the vertical movement checking
							}
							for (size_t l = 0; l < Char::numOfSpikes; ++l)					// loop through each type of spike
								if (symbol == Char::Spike[l])								// if there is that spike there
								{
									collide |= 0x080;										// collide with it
									height = j;												// set the collision height to where the character is
									goto v_end;												// end the vertical movement checking
								}
						}
					}
		v_end:																						// end of the vertical movement checking
			// vertical movement
			if (collide & 0x040 || collide & 0x080)				// if they collided with a spike
			{
				if (SafeKill(i, otherPlayer->score))			// if they die from getting safely killed
					goto m_end;									// end the movement checking
				else											// otherwise
				{
					pos.Y = height;								// set the position of the player to the height of the thing they collided with
					if (collide & 0x040)						// if the collision is above
						yVel += gravityStrength;				// decrease their vertical velocity by gravity
					else										// otherwise (the collision will be below
						yVel = numOfJumps = isJumping = 0;		// stop their jump, reset the number of times they have jumped to zero, set them to be not jumping
				}
			}
			else if (collide & 0x010)							// if there is something deadly there
			{
				pos.Y = height;									// set the position of the player to the height of the thing they collided with
				yVel += gravityStrength;						// decrease their vertical velocity by gravity
			}
			else if (collide & 0x020)							// if there is somthing non-deadly there
			{
				pos.Y = height;									// set the position of the player to the height of the thing they collided with
				yVel = numOfJumps = isJumping = 0;				// stop their jump, reset the number of times they have jumped to zero, set them to be not jumping
			}
			else												// otherwise
			{
				pos.Y += (short)yVel;							// move it vertically based on how fast it is moving vetically
				yVel += gravityStrength;						// decrease it's vertical speed by the gravity
			}
		}
		// other movement
		if (GetAsyncKeyState(Input::Controls::BLOCK[i]))		// if the block key is pressed
		{
			shootDelay = shotDelay;								// start the shooting delay
			isBlocking = true;									// set the player to be blocking
		}
		else if (GetAsyncKeyState(Input::Controls::SHOOT[i])	// if the shoot key is pressed
			&& !isBlocking && !isCarryingFlag)					// and the player is not blocking or carrying the flag
		{
			isShooting = true;									// set the player to be shooting
			if (!shootDelay)									// if the shooting delay is over
			{
				shootDelay = shotDelay;							// start the shooting delay
				if (bullet.capacity() > bullet.size())			// if the player hasn't already made too many bullets
					bullet.push_back(new Bullet(this));			// make a new bullet and add it the player's vector
			}
		}
		else													// otherwise
			isShooting = isBlocking = false;					// the player is not shooting or blocking
		// capture movement
		if (gameState == CAPTURE)									// if the game is in capture the flag mode
			for (short j = pos.X; j < pos.X + size.X; ++j)			// horizontally loop through the boundries of the player
				for (short k = pos.Y; k < pos.Y + size.Y; ++k)		// vertically loop through the boundries of the player
				{
					const char symbol = S->AtXY(j, k);				// holds the character at that position
					if (symbol == 0)								// if that character is off the level
						Kill(i);									// kill the player
					else if (symbol == (i ? '1' : '2')				// if the character is the number of the other player's flag
						&& !isCarryingFlag)							// and they are not carrying the flag
					{
						collide |= 0x100;							// collide with it
						isCarryingFlag = true;						// start the flag
						goto m_end;									// end the movement
					}
					else if (symbol == (i ? '2' : '1')				// if the chracter the number of their flag
						&& isCarryingFlag)							// and they are carrying the flag
					{
						collide |= 0x200;							// collide with it
						isCarryingFlag = false;						// stop carrying it
						++score.value;								// increase the player's score
						goto m_end;									// end the movement
					}
				}
	}
m_end:																								// end of the movement
	// other stuff
	{
		// timer stuff
		if (shootDelay)								// if the shooting is delayed
			--shootDelay;							// decrement the shooting delay time
		if (respawnTimer > 0)						// if the player is not killable
		{
			--respawnTimer;							// decrement the respawning timer
			if (respawnTimer < invincibleTime)		// if the player has been dead for the required amount of time
				isDead = false;						// make them not dead
		}
		// bullet stuff
		for (auto j = otherPlayer->bullet.begin();					// iterate from the other player's first bullet
			j != otherPlayer->bullet.end();)						// to their last bullet
		{
			if ((*j)->Move(S))										// move the bullet
			{														// if it hits a wall
				delete *j;											// kill the bullet?
				j = otherPlayer->bullet.erase(j);					// remove the bullet from the vector
			}
			else if ((*j)->pos.X > pos.X							// if the bullet is farther right than the left edge of the player
				&& (*j)->pos.X < (pos.X + size.X)					// and is farther left than the right
				&& (*j)->pos.Y > pos.Y - 1							// and is farther down than the top
				&& (*j)->pos.Y < pos.Y + size.Y						// and is farther up than the bottom
				&& !isDead)											// and if the player is not already dead
			{
				if (!respawnTimer									// if the player is not invincible or dead
					&& (!isBlocking									// and is not blocking
					|| (((*j)->speed < 0 && isFacingLeft)			// or the bullet is going left and the player is facing right
						|| ((*j)->speed > 0 && !isFacingLeft))))	// or the bullet is going right and the player is facing left
				{
					Kill(i);										// kill the other player
					if (gameState == DEATH)							// if the gamemode is deathmatch
						++otherPlayer->score.value;					// increase the player's score
				}
				delete *j;											// delete the bullet
				j = otherPlayer->bullet.erase(j);					// remove the bullet from the vector
			}
			else													// if the bullet doesn't die
				++j;												// increment the iterator
		}
	}
	// sprite stuff
	UpdateSprites(i);					// update the sprites
}

const bool Player::SafeKill(const size_t i, Score &otherScore)
{
	if (!respawnTimer)					// if they are not invincible
	{
		if (gameState == DEATH)			// if the game mode is death match
			++otherScore.value;			// increase the other player's score
		Kill(i);						// kill the player
		return true;					// the player was killed
	}
	return false;						// the player was not killed
}

const void Player::Kill(const size_t i)
{
	pos = spawnPos;													// reset the position to the player's spawn one
	yVel = numOfJumps = isJumping = 0;								// reset the jumping information
	isFacingLeft = spawnFacingLeft;									// make him face left if he spawns facing left
	respawnTimer = Global::respawnDelay + Global::invincibleTime;	// start the respawn and invincible delay timers
	shootDelay = 0;													// reset the shooting delay
	isDead = true;													// set him to be dead
	isMoving = isBlocking = isShooting = isCarryingFlag = false;	// set him to be not moving or blocking or shooting or flag carrying
	UpdateSprites(i);												// update the sprites
}

const void Player::Reset(const size_t i)
{
	score.Reload();							// reload the score sprite
	Reload();								// reload the character sprite
	Kill(i);								// kill the player
	bullet.clear();							// empty the bullet vector
	bullet.reserve(Max::Bullets);			// reserve enough room for all the bullets
	respawnTimer = score.value = 0;			// reset the death time
	isDead = false;							// unkill the player
	UpdateSprites(i);						// update the sprites
}

const void Player::UpdateSprites(const size_t i)
{
	score.framePosition = { 0, score.value * 6 };									// update the score sprite animation
	respawnTimer ? SetColour(colour - 8) : SetColour(colour);						// set the colour of the character to white if they are invincible otherwise to their normal colour
	framePosition = { 0, (i ? 112 : 0) + (isFacingLeft ? 56 : 0)					// update the character sprite animation based on which character and which direction
		+ (isBlocking ? 48 + (isCarryingFlag ? 0 : 4) : ((isCarryingFlag ? 32 : 0)	// and on if blocking and if flag carrying
		+ (isShooting ? 16 : 0) + (isJumping ? 8 : 0) + (isMoving ? 4 : 0))) };		// and on if shooting and if jumping and if moving
}
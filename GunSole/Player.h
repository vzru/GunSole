#pragma once
#include <vector>

// foreward declaring classes
class Bullet;

class Player : public Sprite {
	//// the starting information
	const bool spawnFacingLeft;			// if the player should be facing left when they spawn
	// timer(s)
	unsigned short shootDelay = 0;		// the time until the player is allowed to shoot again
	// boolean state(s)
	bool isShooting = false;			// is the player is shooting
	bool isMoving = false;				// is the player is moving
	bool isJumping = false;				// is the player is jumping
	bool isCarryingFlag = false;		// is the player carrying the flag
	bool isBlocking = false;			// is the player blocking
	// movement information
	float yVel = 0.0f;					// the player's y velocity
	unsigned short numOfJumps = 0;		// the number of jumps the player has done so far (before hitting the ground)
public:
	// the con/destructor(s)
	Player(const char *const scoreFileName, const COORD scoreSize, const unsigned short numOfScores, const COORD scorePos,			// the score sprite parameters
		const char *const fileName, const COORD spriteSize, const unsigned short numOf = 1, const COORD position = { 0, 0 },		// the character sprite parameters
		const unsigned short theColour = 0x0F, const bool isLeft = false);															// the other player information
	~Player();
	// method(s)
	const void Move(const size_t i, const Sprite *const S, Player *const otherPlayer);	// takes in the player number, the sprite of the screen and the other player and moves the player and the other's bullets
	const void Kill(const size_t i);													// takes in the player number and kills that player
	const void Reset(const size_t i);													// takes in the player number and resets the player's information and sprites

	// timer(s)
	unsigned short respawnTimer = 0;	// the time until the player comes back alive and is killable again
	// boolean state(s)
	bool isFacingLeft = false;			// is the player facing left
	bool isDead = false;				// is the player dead
	// other information
	struct Score : public Sprite {
		Score(const char *const fileName, const COORD spriteSize, const unsigned short numOf, const COORD position, const unsigned short theColour);
		~Score();
		unsigned short value;		// the score
	} score;		// the player's scoring information
	std::vector<Bullet*> bullet;		// vector of the player's bullets
private:
	// method(s)
	const bool SafeKill(const size_t i, Score &otherScore);		// takes in the player number and the other player's score and kills the player if he's not invincible, incrementing the other player's score if it is a deathmatch, returning true if the player was killed
	const void UpdateSprites(const size_t i);							// takees in the player number and resets the sprite animations based on the current states
};
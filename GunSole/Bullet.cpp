#include "Global.h"
#include "Player.h"
#include "Bullet.h"

// the con/destructor(s)
Bullet::Bullet(const Player *const P)							// the player who is constructing the bullet
	: Sprite({ P->pos.X + 3, P->pos.Y + 2 }, P->colour),		// construct the sprite
	speed(-6 * P->isFacingLeft + 3)								// initialize the value(s)
{}
Bullet::~Bullet() {}

const bool Bullet::Move(const Sprite *const S)
{
	const short dir = speed < 0 ? -1 : 1;							// which way the bullet is going
	for (size_t i = 0; speed != i; i += dir)						// loop through each spot in front of it
	{
		if (pos.X <= 0 || pos.X >= S->size.X						// if the bullet is past the edge of the level
			|| S->AtXY(pos.X + dir, pos.Y) == Global::Char::Wall)	// or there is a wall in the way
			return true;											// tell it to kill the bullet
		pos.X += dir;												// move it one character forward
	}
	return false;													// tell it not to kill the bullet
}
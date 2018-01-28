#pragma once

// foreward declaring classes
class Player;

class Bullet : public Sprite {
public:
	// con/destructor(s)
	Bullet(const Player *const S);
	~Bullet();
	// movement information
	const short speed;							// the direction the bullet is heading
	// method(s)
	const bool Move(const Sprite *const S);		// move the bullet (takes in the level, as well as returns true if it should be killed)
};
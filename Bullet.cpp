#include "Bullet.h"

Bullet::Bullet(int targetMask) {
	this->targetMask = (TargetMask)targetMask;
    this->lifetime = 0.0f;
}

void Bullet::Split(std::vector<Bullet>* bulletListRef) {
	//Create a new bullet and add it to the bullet's list
    //Change the current bullet's layerMask to be targetting both enemies and player targetMask = (Player | Enemies);
    //In case the bullet was fired by an enemy, keep the player mask as the player's one
    int targetMask = this->targetMask == 1 ? TargetMask::PlayerMask : TargetMask::PlayerMask | TargetMask::EnemiesMask;
    Bullet splitBullet = Bullet(targetMask);
    splitBullet.r = this->r;

	//Determine the direction by going the opposite of the current direction and offset it by +- 45°
    float angle = 45 * M_PI / 180.0f;
    splitBullet.dx = -this->dx * cosf(angle) + this->dy * sinf(angle);
    splitBullet.dy = -this->dx * sinf(angle) - this->dy * cosf(angle);

	bulletListRef->push_back(splitBullet);
}

TargetMask Bullet::GetTargetMask() {
	return this->targetMask;
}


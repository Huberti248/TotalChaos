#include "Bullet.h"

Bullet::Bullet(TargetMask targetMask) {
	this->targetMask = targetMask;
}

void Bullet::Split(std::vector<Bullet>* bulletListRef) {
	//Create a new bullet and add it to the bullet's list
	//Determine the direction by going the opposite of the current direction and offset it by +- 45°
	//Change the current bullet's layerMask to be targetting both enemies and player
}

TargetMask Bullet::GetTargetMask() {
	return this->targetMask;
}

#include "Bullet.h"

Bullet::Bullet(int targetMask) {
	this->targetMask = (TargetMask)targetMask;
}

void Bullet::Split(std::vector<Bullet>* bulletListRef) {
	//Create a new bullet and add it to the bullet's list
	//Determine the direction by going the opposite of the current direction and offset it by +- 45°
	//Change the current bullet's layerMask to be targetting both enemies and player targetMask = (Player | Enemies);
}

TargetMask Bullet::GetTargetMask() {
	return this->targetMask;
}

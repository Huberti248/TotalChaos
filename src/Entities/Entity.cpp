#include "Entity.h"

bool Entity::operator==(const Entity& other) {
    int cmp = memcmp(this, &other, sizeof(this));
    return cmp == 0;
}

Entity::Entity(){}

Entity::~Entity() { }
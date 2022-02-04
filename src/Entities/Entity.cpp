#include "Entity.h"

bool Entity::operator==(const Entity& other) {
    int cmp = memcmp(this, &other, sizeof(this));
    return cmp == 0;
}

Entity::Entity(){}

Entity::~Entity() { }

SDL_Texture* Entity::GetTexture() {
    LOG_LN("Get texture not implemented in basic entity!!!");
    return nullptr;
}

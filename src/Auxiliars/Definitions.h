#pragma once
//#include "Audio.h"
#include "../Engine/AudioManager.h"
#include "../Auxiliars/MathUtils.h"
#include "../Mechanics/Movement.h"
#include "../UI/Menu.h"
#include "../Entities/Entity.h"
#include "../Entities/Player.h"
#include "../Entities/Enemy.h"
#include "../Entities/Bullet.h"
#include "../Entities/Planet.h"
#include "../Engine/Clock.h"
#include "../DataManagement/Crypto.h"
#include "../DataManagement/Crypto.h"
#include "../DataManagement/HighScore.h"
#include "../DataManagement/PowerupManager.h"

//240 x 240 (smart watch)
//240 x 320 (QVGA)
//360 x 640 (Galaxy S5)
//640 x 480 (480i - Smallest PC monitor)
#define BULLET_SPAWN_DELAY_IN_MS 500
#define ENEMY_SPAWN_DELAY_IN_MS 1000
#define BULLET_SPLIT_DELAY_IN_MS 1050
#define MINIMUM_INTERVAL_BULLET_MS 2000
#define MAXIMUM_INTERVAL_BULLET_MS 7000
#define BULLET_COLLISION_DELAY_IN_MS 100
#define PLANET_SPAWN_DELAY_IN_MS 30000
#define HEALTH_SPAWN_MIN_DELAY_IN_MS 30000
#define HEALTH_SPAWN_MAX_DELAY_IN_MS 90000
#define PORTAL_SPAWN_DELAY_IN_MS 5000
#define STREAK_BOMB_REQUIREMENT 12
#define BOMB_RADIUS 500
#define SHOTGUN_MAX_AMMO 21


int windowWidth = 1280;
int windowHeight = 720;

SDL_Point mousePos;
SDL_Point realMousePos;
SDL_Point pauseMosPos;
bool keys[SDL_NUM_SCANCODES];
bool buttons[SDL_BUTTON_X2 + 1];
TTF_Font* robotoF;
TTF_Font* moonhouseF;
bool gameRunning = true;
bool appRunning = true;

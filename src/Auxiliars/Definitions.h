#pragma once
//#include "Audio.h"
#include "../Engine/AudioManager.h"
#include "../Auxiliars/MathUtils.h"
#include "../Auxiliars/GameManager.h"
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
#include "../DataManagement/TextureLoader.h"
#include "../DataManagement/LevelingManager.h"
//240 x 240 (smart watch)
//240 x 320 (QVGA)
//360 x 640 (Galaxy S5)
//640 x 480 (480i - Smallest PC monitor)
#define BULLET_SPAWN_DELAY_IN_MS 500
#define ENEMY_SPAWN_DELAY_IN_MS 1000
#define BULLET_SPLIT_DELAY_IN_MS 1500
#define MINIMUM_INTERVAL_BULLET_MS 4000
#define MAXIMUM_INTERVAL_BULLET_MS 7000
#define BULLET_COLLISION_DELAY_IN_MS 100
#define PLANET_SPAWN_DELAY_IN_MS 30000
#define HEALTH_SPAWN_MIN_DELAY_IN_MS 30000
#define HEALTH_SPAWN_MAX_DELAY_IN_MS 90000
#define PORTAL_SPAWN_DELAY_IN_MS 5000
#define STREAK_BOMB_REQUIREMENT 12
#define BOMB_RADIUS 500
#define SHOTGUN_MAX_AMMO 21

SDL_Point mousePos;
SDL_Point realMousePos;
SDL_Point pauseMosPos;
bool keys[SDL_NUM_SCANCODES];
bool buttons[SDL_BUTTON_X2 + 1];
TTF_Font* robotoF;
TTF_Font* moonhouseF;
bool gameRunning = true;
bool appRunning = true;

SDL_Texture* bgT;
SDL_Texture* meatT;
SDL_Texture* explosionT;
SDL_Texture* buyT;
SDL_Texture* shieldT;
SDL_Texture* closeT;
SDL_Texture* coinsT;
SDL_Texture* uiSelectedT;
SDL_Texture* portalT;
SDL_Texture* shotgunT;
SDL_Texture* controlsT;
SDL_Texture* highScoreT;
SDL_Texture* mainT;

#pragma region Defined Methods
void AssociateTexturesToPtrs() {
	std::map<const char*, SDL_Texture**> textureAssociationMap = {
		{"Background", &bgT},
		{"Meat", &meatT},
		{"Gun", &explosionT},
		{"Portal", &portalT},
		{"Buy", &buyT},
		{"Shield", &shieldT},
		{"Close", &closeT},
		{"Coin", &coinsT},
		{"Shotgun", &shotgunT},
		{"ControlsMenu", &controlsT},
		{"HighScoresMenu", &highScoreT},
		{"Main", &mainT}
	};

	for (auto& [k, v] : textureAssociationMap)
		*v = TextureLoader::GetTextureByName(k);
}
#pragma endregion

#pragma region Declared Methods
void WindowInit();

void GlobalsInit();

void mainLoop();

void TexturesInit();

void UiInit();

void ClocksInit();

/// <summary>
/// Bounces one or two bullets off an entity
/// </summary>
/// <param name="a">Bullet to be bounced</param>
/// <param name="b">Entity to be bounced off (could be a bullet)</param>
/// <param name="affectB">True if the B parameter is also a bullet</param>
/// <returns>True if either of the bullets was destroyed</returns>
bool BounceOff(Entity* a, Entity* b, bool affectB);

void InputEvents(const SDL_Event& event);

void EnemySpawn();

void EntityMovement(const SDL_FRect& extendedWindowR);

void BulletCollisions(const SDL_FRect& extendedWindowR);

void EnemyBehavior(const SDL_FRect& extendedWindowR);

void PowerUpSpawner();

void RenderAll();

void FireWhenReady();

MenuOption DisplayMainMenu(TTF_Font* titleFont, TTF_Font* font);

void RenderPauseMenu(TTF_Font* font);

void PauseInit(TTF_Font* titleFont, TTF_Font* font);

void RenderGameOverMenu(TTF_Font* font);

void GameOverInit(TTF_Font* titleFont, TTF_Font* font);

void RenderControlsMenu(TTF_Font* font);

void RenderCreditsMenu(TTF_Font* font);

void RenderHighScoresMenu(TTF_Font* titleFont, TTF_Font* font);

void ControlsInit(TTF_Font* titleFont, TTF_Font* font);

void HandleMenuOption(MenuOption option);

void CheckAndAddHighScore(int score);

void ScoreKill();

void ShiftScores(const std::tuple<int, std::string>& toInsert);
#pragma endregion
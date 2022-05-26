#include "Auxiliars/Definitions.h"
#include "Auxiliars/SdlDefinitions.h"


enum class State {
	Gameplay,
	Credits,
	Controls,
	HighScores,
	Main
};

Player player;
std::vector<Bullet> bullets;
Clock bulletClock;
Clock enemyClock;
std::vector<Enemy> enemies;
Text killPointsText;
Text healthText;
std::vector<Planet> planets;
Clock planetClock;
std::vector<Entity> healthPickups;
Clock healthPickupClock;
std::vector<Entity> bombs;
Clock bombClock;
Text shieldPriceText;
Text shieldText;
Text shieldHealthText;
Text killStreakText;
Text bombText;
Text highScoreTextUI;
Text shotgunAmmoText;
SDL_FRect buyBtnR;
SDL_FRect buyR;
SDL_FRect buyShieldR;
SDL_FRect moneyR;
SDL_FRect shieldPrizeCoinsR;
Text moneyText;
int shieldHealth = 0;
SDL_FRect closeBtnR;
int healthSpawnTime = 0;
std::vector<SDL_FRect> portalRects;
Clock portalClock;
MenuButton pauseOptions[PAUSE_NUM_OPTIONS];
const std::string pauseLabels[PAUSE_NUM_OPTIONS] = { 
	"Resume", 
	"Return to Main Menu", 
	"Quit to Desktop" 
};
const MenuOption pauseMenuTypes[PAUSE_NUM_OPTIONS] = { 
	MenuOption::Resume,  
	MenuOption::Main, 
	MenuOption::Quit 
};
Text pauseTitleText;
SDL_FRect pauseContainer;
SDL_FRect uiSelector;
float pauseLabelLargest;
bool pausing = false;
bool pauseKeyHeld = false;
MenuButton gameOverOptions[GAMEOVER_NUM_OPTIONS];
const std::string gameOverLabels[GAMEOVER_NUM_OPTIONS] = {
	"Play Again",  
	"Return to Main Menu"
};
const MenuOption gameOverMenuTypes[GAMEOVER_NUM_OPTIONS] = {
	MenuOption::Restart,   
	MenuOption::Main
};

std::string highScoreInputName;

Text gameOverTitleText;
SDL_FRect gameOverContainer;
float gameOverLargest;
bool gameOver = false;
bool restart = false;
MenuButton controlsOptions;
const std::string controlsLabel = "Return";
MenuOption controlsMenuType = MenuOption::Main;
Text controlsTitleText;
SDL_FRect controlsTitleContainer;
SDL_FRect controlsOptionContainer;
AudioManager* audioManager = AudioManager::Instance();
Text shotgunPriceText;
Text shotgunText;
SDL_FRect buyShotgunR;
SDL_FRect buyShotgunBtnR;
State state = State::Gameplay;
Text highscoresTitleText;
SDL_FRect highscoresTitleContainer;
Text highscoresNameText;

//High score aux varaibles (not best practice)
std::tuple<int, std::string> allScores[HIGH_SCORES_LIMIT];
int indexToInsertHighScore = -1;
bool addingScore = false;
bool scoreAdded = false;

void mainLoop() {
	Clock::GetGlobalClock()->restart();
	SDL_Event event;

	SDL_FRect extendedWindowR;
	extendedWindowR.w = WindowManager::GetWindowWidth() + 5;
	extendedWindowR.h = WindowManager::GetWindowHeight() + 5;
	extendedWindowR.x = -5;
	extendedWindowR.y = -5;

	if (state == State::Gameplay) {
		while (SDL_PollEvent(&event)) {
			InputEvents(event);
		}
		if (!player.buyingShield && !player.buyingShotgun && !pausing && !gameOver) {
			player.dx = 0;
			player.dy = 0;
			//TODO: If the player can move (and refactor this to a player function)
			if (keys[SDL_SCANCODE_A]) {
				player.dx = -1;
			}
			else if (keys[SDL_SCANCODE_D]) {
				player.dx = 1;
			}
			if (keys[SDL_SCANCODE_W]) {
				player.dy = -1;
			}
			else if (keys[SDL_SCANCODE_S]) {
				player.dy = 1;
			}
			if (buttons[SDL_BUTTON_LEFT]) {
				FireWhenReady();
			}
			if (buttons[SDL_BUTTON_RIGHT] && player.hasBomb) {
				//If you have a bomb destroy every bullet and enemy within a radius
				for (size_t i = 0; i < bullets.size(); i++) {
					//Don't destroy the player's bullets
					if (bullets[i].GetTargetMask() == TargetMask::EnemiesMask)
						continue;
					//Check if the bullet is within the radius
					SDL_FPoint playerPos = {
						player.r.x,
						player.r.y
					};

					SDL_FPoint bulletPos = {
						bullets[i].r.x,
						bullets[i].r.y
					};

					float sqrDis = MathUtils::DistanceSqr(bulletPos, playerPos);

					if (sqrDis > (BOMB_RADIUS * BOMB_RADIUS))
						continue;
					bullets.erase(bullets.begin() + i--); //Destroy the bullet if it is within the radius
				}

				for (size_t i = 0; i < enemies.size(); i++) {
					SDL_FPoint playerPos = {
						player.r.x,
						player.r.y
					};

					SDL_FPoint enemyPos = {
						enemies[i].r.x,
						enemies[i].r.y
					};

					float sqrDis = MathUtils::DistanceSqr(enemyPos, playerPos);

					if (sqrDis > (BOMB_RADIUS * BOMB_RADIUS))
						continue;
					ScoreKill();
					enemies.erase(enemies.begin() + i--); //Destroy the enemy if it is within the radius
					audioManager->PlaySFX(SFXAudio::EnemyHit);
					audioManager->PlaySFX(SFXAudio::EnemyDeath);
				}

				//Reset the player's streak
				//TODO: Make this a function on the getters and setters of hasBomb
				player.streak = 0;
				player.hasBomb = false;
			}

			EnemySpawn();

			EntityMovement(extendedWindowR);

			BulletCollisions(extendedWindowR);

			EnemyBehavior(extendedWindowR);

			PowerUpSpawner();
			//To Review (more than one portal at a time)
			if (portalClock.getElapsedTime() > PORTAL_SPAWN_DELAY_IN_MS && portalRects.size() < 1) {
				portalRects.push_back(SDL_FRect());
				portalRects.back().w = 32;
				portalRects.back().h = 32;
				portalRects.back().x = MathUtils::Random(0, WindowManager::GetWindowWidth() - portalRects.back().w);
				portalRects.back().y = MathUtils::Random(0, WindowManager::GetWindowHeight() - portalRects.back().h);
				portalRects.push_back(SDL_FRect());
				portalRects.back().w = 32;
				portalRects.back().h = 32;
				portalRects.back().x = MathUtils::Random(0, WindowManager::GetWindowWidth() - portalRects.back().w);
				portalRects.back().y = MathUtils::Random(0, WindowManager::GetWindowHeight() - portalRects.back().h);
				portalClock.restart();
			}
			for (int i = 0; i < portalRects.size(); ++i) {
				if (SdlUtils::SDL_HasIntersectionF(&player.r, &portalRects[i])) {
					if (i % 2 != 0) {
						player.r.x = portalRects[i - 1].x;
						player.r.y = portalRects[i - 1].y;
					}
					else {
						player.r.x = portalRects[i + 1].x;
						player.r.y = portalRects[i + 1].y;
					}
					if (i % 2 != 0) {
						portalRects.erase(portalRects.begin() + i--);
						portalRects.erase(portalRects.begin() + i);
						portalClock.restart(); // To review
					}
					else {
						portalRects.erase(portalRects.begin() + i);
						portalRects.erase(portalRects.begin() + i--);
						portalClock.restart(); // To review
					}
				}
			}
		}

		if (player.GetHealth() <= 0 && !gameOver) {
			audioManager->PlaySFX(SFXAudio::PlayerDeath);
			highScoreInputName = "";
			gameOver = true;
			pauseMosPos = mousePos;
		}
		RenderAll();
	}
	else {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) {
				gameRunning = false;
				if (event.type == SDL_QUIT) {
					appRunning = false;
				}
				// TODO: On mobile remember to use eventWatch function (it doesn't reach this code when terminating)
			}
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
				SDL_RenderSetScale(SdlUtils::renderer, event.window.data1 / (float)WindowManager::GetWindowWidth(), event.window.data2 / (float)WindowManager::GetWindowHeight());
			}
			if (event.type == SDL_KEYDOWN) {
				keys[event.key.keysym.scancode] = true;
			}
			if (event.type == SDL_KEYUP) {
				keys[event.key.keysym.scancode] = false;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				buttons[event.button.button] = true;
				if ((state == State::Controls) || (state == State::Credits) || (state == State::HighScores)) {
					if (SdlUtils::SDL_PointInFRect(&mousePos, &controlsOptions.buttonText.dstR)) {
						audioManager->PlaySFX(SFXAudio::UISuccess);
						state = State::Main;
						HandleMenuOption(controlsOptions.menuType);
					}
					else {
						audioManager->PlaySFX(SFXAudio::UIFail);
					}
				}
			}
			if (event.type == SDL_MOUSEBUTTONUP) {
				buttons[event.button.button] = false;
			}
			if (event.type == SDL_MOUSEMOTION) {
				float scaleX, scaleY;
				SDL_RenderGetScale(SdlUtils::renderer, &scaleX, &scaleY);
				mousePos.x = event.motion.x / scaleX;
				mousePos.y = event.motion.y / scaleY;
				realMousePos.x = event.motion.x;
				realMousePos.y = event.motion.y;

				if ((state == State::Controls) || (state == State::Credits) || (state == State::HighScores)) {
					if (SdlUtils::SDL_PointInFRect(&mousePos, &controlsOptions.buttonText.dstR)) {
						controlsOptions.selected = true;
					}
					else {
						controlsOptions.selected = false;
					}
				}
			}
		}

		SDL_SetRenderDrawColor(SdlUtils::renderer, 0, 0, 0, 0);
		SDL_RenderClear(SdlUtils::renderer);

		if (state == State::Controls) {
			RenderControlsMenu(robotoF);
		}
		else if (state == State::Credits) {
			RenderCreditsMenu(robotoF);
		}
		else if (state == State::HighScores) {
			RenderHighScoresMenu(moonhouseF, robotoF);
		}
		SDL_RenderPresent(SdlUtils::renderer);
	}
}

void GlobalsInit() {
	//Right here we call all the data management functions
	PowerupManager::InitializePowerUps();
	LevelingManager::ReadAndInitializeExperience();
	GameManager::playerReference = &player;
	//Refactor this to an reset function on each entity
	player.ResetPlayer();

	bullets.clear();
	enemies.clear();
	killPointsText.setText(robotoF, "0");
	healthText.setText(robotoF, player.GetHealth(), { 255, 0, 0 });
	planets.clear();
	healthPickups.clear();
	bombs.clear();

	std::string sText = "Shield: " + std::to_string(shieldHealth);
	shieldHealthText.setText(robotoF, sText, { 255, 255, 255 });

	std::string streakText = "Kill streak: " + std::to_string(player.streak);
	killStreakText.setText(robotoF, streakText, { 255, 255, 255 });

	moneyText.setText(robotoF, 0);
	healthSpawnTime = 0;
	portalRects.clear();
	pausing = false;
	pauseKeyHeld = false;
	gameOver = false;
	restart = false;
	highScoreInputName = "";
	player.hasShotgun = false;
	addingScore = false;
	scoreAdded = false;
	SDL_StopTextInput();
}

int main(int argc, char* argv[])
{
	WindowInit();
	TexturesInit();

	player.r.w = 64;
	player.r.h = 64;

	uiSelector.w = 32;
	uiSelector.h = 32;

	audioManager->PlayMusic(MusicAudio::UIMusic);
	
	UiInit();
	PauseInit(moonhouseF, robotoF);
	GameOverInit(moonhouseF, robotoF);
	ControlsInit(moonhouseF, robotoF);
	ClocksInit();

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(mainLoop, 0, 1);
#else
	while (appRunning) {
		if (!restart) {
			MenuOption buttonType = DisplayMainMenu(moonhouseF, robotoF);
			HandleMenuOption(buttonType);
			
		}
		else {
			gameRunning = true;
		}

		GlobalsInit();
		ClocksInit();

		while (gameRunning) {
			mainLoop();
			float toDelay = Clock::CalculateDelay(Clock::GetDeltaTime());
			SDL_Delay(toDelay);
		}
	}
#endif
	// TODO: On mobile remember to use eventWatch function (it doesn't reach this code when terminating)
	return 0;
}

void TexturesInit() {
	TextureLoader::LoadEntityTextures(SdlUtils::renderer);
	AssociateTexturesToPtrs();
	uiSelectedT = IMG_LoadTexture(SdlUtils::renderer, "res/player.png");
}

void UiInit() {
	buyR.w = 200;
	buyR.h = 200;
	buyR.x = WindowManager::GetWindowWidth() / 2 - buyR.w / 2;
	buyR.y = WindowManager::GetWindowHeight() / 2 - buyR.h / 2;

	killPointsText.dstR.w = 30;
	killPointsText.dstR.h = 20;
	killPointsText.dstR.x = WindowManager::GetWindowWidth() / 2 - killPointsText.dstR.w / 2;
	killPointsText.dstR.y = 0;
	killPointsText.setText(robotoF, "0");

	healthText.dstR.w = 30;
	healthText.dstR.h = 20;
	healthText.dstR.x = WindowManager::GetWindowWidth() - healthText.dstR.w;
	healthText.dstR.y = 0;
	healthText.setText(robotoF, player.GetHealth(), { 255, 0, 0 });

	shieldHealthText.dstR.w = 50;
	shieldHealthText.dstR.h = 30;
	shieldHealthText.dstR.x = WindowManager::GetWindowWidth() / 3 - shieldPriceText.dstR.w / 3;
	shieldHealthText.dstR.y = 0;
	std::string sText = "Shield: " + std::to_string(shieldHealth);
	shieldHealthText.setText(robotoF, sText, { 255, 255, 255 });

	
	killStreakText.dstR.w = 100;
	killStreakText.dstR.h = 30;
	killStreakText.dstR.x = WindowManager::GetWindowWidth() / 4 - killStreakText.dstR.w / 4;
	killStreakText.dstR.y = 0;
	std::string streakText = "Kill streak: " + std::to_string(player.streak);
	killStreakText.setText(robotoF, streakText, { 255, 255, 255 });


	bombText.dstR.w = 120;
	bombText.dstR.h = 30;
	bombText.dstR.x = WindowManager::GetWindowWidth() / 7 - shieldPriceText.dstR.w / 7;
	bombText.dstR.y = 0;
	bombText.setText(robotoF, "[LEFT CLICK FOR BOMB]", {255, 0, 0});

	shotgunAmmoText.dstR.w = 120;
	shotgunAmmoText.dstR.h = 30;
	shotgunAmmoText.dstR.x = WindowManager::GetWindowWidth() / 30 - shieldPriceText.dstR.w / 30;
	shotgunAmmoText.dstR.y = 0;
	std::string ammoStr = "Shotgun Ammo: " + std::to_string(player.shotgunAmmo);
	shotgunAmmoText.setText(robotoF, ammoStr, { 255, 255, 255 });

	shieldPriceText.setText(robotoF, "50");
	shieldPriceText.dstR.w = 100;
	shieldPriceText.dstR.h = 40;
	shieldPriceText.dstR.x = WindowManager::GetWindowWidth() / 2 - shieldPriceText.dstR.w / 2;
	shieldPriceText.dstR.y = buyR.y + 10;

	highScoreTextUI.setText(robotoF, "50");
	highScoreTextUI.dstR.w = 800;
	highScoreTextUI.dstR.h = 40;
	highScoreTextUI.dstR.x = WindowManager::GetWindowWidth() / 2 - highScoreTextUI.dstR.w / 2;
	highScoreTextUI.dstR.y = buyR.y + 10;

	highscoresNameText.setText(robotoF, "50");
	highscoresNameText.dstR.w = 100;
	highscoresNameText.dstR.h = 40;
	highscoresNameText.dstR.x = WindowManager::GetWindowWidth() / 2 - highscoresNameText.dstR.w / 2;
	highscoresNameText.dstR.y = highScoreTextUI.dstR.y + highScoreTextUI.dstR.h + 10;

	buyShieldR.w = 32;
	buyShieldR.h = 32;
	buyShieldR.x = shieldPriceText.dstR.x + shieldPriceText.dstR.w / 2 - buyShieldR.w / 2;
	buyShieldR.y = shieldPriceText.dstR.y + shieldPriceText.dstR.h;

	shieldText.setText(robotoF, "Shield");
	shieldText.dstR.w = 100;
	shieldText.dstR.h = 40;
	shieldText.dstR.x = buyShieldR.x + buyShieldR.w / 2 - shieldText.dstR.w / 2;
	shieldText.dstR.y = buyShieldR.y + buyShieldR.h;

	buyBtnR.w = 100;
	buyBtnR.h = 60;
	buyBtnR.x = shieldText.dstR.x + shieldText.dstR.w / 2 - buyBtnR.w / 2;
	buyBtnR.y = shieldText.dstR.y + shieldText.dstR.h;

	closeBtnR.w = 32;
	closeBtnR.h = 32;
	closeBtnR.x = buyR.x + buyR.w - closeBtnR.w / 2;
	closeBtnR.y = buyR.y - closeBtnR.h / 2;

	moneyR.w = 32;
	moneyR.h = 32;
	moneyR.x = WindowManager::GetWindowWidth() - moneyR.w;
	moneyR.y = healthText.dstR.y + healthText.dstR.h;

	moneyText.setText(robotoF, 0);
	moneyText.dstR.w = 30;
	moneyText.dstR.h = 20;
	moneyText.dstR.x = moneyR.x - moneyText.dstR.w;
	moneyText.dstR.y = healthText.dstR.y + healthText.dstR.h;

	shieldPrizeCoinsR.w = 32;
	shieldPrizeCoinsR.h = 32;
	shieldPrizeCoinsR.x = shieldPriceText.dstR.x + shieldPriceText.dstR.w;
	shieldPrizeCoinsR.y = shieldPriceText.dstR.y;

	shotgunPriceText = shieldPriceText;
	shotgunPriceText.setText(robotoF, 100);

	shotgunText = shieldText;
	shotgunText.setText(robotoF, "Shotgun");

	buyShotgunR = buyShotgunBtnR;

	buyShotgunBtnR = buyBtnR;
}

void ClocksInit()
{
	Clock::GetGlobalClock()->restart();
	bulletClock.restart();
	enemyClock.restart();
	planetClock.restart();
	healthPickupClock.restart();
	portalClock.restart();
}

void WindowInit()
{
	std::srand(std::time(0));
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
	SDL_LogSetOutputFunction(logOutputCallback, 0);
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	//Mix_OpenAudio(AUDIO_FREQUENCY, MIX_DEFAULT_FORMAT, AUDIO_NUM_CHANNELS, AUDIO_CHUNK_SIZE);
	SDL_GetMouseState(&mousePos.x, &mousePos.y);
	window = SDL_CreateWindow("TotalChaos", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowManager::GetWindowWidth(), WindowManager::GetWindowHeight(), SDL_WINDOW_RESIZABLE);
	SdlUtils::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	robotoF = TTF_OpenFont("res/roboto.ttf", 72);
	moonhouseF = TTF_OpenFont("res/moonhouse.ttf", 72);
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	SDL_RenderSetScale(SdlUtils::renderer, w / (float)WindowManager::GetWindowWidth(), h / (float)WindowManager::GetWindowHeight());
	SDL_AddEventWatch(SdlUtils::eventWatch, 0);
	SDL_SetRenderDrawBlendMode(SdlUtils::renderer, SDL_BLENDMODE_BLEND);
}

bool BounceOff(Entity* a, Entity* b, bool affectB) {
	bool wasDeleted = false;

	float v1 = MathUtils::GetMagnitude(a->dx, a->dy);
	float v2 = MathUtils::GetMagnitude(b->dx, b->dy);

	float x1 = a->r.x + a->r.w / 2.0f;
	float y1 = a->r.y + a->r.h / 2.0f;
	float x2 = b->r.x + b->r.w / 2.0f;
	float y2 = b->r.y + b->r.h / 2.0f;

	float t1 = MathUtils::GetAngle(a->dx, a->dy, x1, y1);
	float t2 = MathUtils::GetAngle(b->dx, b->dy, x2, y2);

	float tContact = MathUtils::GetAngle(x2, y2, x1, y1);

	SDL_FPoint directionA = MathUtils::CalculateCollision(v1, v2, t1, t2, tContact);
	MathUtils::Normalize(&directionA);
	SDL_FPoint directionB = MathUtils::CalculateCollision(v2, v1, t2, t1, tContact);
	MathUtils::Normalize(&directionB);

	a->dx = directionA.x;
	a->dy = directionA.y;
	
	Bullet* bulletReference = (Bullet*)a;
	bulletReference->DecreaseSize();

	//This is the case for the shield
	if (!affectB)
		return wasDeleted;
	b->dx = directionB.x;
	b->dy = directionB.y;
	
	bulletReference = (Bullet*)b;
	bulletReference->DecreaseSize();
	return false;
}

void InputEvents(const SDL_Event& event) {
	//TODO: Refactor to switch
	if (event.type == SDL_QUIT) {
		gameRunning = false;
		appRunning = false;
		// TODO: On mobile remember to use eventWatch function (it doesn't reach this code when terminating)
	}
	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
		SDL_RenderSetScale(SdlUtils::renderer, event.window.data1 / (float)WindowManager::GetWindowWidth(), event.window.data2 / (float)WindowManager::GetWindowHeight());
	}
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && !pauseKeyHeld && !gameOver) {
			pausing = !pausing;
			pauseKeyHeld = true;
			pauseMosPos = mousePos;

			pausing ? audioManager->PauseMusic() : audioManager->ResumeMusic();
		}

		//Handle backspace for input
		if (event.key.keysym.sym == SDLK_BACKSPACE && highScoreInputName.length() > 0 && !scoreAdded && addingScore) {
			//lop off character
			highScoreInputName.pop_back();
			//renderText = true;
		}

		//Add a score
		if ((event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN) && addingScore && !scoreAdded && highScoreInputName.length() > 0) {
			ShiftScores(std::make_tuple(std::stoi(killPointsText.text), highScoreInputName));
			//Send the score to the file 
			HighScores::WriteHighScore(allScores);
			addingScore = false;
			scoreAdded = true;
		}

		keys[event.key.keysym.scancode] = true;
	}

	//Keyboard input for game over
	//Special text input event
	else if (event.type == SDL_TEXTINPUT)
	{
		//Not copy or pasting
		if (!(SDL_GetModState() & KMOD_CTRL && (event.text.text[0] == 'c' || event.text.text[0] == 'C' || event.text.text[0] == 'v' || event.text.text[0] == 'V'))
			&& event.text.text[0] != ' ' && highScoreInputName.length() < 6) {
			//Append character
			highScoreInputName += event.text.text;
		}
	}

	if (event.type == SDL_KEYUP) {
		if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
			pauseKeyHeld = false;
		}
		keys[event.key.keysym.scancode] = false;
	}
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		bool uiSuccess = false;
		bool menuSuccess = false;
		bool insufficientMoney = false;
		buttons[event.button.button] = true;
		if (gameOver) {
			buttons[event.button.button] = false;

			// Game is over, so you can only interact with the menu on button clicks
			for (int i = 0; i < PAUSE_NUM_OPTIONS; ++i) {
				if (SdlUtils::SDL_PointInFRect(&mousePos, &gameOverOptions[i].buttonText.dstR)) {
					HandleMenuOption(gameOverOptions[i].menuType);
					uiSuccess = true;
				}
			}
		}
		else if (pausing) {
			buttons[event.button.button] = false;
			// Game is paused, so you can only interact with the menu on button clicks
			for (int i = 0; i < PAUSE_NUM_OPTIONS; ++i) {
				if (SdlUtils::SDL_PointInFRect(&mousePos, &pauseOptions[i].buttonText.dstR)) {
					HandleMenuOption(pauseOptions[i].menuType);
					uiSuccess = true;
				}
			}
		}
		else {
			if (SdlUtils::SDL_PointInFRect(&mousePos, &closeBtnR) && (player.buyingShield || player.buyingShotgun)) {
				player.buyingShield = false;
				player.buyingShotgun = false;
				menuSuccess = true;
			}
			if (SdlUtils::SDL_PointInFRect(&mousePos, &buyBtnR) && player.buyingShield) {
				if (std::stoi(moneyText.text) >= std::stoi(shieldPriceText.text)) {
					player.hasShield = true;
					shieldHealth = 10;
#if _DEBUG
					std::string sText = "Shield: " + std::to_string(shieldHealth);
					shieldHealthText.setText(robotoF, sText, { 255, 255, 255 });
#endif
					moneyText.setText(robotoF, std::stoi(moneyText.text) - std::stoi(shieldPriceText.text));
					menuSuccess = true;
				}
				else {
					insufficientMoney = true;
				}
			}
			if (SdlUtils::SDL_PointInFRect(&mousePos, &buyShotgunBtnR) && player.buyingShotgun) {
				if (std::stoi(moneyText.text) >= std::stoi(shotgunPriceText.text)) {
					player.hasShotgun = true;
					player.shotgunAmmo = SHOTGUN_MAX_AMMO;
					moneyText.setText(robotoF, std::stoi(moneyText.text) - std::stoi(shotgunPriceText.text));
					menuSuccess = true;
				}
				else {
					insufficientMoney = true;
				}
			}
		}

		if (uiSuccess) {
			audioManager->PlaySFX(SFXAudio::UISuccess);
		}
		else {
			if (pausing || gameOver) {
				audioManager->PlaySFX(SFXAudio::UIFail);
			}
			else if (menuSuccess) {
				audioManager->PlaySFX(SFXAudio::MenuSuccess);
			}
			else if (insufficientMoney) {
				audioManager->PlaySFX(SFXAudio::MenuFail);
			}
		}		
	}
	if (event.type == SDL_MOUSEBUTTONUP) {
		buttons[event.button.button] = false;
	}
	if (event.type == SDL_MOUSEMOTION) {
		float scaleX, scaleY;
		SDL_RenderGetScale(SdlUtils::renderer, &scaleX, &scaleY);
		mousePos.x = event.motion.x / scaleX;
		mousePos.y = event.motion.y / scaleY;
		realMousePos.x = event.motion.x;
		realMousePos.y = event.motion.y;
		if (gameOver) {
			for (int i = 0; i < GAMEOVER_NUM_OPTIONS; ++i) {
				if (SdlUtils::SDL_PointInFRect(&mousePos, &gameOverOptions[i].buttonText.dstR)) {
					gameOverOptions[i].selected = true;
				}
				else {
					gameOverOptions[i].selected = false;
				}
			}
		}
		if (pausing) {
			// Game is paused, so you can only interact with the menu buttons
			for (int i = 0; i < PAUSE_NUM_OPTIONS; ++i) {
				if (SdlUtils::SDL_PointInFRect(&mousePos, &pauseOptions[i].buttonText.dstR)) {
					pauseOptions[i].selected = true;
				}
				else {
					pauseOptions[i].selected = false;
				}
			}
		}
	}
}

void EnemySpawn() {
	if (enemyClock.getElapsedTime() > ENEMY_SPAWN_DELAY_IN_MS) {
		//Create a random interval for the shooting between a range defined by constants
		int bulletInterval = MathUtils::Random(MINIMUM_INTERVAL_BULLET_MS, MAXIMUM_INTERVAL_BULLET_MS);
		enemies.push_back(Enemy(bulletInterval));

		switch (enemies.back().spawnPlace) {
		case SpawnPlace::Up:
			enemies.back().r.x = MathUtils::Random(0, WindowManager::GetWindowWidth() - enemies.back().r.w);
			enemies.back().r.y = 0;
			enemies.back().dy = 1;
			break;
		case SpawnPlace::Down:
			enemies.back().r.x = MathUtils::Random(0, WindowManager::GetWindowWidth() - enemies.back().r.w);
			enemies.back().r.y = WindowManager::GetWindowHeight();
			enemies.back().dy = -1;
			break;
		case SpawnPlace::Left:
			enemies.back().r.x = -enemies.back().r.w;
			enemies.back().r.y = MathUtils::Random(0, WindowManager::GetWindowHeight() - enemies.back().r.h);
			enemies.back().dx = 1;
			break;
		case SpawnPlace::Right:
			enemies.back().r.x = WindowManager::GetWindowWidth();
			enemies.back().r.y = MathUtils::Random(0, WindowManager::GetWindowHeight() - enemies.back().r.h);
			enemies.back().dx = -1;
			break;
		}
		enemyClock.restart();
	}
}

void EntityMovement(const SDL_FRect& extendedWindowR) {
	float deltaTime = Clock::GetDeltaTime();
	//Bullets movement
	Bullet::MoveBulletGroup(&bullets);
	//Player movement
	player.MoveBehaviour(WindowManager::GetWindowWidth(), WindowManager::GetWindowHeight());
	//Enemy movement
	Enemy::MoveEnemyGroup(&enemies);
	//Planet's movement
	Planet::MovePlanetGroup(&planets, &player, extendedWindowR, SdlUtils::SDL_HasIntersectionF);
	//TODO: Refactor this into a single function (we can take a function ptr for special actions)
	//Health pickup's movement
	for (int i = 0; i < healthPickups.size(); ++i) {
		healthPickups[i].r.x += healthPickups[i].dx * deltaTime * PLANET_SPEED;
		healthPickups[i].r.y += healthPickups[i].dy * deltaTime * PLANET_SPEED;
		if (SdlUtils::SDL_HasIntersectionF(&player.r, &healthPickups[i].r)) {
			player.SetHealth(player.GetHealth() + 5);
			healthText.setText(robotoF, player.GetHealth(), { 255, 0, 0 });
		}
		if (SdlUtils::SDL_HasIntersectionF(&player.r, &healthPickups[i].r)
			|| !SdlUtils::SDL_HasIntersectionF(&healthPickups[i].r, &extendedWindowR)) {
			healthPickups.erase(healthPickups.begin() + i--);
		}
	}

	//Explosion's movement
	for (size_t i = 0; i < bombs.size(); ++i) {
		bombs[i].r.x += bombs[i].dx * deltaTime * PLANET_SPEED;
		bombs[i].r.y += bombs[i].dy * deltaTime * PLANET_SPEED;
		if (SdlUtils::SDL_HasIntersectionF(&player.r, &bombs[i].r)) {
			player.hasBomb = true;
		}
		if (SdlUtils::SDL_HasIntersectionF(&player.r, &bombs[i].r)
			|| !SdlUtils::SDL_HasIntersectionF(&bombs[i].r, &extendedWindowR)) {
			bombs.erase(bombs.begin() + i--);
		}
	}
}

void BulletCollisions(const SDL_FRect& extendedWindowR) {
deleteCollidingBegin:
	for (int i = 0; i < bullets.size(); ++i) {
		//Player collision
		if ((bullets[i].GetTargetMask() & TargetMask::PlayerMask) != 0) {
			if (SdlUtils::SDL_HasIntersectionF(&bullets[i].r, &player.r)) {
				if (player.hasShield) {
					bool willDamage = !bullets[i].bouncedOffShield;

					if (bullets[i].bouncedOffShield && bullets[i].shieldBounceDelay.getElapsedTime() > BULLET_SHIELD_BOUNCE_TOLERANCE) {
						bullets[i].bouncedOffShield = false;
					}
					else {
						bullets[i].bouncedOffShield = true;
						bullets[i].shieldBounceDelay.restart();
					}

					if (willDamage) {
						shieldHealth--;
						std::string sText = "Shield: " + std::to_string(shieldHealth);
						shieldHealthText.setText(robotoF, sText, { 255, 255, 255 });
					}

					if (shieldHealth <= 0) {
						player.hasShield = false;
						planetClock.restart();
					}
					if (BounceOff(&bullets[i], &player, false)) goto deleteCollidingBegin;
				}
				else {
					audioManager->PlaySFX(SFXAudio::PlayerHit);
					player.SetHealth(player.GetHealth() - 1);
					bullets.erase(bullets.begin() + i--);
					healthText.setText(robotoF, player.GetHealth(), { 255, 0, 0 });

					std::string streakText = "Kill streak: " + std::to_string(player.streak);
					killStreakText.setText(robotoF, streakText, { 255, 255, 255 });
					goto deleteCollidingBegin;
				}
			}
		}

		//Enemy collision
		for (int j = 0; j < enemies.size(); ++j) {
			//Layer control
			//TODO: Add validation for bullet indexing, since it can happen that i is out of range
			if ((bullets.at(i).GetTargetMask() & TargetMask::EnemiesMask) == 0)
				continue;
			if (SdlUtils::SDL_HasIntersectionF(&bullets.at(i).r, &enemies[j].r)) {

				audioManager->PlaySFX(SFXAudio::EnemyHit);
				audioManager->PlaySFX(SFXAudio::EnemyDeath);
				Enemy::HandleInstanceDmg(&enemies, j);
				bullets.erase(bullets.begin() + i--);
				ScoreKill();

				std::string streakText = "Kill streak: " + std::to_string(player.streak);
				killStreakText.setText(robotoF, streakText, { 255, 255, 255 });
				goto deleteCollidingBegin;
			}
		}

		if ((bullets[i].lifetime + Clock::GetDeltaTime()) > BULLET_SPLIT_DELAY_IN_MS) {
			bullets[i].Split(&bullets);
			bullets[i].lifetime = 0;
		}
		else {
			bullets[i].lifetime += Clock::GetDeltaTime();
		}

		//Bullet collision
		if (i < (bullets.size() - 1)) {
			for (int j = i + 1; j < bullets.size(); ++j) {
				bool timePassed = (bullets.at(i).lifetime > BULLET_COLLISION_DELAY_IN_MS) && (bullets[j].lifetime > BULLET_COLLISION_DELAY_IN_MS);
				if (SdlUtils::SDL_HasIntersectionF(&bullets.at(i).r, &bullets[j].r) && timePassed) {
					//If the bullet bounces and gets destroyed, restart the loop
					if (BounceOff(&bullets.at(i), &bullets[j], true)) goto deleteCollidingBegin;
				}
			}
		}

		//Window collision
		if (!SdlUtils::SDL_HasIntersectionF(&bullets.at(i).r, &extendedWindowR)) {
			bullets.erase(bullets.begin() + i--);
			goto deleteCollidingBegin;
		}
	}
}

void EnemyBehavior(const SDL_FRect& extendedWindowR) {
	for (int i = 0; i < enemies.size(); ++i) {
		SDL_FPoint playerPos = {
			player.r.x,
			player.r.y
		};

		enemies[i].Combat(enemies[i].GetTexture(), &bullets, playerPos, SdlUtils::renderer);
		SDL_FRect enemyR = enemies[i].r;
		--enemyR.x;
		--enemyR.y;
		enemyR.w += 2;
		enemyR.h += 2;
		if (!SdlUtils::SDL_HasIntersectionF(&enemies[i].r, &extendedWindowR)) {
			enemies.erase(enemies.begin() + i--);
		}
	}
}

void PowerUpSpawner() {
	if (planetClock.getElapsedTime() > PLANET_SPAWN_DELAY_IN_MS && !player.hasShield) {
		PlanetType planetType = Planet::RandomPlanetType(player.hasShotgun);
		planets.push_back(Planet(planetType, WindowManager::GetWindowWidth()));
		planetClock.restart();
	}

	//Randomize the spawn time of the health
	healthSpawnTime = healthSpawnTime == 0 ? MathUtils::Random(HEALTH_SPAWN_MIN_DELAY_IN_MS, HEALTH_SPAWN_MAX_DELAY_IN_MS) : healthSpawnTime;

	if (healthPickupClock.getElapsedTime() > healthSpawnTime) {
		healthPickups.push_back(Entity());
		healthPickups.back().r.w = 64;
		healthPickups.back().r.h = 64;
		healthPickups.back().r.x = MathUtils::Random(0, WindowManager::GetWindowWidth() - healthPickups.back().r.w);
		healthPickups.back().r.y = -healthPickups.back().r.h;
		healthPickups.back().dy = 1;
		healthSpawnTime = 0;
		healthPickupClock.restart();
	}

	if (!player.hasBomb && player.streak >= STREAK_BOMB_REQUIREMENT && bombs.size() < 1) {
		//Create the bomb
		bombs.push_back(Entity());
		bombs.back().r.w = 64;
		bombs.back().r.h = 64;
		bombs.back().r.x = MathUtils::Random(0, WindowManager::GetWindowWidth() - bombs.back().r.w);
		bombs.back().r.y = -bombs.back().r.h;
		bombs.back().dy = 1;
	}
}

void RenderAll() {
	SDL_SetRenderDrawColor(SdlUtils::renderer, 0, 0, 0, 0);
	SDL_RenderClear(SdlUtils::renderer);
	SDL_RenderCopyF(SdlUtils::renderer, bgT, 0, 0);
	if (player.hasShield) {
		SDL_RenderCopyF(SdlUtils::renderer, shieldT, 0, &player.r);
		shieldHealthText.draw();
	}
	if (pausing || gameOver) {
		RotateEntityTowards(player.GetTexture(), player, pauseMosPos, SdlUtils::renderer);
	}
	else {
		RotateEntityTowards(player.GetTexture(), player, mousePos, SdlUtils::renderer);
		//SDL_RenderCopyF(SdlUtils::renderer, player.GetTexture(), 0, &player.r);
	}

	//Render bullets
	for (int i = 0; i < bullets.size(); ++i) {
		SDL_Texture* textureVariant = bullets[i].GetTexture();
		SDL_RenderCopyF(SdlUtils::renderer, textureVariant, 0, &bullets[i].r);
	}

	//Render enemies
	for (int i = 0; i < enemies.size(); ++i) {
		//Based on the enemies' spawn position, rotate them to the proper angle
		double angles[] = { 180.0, 0.0, 90.0, 270.0 }; // Array of rotations mapped to the enum of spawn positions
		int index = (int)enemies[i].spawnPlace;
		SDL_RenderCopyExF(SdlUtils::renderer, enemies[i].GetTexture(), 0, &enemies[i].r, angles[index], 0, SDL_FLIP_NONE);
	}
	//Render UI text
	killPointsText.draw();
	healthText.draw();
	killStreakText.draw();
	shotgunAmmoText.draw();

	if (player.hasBomb)
		bombText.draw();

	//Renders planets
	for (int i = 0; i < planets.size(); ++i) {
		SDL_Texture* textureVariant = planets[i].GetTexture();
		SDL_RenderCopyF(SdlUtils::renderer, textureVariant, 0, &planets[i].r);
	}

	//Renders meat
	for (size_t i = 0; i < healthPickups.size(); i++) {
		SDL_RenderCopyF(SdlUtils::renderer, meatT, 0, &healthPickups[i].r);
	}

	//Render portals
	for (int i = 0; i < portalRects.size(); ++i) {
		SDL_RenderCopyF(SdlUtils::renderer, portalT, 0, &portalRects[i]);
	}

	//Render bomb
	for (size_t i = 0; i < bombs.size(); i++) {
		SDL_RenderCopyF(SdlUtils::renderer, explosionT, 0, &bombs[i].r);
	}

	SDL_RenderCopyF(SdlUtils::renderer, coinsT, 0, &moneyR);
	moneyText.draw();

	if (player.buyingShield) {
		SDL_SetRenderDrawColor(SdlUtils::renderer, 125, 125, 125, 0);
		SDL_RenderFillRectF(SdlUtils::renderer, &buyR);
		shieldPriceText.draw();
		shieldText.draw();
		SDL_RenderCopyF(SdlUtils::renderer, buyT, 0, &buyBtnR);
		SDL_RenderCopyF(SdlUtils::renderer, shieldT, 0, &buyShieldR);
		SDL_RenderCopyF(SdlUtils::renderer, closeT, 0, &closeBtnR);
		SDL_RenderCopyF(SdlUtils::renderer, coinsT, 0, &shieldPrizeCoinsR);
	}
	if (player.buyingShotgun) {
		SDL_SetRenderDrawColor(SdlUtils::renderer, 125, 125, 125, 0);
		SDL_RenderFillRectF(SdlUtils::renderer, &buyR);
		shotgunPriceText.draw();
		shotgunText.draw();
		SDL_RenderCopyF(SdlUtils::renderer, shotgunT, 0, &buyShotgunR);
		SDL_RenderCopyF(SdlUtils::renderer, closeT, 0, &closeBtnR);
		SDL_RenderCopyF(SdlUtils::renderer, buyT, 0, &buyShotgunBtnR);
	}

	if (pausing) {
		RenderPauseMenu(robotoF);
	}

	if (gameOver) {
		RenderGameOverMenu(robotoF);
	}

	SDL_RenderPresent(SdlUtils::renderer);
}

void FireWhenReady() {
	if (bulletClock.getElapsedTime() > BULLET_SPAWN_DELAY_IN_MS) {
		SFXAudio weaponType = SFXAudio::PlayerFire1;
		//TODO: Encapsulate this in a function (and probably a different file)
		bullets.push_back(Bullet(TargetMask::EnemiesMask));
		bullets.back().r.x = player.r.x + player.r.w / 2 - bullets.back().r.w / 2;
		bullets.back().r.y = player.r.y - bullets.back().r.h / 2;

		//Get the direction on dy and dx from the normalized position of the mouse
		SDL_FPoint fMousePos = MathUtils::ToSDL_FPoint(mousePos);
		SDL_FPoint playerPos = {
			player.r.x,
			player.r.y
		};

		SDL_FPoint finalPos = MathUtils::VectorSubstract(fMousePos, playerPos);

		MathUtils::Normalize(&finalPos);

		bullets.back().dy = finalPos.y;
		bullets.back().dx = finalPos.x;

		if (player.hasShotgun) {
			weaponType = SFXAudio::PlayerFire2;
			player.shotgunAmmo--;

			std::string ammoStr = "Shotgun Ammo: " + std::to_string(player.shotgunAmmo);
			shotgunAmmoText.setText(robotoF, ammoStr, { 255, 255, 255 });

			bullets.push_back(bullets.back());
			float angle = -135.0f * M_PI / 180.0f;
			bullets.back().dx = -finalPos.x * cosf(angle) + finalPos.y * sinf(angle);
			bullets.back().dy = -finalPos.x * sinf(angle) - finalPos.y * cosf(angle);

			bullets.push_back(bullets.back());
			angle = 135.0f * M_PI / 180.0f;
			bullets.back().dx = -finalPos.x * cosf(angle) + finalPos.y * sinf(angle);
			bullets.back().dy = -finalPos.x * sinf(angle) - finalPos.y * cosf(angle);
			if (player.shotgunAmmo <= 0)
				player.hasShotgun = false;
		}
		audioManager->PlaySFX(weaponType);
		bulletClock.restart();
	}
}

#pragma region Menu Functions
MenuOption DisplayMainMenu(TTF_Font* titleFont, TTF_Font* font)
{
	const int NUMMENU = 5;
	MenuButton buttons[NUMMENU];
	const std::string labels[NUMMENU] = { "Play", "Controls", "HighScores", "Credits", "Exit" };
	const MenuOption menuTypes[NUMMENU] = { MenuOption::Play, MenuOption::Controls, MenuOption::Highscores, MenuOption::Credits, MenuOption::Quit };

	// Setup background and title

	Text titleText;
	titleText.dstR.w = WindowManager::GetWindowWidth() - 100;
	titleText.dstR.h = 100;
	titleText.dstR.x = WindowManager::GetWindowWidth() / 2.0f - titleText.dstR.w / 2.0f;
	titleText.dstR.y = titleText.dstR.h / 2.0f;
	titleText.setText(titleFont, "Total Chaos In Space", TITLE_COLOR);

	float largestWidth = 0;
	// Setup buttons
	for (int i = 0; i < NUMMENU; ++i) {
		buttons[i].label = labels[i];
		buttons[i].menuType = menuTypes[i];
		buttons[i].selected = false;
		buttons[i].buttonText.dstR.w = strlen(labels[i].c_str()) * LETTER_WIDTH;
		buttons[i].buttonText.dstR.h = 40;
		CalculateButtonPosition(&buttons[i].buttonText.dstR, i, NUMMENU, WindowManager::GetWindowWidth(), WindowManager::GetWindowHeight(), MAIN_MENU_BUTTON_PADDING);
		buttons[i].buttonText.dstR.y += titleText.dstR.h;
		buttons[i].buttonText.setText(font, buttons[i].label, BUTTON_UNSELECTED);
		largestWidth = std::max(buttons[i].buttonText.dstR.w, largestWidth);
	}

	// Setup pointer by selected button.
	SDL_FRect uiSelectedR;
	uiSelectedR.w = 32;
	uiSelectedR.h = 32;

	SDL_Event eventMenu;
	Clock menuClock;
	while (true) {
		menuClock.restart();
		SDL_RenderClear(SdlUtils::renderer);
		SDL_RenderCopyF(SdlUtils::renderer, mainT, 0, 0);
		while (SDL_PollEvent(&eventMenu)) {
			switch (eventMenu.type) {
			case SDL_QUIT:
				return MenuOption::Quit;
			case SDL_WINDOWEVENT:
				if (eventMenu.window.event == SDL_WINDOWEVENT_RESIZED) {
					SDL_RenderSetScale(SdlUtils::renderer, eventMenu.window.data1 / (float)WindowManager::GetWindowWidth(), eventMenu.window.data2 / (float)WindowManager::GetWindowHeight());
				}
				break;
			case SDL_MOUSEMOTION:
				float scaleX, scaleY;
				SDL_RenderGetScale(SdlUtils::renderer, &scaleX, &scaleY);
				mousePos.x = eventMenu.motion.x / scaleX;
				mousePos.y = eventMenu.motion.y / scaleY;
				realMousePos.x = eventMenu.motion.x;
				realMousePos.y = eventMenu.motion.y;

				for (int i = 0; i < NUMMENU; ++i) {
					if (SdlUtils::SDL_PointInFRect(&mousePos, &buttons[i].buttonText.dstR)) {
						if (!buttons[i].selected) {
							buttons[i].selected = true;
							buttons[i].buttonText.setText(font, buttons[i].label, BUTTON_SELECTED);
						}
					}
					else {
						if (buttons[i].selected) {
							buttons[i].selected = false;
							buttons[i].buttonText.setText(font, buttons[i].label, BUTTON_UNSELECTED);
						}
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				for (int i = 0; i < NUMMENU; ++i) {
					if (SdlUtils::SDL_PointInFRect(&mousePos, &buttons[i].buttonText.dstR)) {
						audioManager->PlaySFX(SFXAudio::UISuccess);
						return buttons[i].menuType;
					}
				}
				audioManager->PlaySFX(SFXAudio::UIFail);
				break;
			}
		}

		titleText.draw();
		for (MenuButton button : buttons) {
			if (button.selected) {
				uiSelectedR.x = WindowManager::GetWindowWidth() / 2.0f - largestWidth / 2.0f - uiSelectedR.w - POINTER_OFFSET;
				uiSelectedR.y = button.buttonText.dstR.y;
				SDL_RenderCopyF(SdlUtils::renderer, uiSelectedT, 0, &uiSelectedR);
			}
			button.buttonText.draw();
		}

		SDL_RenderPresent(SdlUtils::renderer);
		SDL_Delay(Clock::CalculateDelay(menuClock.getElapsedTime()));
	}
}

void RenderPauseMenu(TTF_Font* font)
{
	SDL_SetRenderDrawColor(SdlUtils::renderer, 20, 20, 30, 200);
	SDL_RenderFillRectF(SdlUtils::renderer, &pauseContainer);
	pauseTitleText.draw();

	for (int i = 0; i < PAUSE_NUM_OPTIONS; ++i) {
		if (pauseOptions[i].selected) {
			pauseOptions[i].buttonText.setText(font, pauseOptions[i].label, BUTTON_SELECTED);
			uiSelector.x = WindowManager::GetWindowWidth() / 2.0f - pauseLabelLargest / 2.0f - uiSelector.w - POINTER_OFFSET;
			uiSelector.y = pauseOptions[i].buttonText.dstR.y;
			SDL_RenderCopyF(SdlUtils::renderer, uiSelectedT, 0, &uiSelector);
		}
		else {
			pauseOptions[i].buttonText.setText(font, pauseOptions[i].label, BUTTON_UNSELECTED);
		}

		pauseOptions[i].buttonText.draw();
	}
}

void PauseInit(TTF_Font* titleFont, TTF_Font* font) {
	// Setup background and title
	pauseContainer.w = WindowManager::GetWindowWidth() / 2.0f + 75;
	pauseContainer.h = WindowManager::GetWindowHeight();
	pauseContainer.x = WindowManager::GetWindowWidth() / 2.0f - pauseContainer.w / 2.0f;
	pauseContainer.y = 0;

	pauseTitleText.dstR.w = pauseContainer.w - 100;
	pauseTitleText.dstR.h = 100;
	pauseTitleText.dstR.x = WindowManager::GetWindowWidth() / 2.0f - pauseTitleText.dstR.w / 2.0f;
	pauseTitleText.dstR.y = pauseTitleText.dstR.h / 2.0f;
	pauseTitleText.setText(titleFont, "Paused", TITLE_COLOR);

	pauseLabelLargest = 0.0f;
	// Setup buttons
	for (int i = 0; i < PAUSE_NUM_OPTIONS; ++i) {
		pauseOptions[i].label = pauseLabels[i];
		pauseOptions[i].menuType = pauseMenuTypes[i];
		pauseOptions[i].selected = false;
		pauseOptions[i].buttonText.dstR.w = strlen(pauseOptions[i].label.c_str()) * LETTER_WIDTH;
		pauseOptions[i].buttonText.dstR.h = 50;
		CalculateButtonPosition(&pauseOptions[i].buttonText.dstR, i, PAUSE_NUM_OPTIONS, WindowManager::GetWindowWidth(), WindowManager::GetWindowHeight(), PAUSE_MENU_BUTTON_PADDING);
		pauseOptions[i].buttonText.dstR.y += pauseTitleText.dstR.h;
		pauseOptions[i].buttonText.setText(font, pauseOptions[i].label, BUTTON_UNSELECTED);

		pauseLabelLargest = std::max(pauseLabelLargest, pauseOptions[i].buttonText.dstR.w);
		;
	}
}

void RenderGameOverMenu(TTF_Font* font) {
	SDL_SetRenderDrawColor(SdlUtils::renderer, 60, 60, 60, 150);
	SDL_RenderFillRectF(SdlUtils::renderer, &gameOverContainer);
	gameOverTitleText.draw();
	if (!scoreAdded)
		CheckAndAddHighScore(std::stoi(killPointsText.text));
	else {
		SDL_StopTextInput();
		highScoreTextUI.setText(robotoF, "HIGH SCORE ADDED!", { 255, 255, 255 });
		highScoreTextUI.dstR.w = std::string("HIGH SCORE ADDED!").length() * LETTER_WIDTH;
		highScoreTextUI.dstR.x = WindowManager::GetWindowWidth() / 2.0f - highScoreTextUI.dstR.w / 2.0f;
		highScoreTextUI.draw();
	}

	for (int i = 0; i < GAMEOVER_NUM_OPTIONS; ++i) {
		if (gameOverOptions[i].selected) {
			gameOverOptions[i].buttonText.setText(font, gameOverOptions[i].label, BUTTON_SELECTED);
			uiSelector.x = WindowManager::GetWindowWidth() / 2.0f - gameOverLargest / 2.0f - uiSelector.w - POINTER_OFFSET;
			uiSelector.y = gameOverOptions[i].buttonText.dstR.y;
			SDL_RenderCopyF(SdlUtils::renderer, uiSelectedT, 0, &uiSelector);
		}
		else {
			gameOverOptions[i].buttonText.setText(font, gameOverOptions[i].label, BUTTON_UNSELECTED);
		}

		gameOverOptions[i].buttonText.draw();
	}
}

void GameOverInit(TTF_Font* titleFont, TTF_Font* font) {
	// Setup background and title
	gameOverContainer.w = WindowManager::GetWindowWidth();
	gameOverContainer.h = WindowManager::GetWindowHeight();
	gameOverContainer.x = 0;
	gameOverContainer.y = 0;

	gameOverTitleText.dstR.w = gameOverContainer.w - 300;
	gameOverTitleText.dstR.h = 100;
	gameOverTitleText.dstR.x = WindowManager::GetWindowWidth() / 2.0f - gameOverTitleText.dstR.w / 2.0f;
	gameOverTitleText.dstR.y = gameOverTitleText.dstR.h / 2.0f + 100;
	gameOverTitleText.setText(titleFont, "GAME OVER", TITLE_COLOR);

	gameOverLargest = 0.0f;
	// Setup buttons
	for (int i = 0; i < GAMEOVER_NUM_OPTIONS; ++i) {
		gameOverOptions[i].label = gameOverLabels[i];
		gameOverOptions[i].menuType = gameOverMenuTypes[i];
		gameOverOptions[i].selected = false;
		gameOverOptions[i].buttonText.dstR.w = strlen(gameOverOptions[i].label.c_str()) * LETTER_WIDTH;
		gameOverOptions[i].buttonText.dstR.h = 50;
		CalculateButtonPosition(&gameOverOptions[i].buttonText.dstR, i, GAMEOVER_NUM_OPTIONS, WindowManager::GetWindowWidth(), WindowManager::GetWindowHeight(), GAMEOVER_MENU_BUTTON_PADDING);
		gameOverOptions[i].buttonText.dstR.y += gameOverTitleText.dstR.h;
		gameOverOptions[i].buttonText.setText(font, gameOverOptions[i].label, BUTTON_UNSELECTED);

		gameOverLargest = std::max(gameOverLargest, gameOverOptions[i].buttonText.dstR.w);;
	}    
}

void RenderControlsMenu(TTF_Font* font) {
	SDL_RenderCopyF(SdlUtils::renderer, controlsT, 0, 0);
	SDL_SetRenderDrawColor(SdlUtils::renderer, 60, 60, 60, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRectF(SdlUtils::renderer, &controlsTitleContainer);  

	SDL_SetRenderDrawColor(SdlUtils::renderer, 60, 60, 60, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRectF(SdlUtils::renderer, &controlsOptionContainer);
	
	controlsTitleText.draw();

	if (controlsOptions.selected) {
		controlsOptions.buttonText.setText(font, controlsOptions.label, BUTTON_SELECTED);
	}
	else {
		controlsOptions.buttonText.setText(font, controlsOptions.label, BUTTON_UNSELECTED);
	}

	controlsOptions.buttonText.draw();
}

void RenderCreditsMenu(TTF_Font* font) {
	Text authorsText;
	authorsText.dstR.w = std::string("Game authors:").length() * LETTER_WIDTH;
	authorsText.dstR.h = 50;
	authorsText.dstR.x = WindowManager::GetWindowWidth() / 2 - authorsText.dstR.w / 2;
	authorsText.dstR.y = 0;
	authorsText.setText(moonhouseF, "Game authors:", { 255, 0, 0 });
	authorsText.draw();
	std::vector<Text> authors;
	authors.push_back(Text());
	authors.back().setText(robotoF, "Huberti");
	authors.back().dstR.w = 100;
	authors.back().dstR.h = 50;
	authors.back().dstR.x = WindowManager::GetWindowWidth() / 2 - authors.back().dstR.w / 2;
	authors.back().dstR.y = authorsText.dstR.y + authorsText.dstR.h;
	authors.push_back(authors.back());
	authors.back().setText(robotoF, "Kyn21kx");
	authors.back().dstR.y = authors[authors.size() - 2].dstR.y + authors[authors.size() - 2].dstR.h;
	authors.push_back(authors.back());
	authors.back().setText(robotoF, "ajyang2000");
	authors.back().dstR.y = authors[authors.size() - 2].dstR.y + authors[authors.size() - 2].dstR.h;
	authors.push_back(authors.back());
	authors.back().setText(robotoF, "Altimerra");
	authors.back().dstR.y = authors[authors.size() - 2].dstR.y + authors[authors.size() - 2].dstR.h;
	authors.push_back(authors.back());
	authors.back().setText(robotoF, "MalgorzataMika");
	authors.back().dstR.w = 140;
	authors.back().dstR.y = authors[authors.size() - 2].dstR.y + authors[authors.size() - 2].dstR.h;
	Text externalGraphicsText;
	externalGraphicsText.setText(moonhouseF, "External graphics:", { 0, 255, 0 });
	externalGraphicsText.dstR.w = std::string("External graphics:").length() * LETTER_WIDTH;
	externalGraphicsText.dstR.h = 50;
	externalGraphicsText.dstR.x = WindowManager::GetWindowWidth() / 2 - externalGraphicsText.dstR.w / 2;
	externalGraphicsText.dstR.y = authors.back().dstR.y + authors.back().dstR.h;
	externalGraphicsText.draw();
	std::vector<Text> egAuthorsTexts;
	egAuthorsTexts.push_back(Text());
	egAuthorsTexts.back().setText(robotoF, "Gumichan01");
	egAuthorsTexts.back().dstR.w = 100;
	egAuthorsTexts.back().dstR.h = 40;
	egAuthorsTexts.back().dstR.x = WindowManager::GetWindowWidth() / 2 - egAuthorsTexts.back().dstR.w / 2;
	egAuthorsTexts.back().dstR.y = externalGraphicsText.dstR.y + externalGraphicsText.dstR.h;
	egAuthorsTexts.push_back(egAuthorsTexts.back());
	egAuthorsTexts.back().setText(robotoF, "Skorpio");
	egAuthorsTexts.back().dstR.y = egAuthorsTexts[egAuthorsTexts.size() - 2].dstR.y + egAuthorsTexts[egAuthorsTexts.size() - 2].dstR.h;
	egAuthorsTexts.push_back(egAuthorsTexts.back());
	egAuthorsTexts.back().setText(robotoF, "Freepik");
	egAuthorsTexts.back().dstR.y = egAuthorsTexts[egAuthorsTexts.size() - 2].dstR.y + egAuthorsTexts[egAuthorsTexts.size() - 2].dstR.h;
	for (int i = 0; i < authors.size(); ++i) {
		authors[i].draw();
	}
	for (int i = 0; i < egAuthorsTexts.size(); ++i) {
		egAuthorsTexts[i].draw();
	}

	SDL_SetRenderDrawColor(SdlUtils::renderer, 60, 60, 60, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRectF(SdlUtils::renderer, &controlsOptionContainer);

	if (controlsOptions.selected) {
		controlsOptions.buttonText.setText(font, controlsOptions.label, BUTTON_SELECTED);
	}
	else {
		controlsOptions.buttonText.setText(font, controlsOptions.label, BUTTON_UNSELECTED);
	}

	controlsOptions.buttonText.draw();
}

void ControlsInit(TTF_Font* titleFont, TTF_Font* font) {
	// Setup background and title
	controlsTitleContainer.w = 500;
	controlsTitleContainer.h = 100;
	controlsTitleContainer.x = WindowManager::GetWindowWidth() / 2.0f - controlsTitleContainer.w / 2.0f;
	controlsTitleContainer.y = 0;

	controlsOptionContainer.w = strlen(controlsLabel.c_str()) * LETTER_WIDTH + 50;
	controlsOptionContainer.h = 75;
	controlsOptionContainer.x = 20;
	controlsOptionContainer.y = WindowManager::GetWindowHeight() - controlsOptionContainer.h - 20;

	controlsTitleText.dstR.w = 450;
	controlsTitleText.dstR.h = 75;
	controlsTitleText.dstR.x = WindowManager::GetWindowWidth() / 2.0f - controlsTitleText.dstR.w / 2.0f;
	controlsTitleText.dstR.y = controlsTitleContainer.h / 2.0f - controlsTitleText.dstR.h / 2.0f;
	controlsTitleText.setText(titleFont, "Controls", TITLE_COLOR);

	controlsOptions.label = controlsLabel;
	controlsOptions.menuType = controlsMenuType;
	controlsOptions.selected = false;
	controlsOptions.buttonText.dstR.w = strlen(controlsOptions.label.c_str()) * LETTER_WIDTH;
	controlsOptions.buttonText.dstR.h = 50;
	controlsOptions.buttonText.dstR.x = controlsOptionContainer.x + controlsOptionContainer.w / 2.0f - controlsOptions.buttonText.dstR.w / 2.0f;
	controlsOptions.buttonText.dstR.y = controlsOptionContainer.y + controlsOptionContainer.h / 2.0f - controlsOptions.buttonText.dstR.h / 2.0f;
	controlsOptions.buttonText.setText(font, controlsOptions.label, BUTTON_UNSELECTED);

}

void RenderHighScoresMenu(TTF_Font* titleFont, TTF_Font* font) {
	SDL_RenderCopyF(SdlUtils::renderer, highScoreT, 0, 0);

	highscoresTitleContainer.w = 500;
	highscoresTitleContainer.h = 100;
	highscoresTitleContainer.x = WindowManager::GetWindowWidth() / 2.0f - controlsTitleContainer.w / 2.0f;
	highscoresTitleContainer.y = 0;

	highscoresTitleText.dstR.w = 450;
	highscoresTitleText.dstR.h = 75;
	highscoresTitleText.dstR.x = WindowManager::GetWindowWidth() / 2.0f - controlsTitleText.dstR.w / 2.0f;
	highscoresTitleText.dstR.y = controlsTitleContainer.h / 2.0f - controlsTitleText.dstR.h / 2.0f;
	highscoresTitleText.setText(titleFont, "HIGHSCORES", TITLE_COLOR);
	highscoresTitleText.draw();

	SDL_SetRenderDrawColor(SdlUtils::renderer, 60, 60, 60, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRectF(SdlUtils::renderer, &controlsOptionContainer);

	if (controlsOptions.selected) {
		controlsOptions.buttonText.setText(font, controlsOptions.label, BUTTON_SELECTED);
	}
	else {
		controlsOptions.buttonText.setText(font, controlsOptions.label, BUTTON_UNSELECTED);
	}

	controlsOptions.buttonText.draw();

	//TODO: Add actual highscores.
	HighScores::ReadScores(allScores);

	for (int i = 0; i < HIGH_SCORES_LIMIT; ++i) {
		int nScore = std::get<0>(allScores[i]);
		if (nScore < 0) break;
		std::string name = std::get<1>(allScores[i]);

		Text score;
		score.dstR.h = 100;
		score.dstR.w = strlen(std::to_string(nScore).c_str()) * LETTER_WIDTH;
		Text nameText;
		nameText.dstR.h = 100;
		nameText.dstR.w = strlen(name.c_str()) * LETTER_WIDTH;

		CalculateHighScorePosition(&nameText.dstR, &score.dstR, i);
		nameText.setText(robotoF, name, { 255, 255, 255 });
		nameText.draw();

		score.setText(robotoF, std::to_string(nScore), { 255, 255, 255 });
		score.draw();
	}
}

void HandleMenuOption(MenuOption option)
{
	switch (option) {
		case MenuOption::Play:
			gameRunning = true;
			audioManager->PlayMusic(MusicAudio::BattleMusic);
			state = State::Gameplay;
			break;
		case MenuOption::Credits:
			gameRunning = true;
			state = State::Credits;
			break;
		case MenuOption::Restart:
			restart = true;
			gameRunning = false;
			audioManager->StopMusic();
			audioManager->PlayMusic(MusicAudio::BattleMusic);
			break;
		case MenuOption::Resume:
			pausing = false;
			pauseKeyHeld = false;
			audioManager->ResumeMusic();
			break;
		case MenuOption::Controls:
			gameRunning = true;
			state = State::Controls;
			break;
		case MenuOption::Highscores:
			gameRunning = true;
			state = State::HighScores;
			break;
		case MenuOption::Main:
			gameRunning = false;
			restart = false;
			
			if (audioManager->IsPlaying() != (int)MusicAudio::UIMusic)
				audioManager->PlayMusic(MusicAudio::UIMusic);
			
			state = State::Main;
			break;
		case MenuOption::Quit:
			gameRunning = false;
			appRunning = false;
			audioManager->StopMusic();
			audioManager->Release();
			break;
	}
}

void CheckAndAddHighScore(int score) {
	if (!addingScore && highScoreInputName.length() < 1) {
		//Get the array of scores
		HighScores::ReadScores(allScores);
		for (size_t i = 0; i < HIGH_SCORES_LIMIT; i++) {
			int indexedScore = std::get<0>(allScores[i]);
			//Check if the score is bigger than the current one
			if (score > indexedScore) {
				//Ask for the name of the player, and set it to a new tuple with the score
				addingScore = true;
				indexToInsertHighScore = i;
				break;
			}
		}
	}
	else {
		SDL_StartTextInput();
		std::string txt = "New highscore, Enter your name (6 Characters Max): ";
		highScoreTextUI.setText(robotoF, txt, { 255, 255, 255 });
		highScoreTextUI.draw();
		highscoresNameText.dstR.w = highScoreInputName.length() * LETTER_WIDTH;
		highscoresNameText.dstR.x = WindowManager::GetWindowWidth() / 2.0f - highscoresNameText.dstR.w / 2.0f;
		highscoresNameText.setText(robotoF, highScoreInputName, { 255, 255, 255 });
		highscoresNameText.draw();
	}


}
#pragma endregion

void ScoreKill() {
	killPointsText.setText(robotoF, std::stoi(killPointsText.text) + 1);
	if (std::stoi(killPointsText.text) % 100 == 0) {
		moneyText.setText(robotoF, std::stoi(moneyText.text) + 30);
	}
	moneyText.setText(robotoF, std::stoi(moneyText.text) + 1);
}

void ShiftScores(const std::tuple<int, std::string>& toInsert) {
	//Create a temporary tuple holding the previous value of the tuple
	std::tuple<int, std::string> prev = allScores[indexToInsertHighScore];
	//Replace the content at the index
	allScores[indexToInsertHighScore] = toInsert;
	//Shift the array
	for (size_t i = indexToInsertHighScore + 1; i < HIGH_SCORES_LIMIT; i++) {
		if (i == HIGH_SCORES_LIMIT - 1) break;
		allScores[i] = prev;
		prev = allScores[i + 1];
	}
}

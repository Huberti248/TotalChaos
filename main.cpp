#include "SdlDefinitions.h"
#include "AudioManager.h"

enum class State {
    Gameplay,
    Credits,
};

Player player;
SDL_Texture* playerT;
SDL_Texture* bgT;
SDL_Texture* bulletT;
SDL_Texture* redBulletT;
SDL_Texture* purpleBulletT;
SDL_Texture* enemyT;
SDL_Texture* planetT;
SDL_Texture* meatT;
SDL_Texture* explosionT;
SDL_Texture* buyT;
SDL_Texture* shieldT;
SDL_Texture* closeT;
SDL_Texture* coinsT;
SDL_Texture* uiSelectedT;
SDL_Texture* portalT;
SDL_Texture* weaponPlanetT;
SDL_Texture* shotgunT;
SDL_Texture* controlsT;
Clock globalClock;
std::vector<Bullet> bullets;
Clock bulletClock;
Clock enemyClock;
std::vector<Enemy> enemies;
Text killPointsText;
Text healthText;
std::vector<Entity> planets;
Clock planetClock;
std::vector<Entity> healthPickups;
Clock healthPickupClock;
std::vector<Entity> bombs;
Clock bombClock;
bool buyingShield = false;
Text shieldPriceText;
Text shieldText;
Text shieldHealthText;
SDL_FRect buyBtnR;
SDL_FRect buyR;
SDL_FRect buyShieldR;
SDL_FRect moneyR;
SDL_FRect shieldPrizeCoinsR;
Text moneyText;
bool hasShield = false;
int shieldHealth = 0;
SDL_FRect closeBtnR;
int healthSpawnTime = 0;
std::vector<SDL_FRect> portalRects;
Clock portalClock;
bool menuRunning = false;
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
bool controlsMenu = false;
float playerHealth;
AudioManager* audioManager = AudioManager::Instance();
Text shotgunPriceText;
Text shotgunText;
SDL_FRect buyShotgunR;
SDL_FRect buyShotgunBtnR;
bool buyingShotgun = false;
bool hasShotgun = false;
State state = State::Gameplay;

void WindowInit();

void GlobalsInit();

void mainLoop();

void menuMainLoop();

void TexturesInit();

void UiInit();

void ClocksInit();

void BounceOff(Entity* a, Entity* b, bool affectB);

void InputEvents(const SDL_Event& event);

void EnemySpawn();

void EntityMovement(const SDL_FRect& extendedWindowR, float deltaTime);

void BulletCollisions(const SDL_FRect& extendedWindowR, float deltaTime);

void EnemyBehavior(const SDL_FRect& extendedWindowR, float deltaTime);

void PowerUpSpawner();

void RenderAll();

void FireWhenReady();

MenuOption DisplayMainMenu(TTF_Font* titleFont, TTF_Font* font);

void RenderPauseMenu(TTF_Font* font);

void PauseInit(TTF_Font* titleFont, TTF_Font* font);

void RenderGameOverMenu(TTF_Font* font);

void GameOverInit(TTF_Font* titleFont, TTF_Font* font);

void RenderControlsMenu(TTF_Font* font);

void ControlsInit(TTF_Font* titleFont, TTF_Font* font);

void HandleMenuOption(MenuOption option);

float clamp(float n, float lower, float upper)
{
    return std::max(lower, std::min(n, upper));
}

void mainLoop()
{
    float deltaTime = globalClock.restart();
    SDL_Event event;

    SDL_FRect extendedWindowR;
    extendedWindowR.w = windowWidth + 5;
    extendedWindowR.h = windowHeight + 5;
    extendedWindowR.x = -5;
    extendedWindowR.y = -5;

    if (state == State::Gameplay) {
        while (SDL_PollEvent(&event)) {
            InputEvents(event);
        }
        if (!buyingShield && !buyingShotgun && !pausing && !gameOver) {
            player.dx = 0;
            player.dy = 0;
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
                    enemies.erase(enemies.begin() + i--); //Destroy the enemy if it is within the radius
                    audioManager->PlaySFX(SFXAudio::EnemyDeath);
                }

                //Reset the player's streak
                //TODO: Make this a function on the getters and setters of hasBomb
                player.streak = 0;
                player.hasBomb = false;
            }

            EnemySpawn();

            EntityMovement(extendedWindowR, deltaTime);

            BulletCollisions(extendedWindowR, deltaTime);

            EnemyBehavior(extendedWindowR, deltaTime);

            PowerUpSpawner();
            //To Review (more than one portal at a time)
            if (portalClock.getElapsedTime() > PORTAL_SPAWN_DELAY_IN_MS && portalRects.size() < 1) {
                portalRects.push_back(SDL_FRect());
                portalRects.back().w = 32;
                portalRects.back().h = 32;
                portalRects.back().x = random(0, windowWidth - portalRects.back().w);
                portalRects.back().y = random(0, windowHeight - portalRects.back().h);
                portalRects.push_back(SDL_FRect());
                portalRects.back().w = 32;
                portalRects.back().h = 32;
                portalRects.back().x = random(0, windowWidth - portalRects.back().w);
                portalRects.back().y = random(0, windowHeight - portalRects.back().h);
                portalClock.restart();
            }
            for (int i = 0; i < portalRects.size(); ++i) {
                if (SDL_HasIntersectionF(&player.r, &portalRects[i])) {
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
            gameOver = true;
            pauseMosPos = mousePos;
        }
        RenderAll();
    }
    else if (state == State::Credits) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                gameRunning = false;
                // TODO: On mobile remember to use eventWatch function (it doesn't reach this code when terminating)
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_RenderSetScale(renderer, event.window.data1 / (float)windowWidth, event.window.data2 / (float)windowHeight);
            }
            if (event.type == SDL_KEYDOWN) {
                keys[event.key.keysym.scancode] = true;
            }
            if (event.type == SDL_KEYUP) {
                keys[event.key.keysym.scancode] = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                buttons[event.button.button] = true;
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                buttons[event.button.button] = false;
            }
            if (event.type == SDL_MOUSEMOTION) {
                float scaleX, scaleY;
                SDL_RenderGetScale(renderer, &scaleX, &scaleY);
                mousePos.x = event.motion.x / scaleX;
                mousePos.y = event.motion.y / scaleY;
                realMousePos.x = event.motion.x;
                realMousePos.y = event.motion.y;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        Text authorsText;
        authorsText.dstR.w = 140;
        authorsText.dstR.h = 40;
        authorsText.dstR.x = windowWidth / 2 - authorsText.dstR.w / 2;
        authorsText.dstR.y = 0;
        authorsText.setText(renderer, robotoF, "Game authors:");
        authorsText.draw(renderer);
        std::vector<Text> authors;
        authors.push_back(Text());
        authors.back().setText(renderer, robotoF, "Huberti");
        authors.back().dstR.w = 100;
        authors.back().dstR.h = 40;
        authors.back().dstR.x = windowWidth / 2 - authors.back().dstR.w / 2;
        authors.back().dstR.y = authorsText.dstR.y + authorsText.dstR.h;
        authors.push_back(authors.back());
        authors.back().setText(renderer, robotoF, "Kyn21kx");
        authors.back().dstR.y = authors[authors.size() - 2].dstR.y + authors[authors.size() - 2].dstR.h;
        authors.push_back(authors.back());
        authors.back().setText(renderer, robotoF, "ajyang2000");
        authors.back().dstR.y = authors[authors.size() - 2].dstR.y + authors[authors.size() - 2].dstR.h;
        authors.push_back(authors.back());
        authors.back().setText(renderer, robotoF, "Altimerra");
        authors.back().dstR.y = authors[authors.size() - 2].dstR.y + authors[authors.size() - 2].dstR.h;
        authors.push_back(authors.back());
        authors.back().setText(renderer, robotoF, "MalgorzataMika");
        authors.back().dstR.w = 140;
        authors.back().dstR.y = authors[authors.size() - 2].dstR.y + authors[authors.size() - 2].dstR.h;
        Text externalGraphicsText;
        externalGraphicsText.setText(renderer, robotoF, "External graphics:");
        externalGraphicsText.dstR.w = 140;
        externalGraphicsText.dstR.h = 40;
        externalGraphicsText.dstR.x = windowWidth / 2 - externalGraphicsText.dstR.w / 2;
        externalGraphicsText.dstR.y = authors.back().dstR.y + authors.back().dstR.h;
        externalGraphicsText.draw(renderer);
        std::vector<Text> egAuthorsTexts;
        egAuthorsTexts.push_back(Text());
        egAuthorsTexts.back().setText(renderer, robotoF, "Gumichan01");
        egAuthorsTexts.back().dstR.w = 100;
        egAuthorsTexts.back().dstR.h = 40;
        egAuthorsTexts.back().dstR.x = windowWidth / 2 - egAuthorsTexts.back().dstR.w / 2;
        egAuthorsTexts.back().dstR.y = externalGraphicsText.dstR.y + externalGraphicsText.dstR.h;
        egAuthorsTexts.push_back(egAuthorsTexts.back());
        egAuthorsTexts.back().setText(renderer, robotoF, "Skorpio");
        egAuthorsTexts.back().dstR.y = egAuthorsTexts[egAuthorsTexts.size() - 2].dstR.y + egAuthorsTexts[egAuthorsTexts.size() - 2].dstR.h;
        egAuthorsTexts.push_back(egAuthorsTexts.back());
        egAuthorsTexts.back().setText(renderer, robotoF, "Freepik");
        egAuthorsTexts.back().dstR.y = egAuthorsTexts[egAuthorsTexts.size() - 2].dstR.y + egAuthorsTexts[egAuthorsTexts.size() - 2].dstR.h;
        for (int i = 0; i < authors.size(); ++i) {
            authors[i].draw(renderer);
        }
        for (int i = 0; i < egAuthorsTexts.size(); ++i) {
            egAuthorsTexts[i].draw(renderer);
        }
        SDL_RenderPresent(renderer);
    }
}

void GlobalsInit()
{
    player.dx = 0;
    player.dy = 0;
    player.ghostBullet = false;
    player.hasBomb = false;
    player.streak = 0;
    player.r.x = windowWidth / 2 - player.r.w / 2;
    player.r.y = windowHeight / 2 - player.r.h / 2;
    player.SetHealth(playerHealth);

    bullets.clear();
    enemies.clear();
    killPointsText.setText(renderer, robotoF, "0");
    healthText.setText(renderer, robotoF, playerHealth, { 255, 0, 0 });
    planets.clear();
    healthPickups.clear();
    bombs.clear();
    buyingShield = false;
    buyingShotgun = false;

    std::string sText = "Shield: " + std::to_string(shieldHealth);
    shieldHealthText.setText(renderer, robotoF, sText, { 255, 0, 0 });

    moneyText.setText(renderer, robotoF, 0);
    hasShield = false;
    healthSpawnTime = 0;
    portalRects.clear();
    pausing = false;
    pauseKeyHeld = false;
    gameOver = false;
    restart = false;
}

int main(int argc, char* argv[])
{
    WindowInit();
    TexturesInit();

    player.r.w = 64;
    player.r.h = 64;
    playerHealth = player.GetHealth();

    uiSelector.w = 32;
    uiSelector.h = 32;

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

        if (menuRunning) {
            while (menuRunning) {
                menuMainLoop();
            }
        }
        else {
            GlobalsInit();
            ClocksInit();

    while (gameRunning) {
        mainLoop();
    }
    }
}
#endif
    // TODO: On mobile remember to use eventWatch function (it doesn't reach this code when terminating)
    return 0;
}

void TexturesInit()
{
    playerT = IMG_LoadTexture(renderer, "res/player.png");
    bgT = IMG_LoadTexture(renderer, "res/bg.png");
    bulletT = IMG_LoadTexture(renderer, "res/bullet.png");
    redBulletT = IMG_LoadTexture(renderer, "res/bulletRed.png");
    purpleBulletT = IMG_LoadTexture(renderer, "res/bulletPurple.png");
    enemyT = IMG_LoadTexture(renderer, "res/enemy.png");
    planetT = IMG_LoadTexture(renderer, "res/planet.png");
    meatT = IMG_LoadTexture(renderer, "res/meat.png");
#ifdef _DEBUG //Debug define guard on purpose to generate an error if we don't replace this
    //TODO: REPLCE THIS, AS IT IS ONLY A PLACEHOLDER!!!!
    explosionT = IMG_LoadTexture(renderer, "res/gun.png");
#endif
    portalT = IMG_LoadTexture(renderer, "res/portal.png");
    buyT = IMG_LoadTexture(renderer, "res/buy.png");
    shieldT = IMG_LoadTexture(renderer, "res/shield.png");
    closeT = IMG_LoadTexture(renderer, "res/close.png");
    coinsT = IMG_LoadTexture(renderer, "res/coins.png");
    uiSelectedT = IMG_LoadTexture(renderer, "res/player.png");
    weaponPlanetT = IMG_LoadTexture(renderer, "res/weaponPlanet.png");
    shotgunT = IMG_LoadTexture(renderer, "res/shotgun.png");
    controlsT = IMG_LoadTexture(renderer, "res/controlsMenu.png");
}

void UiInit()
{
    buyR.w = 200;
    buyR.h = 200;
    buyR.x = windowWidth / 2 - buyR.w / 2;
    buyR.y = windowHeight / 2 - buyR.h / 2;

    killPointsText.dstR.w = 30;
    killPointsText.dstR.h = 20;
    killPointsText.dstR.x = windowWidth / 2 - killPointsText.dstR.w / 2;
    killPointsText.dstR.y = 0;
    killPointsText.setText(renderer, robotoF, "0");

    healthText.dstR.w = 30;
    healthText.dstR.h = 20;
    healthText.dstR.x = windowWidth - healthText.dstR.w;
    healthText.dstR.y = 0;
    healthText.setText(renderer, robotoF, player.GetHealth(), { 255, 0, 0 });

    shieldHealthText.dstR.w = 50;
    shieldHealthText.dstR.h = 30;
    shieldHealthText.dstR.x = windowWidth / 3 - shieldPriceText.dstR.w / 3;
    shieldHealthText.dstR.y = 0;
    std::string sText = "Shield: " + std::to_string(shieldHealth);
    shieldHealthText.setText(renderer, robotoF, sText, { 255, 255, 255 });

    shieldPriceText.setText(renderer, robotoF, "50");
    shieldPriceText.dstR.w = 100;
    shieldPriceText.dstR.h = 40;
    shieldPriceText.dstR.x = windowWidth / 2 - shieldPriceText.dstR.w / 2;
    shieldPriceText.dstR.y = buyR.y + 10;

    buyShieldR.w = 32;
    buyShieldR.h = 32;
    buyShieldR.x = shieldPriceText.dstR.x + shieldPriceText.dstR.w / 2 - buyShieldR.w / 2;
    buyShieldR.y = shieldPriceText.dstR.y + shieldPriceText.dstR.h;

    shieldText.setText(renderer, robotoF, "Shield");
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
    moneyR.x = windowWidth - moneyR.w;
    moneyR.y = healthText.dstR.y + healthText.dstR.h;

    moneyText.setText(renderer, robotoF, 0);
    moneyText.dstR.w = 30;
    moneyText.dstR.h = 20;
    moneyText.dstR.x = moneyR.x - moneyText.dstR.w;
    moneyText.dstR.y = healthText.dstR.y + healthText.dstR.h;

    shieldPrizeCoinsR.w = 32;
    shieldPrizeCoinsR.h = 32;
    shieldPrizeCoinsR.x = shieldPriceText.dstR.x + shieldPriceText.dstR.w;
    shieldPrizeCoinsR.y = shieldPriceText.dstR.y;

    shotgunPriceText = shieldPriceText;
    shotgunPriceText.setText(renderer, robotoF, 100);

    shotgunText = shieldText;
    shotgunText.setText(renderer, robotoF, "Shotgun");

    buyShotgunR = buyShotgunBtnR;

    buyShotgunBtnR = buyBtnR;
}

void ClocksInit()
{
    globalClock.restart();
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
    window = SDL_CreateWindow("TotalChaos", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    robotoF = TTF_OpenFont("res/roboto.ttf", 72);
    moonhouseF = TTF_OpenFont("res/moonhouse.ttf", 72);
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_RenderSetScale(renderer, w / (float)windowWidth, h / (float)windowHeight);
    SDL_AddEventWatch(eventWatch, 0);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void BounceOff(Entity* a, Entity* b, bool affectB)
{
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
    if (!affectB)
        return;
    b->dx = directionB.x;
    b->dy = directionB.y;
}

void InputEvents(const SDL_Event& event)
{
    if (event.type == SDL_QUIT) {
        gameRunning = false;
        appRunning = false;
        // TODO: On mobile remember to use eventWatch function (it doesn't reach this code when terminating)
    }
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
        SDL_RenderSetScale(renderer, event.window.data1 / (float)windowWidth, event.window.data2 / (float)windowHeight);
    }
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && !pauseKeyHeld && !gameOver) {
            pausing = !pausing;
            pauseKeyHeld = true;
            pauseMosPos = mousePos;

            pausing ? audioManager->PauseMusic() : audioManager->ResumeMusic();
        }
        keys[event.key.keysym.scancode] = true;
    }
    if (event.type == SDL_KEYUP) {
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            pauseKeyHeld = false;
        }
        keys[event.key.keysym.scancode] = false;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        buttons[event.button.button] = true;
        if (gameOver) {
            buttons[event.button.button] = false;
            // Game is over, so you can only interact with the menu on button clicks
            for (int i = 0; i < PAUSE_NUM_OPTIONS; ++i) {
                if (SDL_PointInFRect(&mousePos, &gameOverOptions[i].buttonText.dstR)) {
                    audioManager->PlaySFX(SFXAudio::UI);
                    HandleMenuOption(gameOverOptions[i].menuType);
                }
            }
        }
        else if (pausing) {
            buttons[event.button.button] = false;
            // Game is paused, so you can only interact with the menu on button clicks
            for (int i = 0; i < PAUSE_NUM_OPTIONS; ++i) {
                if (SDL_PointInFRect(&mousePos, &pauseOptions[i].buttonText.dstR)) {
                    audioManager->PlaySFX(SFXAudio::UI);
                    HandleMenuOption(pauseOptions[i].menuType);
                }
            }
        }
        else {
            if (SDL_PointInFRect(&mousePos, &closeBtnR) && (buyingShield || buyingShotgun)) {
                buyingShield = false;
                buyingShotgun = false;
            }
            if (SDL_PointInFRect(&mousePos, &buyBtnR) && buyingShield) {
                if (std::stoi(moneyText.text) >= std::stoi(shieldPriceText.text)) {
                    hasShield = true;
                    shieldHealth = 10;
#if _DEBUG
                    std::string sText = "Shield: " + std::to_string(shieldHealth);
                    shieldHealthText.setText(renderer, robotoF, sText, { 255, 0, 0 });
#endif
                    moneyText.setText(renderer, robotoF, std::stoi(moneyText.text) - std::stoi(shieldPriceText.text));
                }
            }
            if (SDL_PointInFRect(&mousePos, &buyShotgunBtnR) && buyingShotgun) {
                if (std::stoi(moneyText.text) >= std::stoi(shotgunPriceText.text)) {
                    hasShotgun = true;
                    moneyText.setText(renderer, robotoF, std::stoi(moneyText.text) - std::stoi(shotgunPriceText.text));
                }
            }
        }
    }
    if (event.type == SDL_MOUSEBUTTONUP) {
        buttons[event.button.button] = false;
    }
    if (event.type == SDL_MOUSEMOTION) {
        float scaleX, scaleY;
        SDL_RenderGetScale(renderer, &scaleX, &scaleY);
        mousePos.x = event.motion.x / scaleX;
        mousePos.y = event.motion.y / scaleY;
        realMousePos.x = event.motion.x;
        realMousePos.y = event.motion.y;
        if (gameOver) {
            for (int i = 0; i < GAMEOVER_NUM_OPTIONS; ++i) {
                if (SDL_PointInFRect(&mousePos, &gameOverOptions[i].buttonText.dstR)) {
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
                if (SDL_PointInFRect(&mousePos, &pauseOptions[i].buttonText.dstR)) {
                    pauseOptions[i].selected = true;
                }
                else {
                    pauseOptions[i].selected = false;
                }
            }
        }
    }
}

void EnemySpawn()
{
    if (enemyClock.getElapsedTime() > ENEMY_SPAWN_DELAY_IN_MS) {
        //Create a random interval for the shooting between a range defined by constants
        int bulletInterval = random(MINIMUM_INTERVAL_BULLET_MS, MAXIMUM_INTERVAL_BULLET_MS);
        enemies.push_back(Enemy(bulletInterval));
        enemies.back().r.w = 32;
        enemies.back().r.h = 32;
        enemies.back().spawnPlace = (SpawnPlace)random(0, 3);

        switch (enemies.back().spawnPlace) {
        case SpawnPlace::Up:
            enemies.back().r.x = random(0, windowWidth - enemies.back().r.w);
            enemies.back().r.y = 0;
            enemies.back().dy = 1;
            break;
        case SpawnPlace::Down:
            enemies.back().r.x = random(0, windowWidth - enemies.back().r.w);
            enemies.back().r.y = windowHeight;
            enemies.back().dy = -1;
            break;
        case SpawnPlace::Left:
            enemies.back().r.x = -enemies.back().r.w;
            enemies.back().r.y = random(0, windowHeight - enemies.back().r.h);
            enemies.back().dx = 1;
            break;
        case SpawnPlace::Right:
            enemies.back().r.x = windowWidth;
            enemies.back().r.y = random(0, windowHeight - enemies.back().r.h);
            enemies.back().dx = -1;
            break;
        }
        enemyClock.restart();
    }
}

void EntityMovement(const SDL_FRect& extendedWindowR, float deltaTime)
{
    //Bullet movement
    for (int i = 0; i < bullets.size(); ++i) {
        bullets[i].r.y += bullets[i].dy * deltaTime * BULLET_SPEED;
        bullets[i].r.x += bullets[i].dx * deltaTime * BULLET_SPEED;
    }

    //Player movement
    player.r.x += player.dx * deltaTime * PLAYER_SPEED;
    player.r.y += player.dy * deltaTime * PLAYER_SPEED;
    player.r.x = clamp(player.r.x, 0.0f, windowWidth - player.r.w);
    player.r.y = clamp(player.r.y, 0.0f, windowHeight - player.r.h);

    //Enemy movement
    for (int i = 0; i < enemies.size(); ++i) {
        enemies[i].r.x += enemies[i].dx * deltaTime * ENEMY_SPEED;
        enemies[i].r.y += enemies[i].dy * deltaTime * ENEMY_SPEED;
    }

    //Planet's movement
    for (int i = 0; i < planets.size(); ++i) {
        planets[i].r.x += planets[i].dx * deltaTime * PLANET_SPEED;
        planets[i].r.y += planets[i].dy * deltaTime * PLANET_SPEED;
        if (SDL_HasIntersectionF(&player.r, &planets[i].r)) {
            if (planets[i].planetType == PlanetType::Shield) {
                buyingShield = true;
            }
            else if (planets[i].planetType == PlanetType::Weapon) {
                buyingShotgun = true;
            }
        }
        if (SDL_HasIntersectionF(&player.r, &planets[i].r)
            || !SDL_HasIntersectionF(&planets[i].r, &extendedWindowR)) {
            planets.erase(planets.begin() + i--);
        }
    }

    //Health pickup's movement
    for (int i = 0; i < healthPickups.size(); ++i) {
        healthPickups[i].r.x += healthPickups[i].dx * deltaTime * PLANET_SPEED;
        healthPickups[i].r.y += healthPickups[i].dy * deltaTime * PLANET_SPEED;
        if (SDL_HasIntersectionF(&player.r, &healthPickups[i].r)) {
            player.SetHealth(player.GetHealth() + 10);
        }
        if (SDL_HasIntersectionF(&player.r, &healthPickups[i].r)
            || !SDL_HasIntersectionF(&healthPickups[i].r, &extendedWindowR)) {
            healthPickups.erase(healthPickups.begin() + i--);
        }
    }

    //Explosion's movement
    for (size_t i = 0; i < bombs.size(); ++i) {
        bombs[i].r.x += bombs[i].dx * deltaTime * PLANET_SPEED;
        bombs[i].r.y += bombs[i].dy * deltaTime * PLANET_SPEED;
        if (SDL_HasIntersectionF(&player.r, &bombs[i].r)) {
            player.hasBomb = true;
        }
        if (SDL_HasIntersectionF(&player.r, &bombs[i].r)
            || !SDL_HasIntersectionF(&bombs[i].r, &extendedWindowR)) {
            bombs.erase(bombs.begin() + i--);
        }
    }
}

void BulletCollisions(const SDL_FRect& extendedWindowR, float deltaTime)
{
deleteCollidingBegin:
    for (int i = 0; i < bullets.size(); ++i) {
        //Player collision
        if ((bullets[i].GetTargetMask() & TargetMask::PlayerMask) != 0) {
            if (SDL_HasIntersectionF(&bullets[i].r, &player.r)) {
                if (hasShield) {
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
                        shieldHealthText.setText(renderer, robotoF, sText, { 255, 0, 0 });
                    }

                    if (shieldHealth <= 0) {
                        hasShield = false;
                    }
                    BounceOff(&bullets[i], &player, false);
                }
                else {
                    audioManager->PlaySFX(SFXAudio::PlayerHit);
                    player.SetHealth(player.GetHealth() - 1);
                    bullets.erase(bullets.begin() + i--);
                    healthText.setText(renderer, robotoF, player.GetHealth(), { 255, 0, 0 });
                    goto deleteCollidingBegin;
                }
            }
        }

        //Enemy collision
        for (int j = 0; j < enemies.size(); ++j) {
            //Layer control
            if ((bullets[i].GetTargetMask() & TargetMask::EnemiesMask) == 0)
                continue;
            if (SDL_HasIntersectionF(&bullets[i].r, &enemies[j].r)) {
                player.streak++;
                audioManager->PlaySFX(SFXAudio::EnemyDeath);
                enemies.erase(enemies.begin() + j--);
                bullets.erase(bullets.begin() + i--);
                killPointsText.setText(renderer, robotoF, std::stoi(killPointsText.text) + 1);
                if (std::stoi(killPointsText.text) % 100 == 0) {
                    moneyText.setText(renderer, robotoF, std::stoi(moneyText.text) + 30);
                }
                moneyText.setText(renderer, robotoF, std::stoi(moneyText.text) + 1);
                goto deleteCollidingBegin;
            }
        }

        if ((bullets[i].lifetime + deltaTime) > BULLET_SPLIT_DELAY_IN_MS) {
            bullets[i].Split(&bullets);
            bullets[i].lifetime = 0;
        }
        else {
            bullets[i].lifetime += deltaTime;
        }

        //Bullet collision
        if (i != (bullets.size() - 1)) {
            for (int j = i + 1; j < bullets.size(); ++j) {
                bool timePassed = (bullets[i].lifetime > BULLET_COLLISION_DELAY_IN_MS) && (bullets[j].lifetime > BULLET_COLLISION_DELAY_IN_MS);
                if (SDL_HasIntersectionF(&bullets[i].r, &bullets[j].r) && timePassed) {
                    BounceOff(&bullets[i], &bullets[j], true);
                }
            }
        }

        if (!SDL_HasIntersectionF(&bullets[i].r, &extendedWindowR)) {
            bullets.erase(bullets.begin() + i--);
        }
    }
}

void EnemyBehavior(const SDL_FRect& extendedWindowR, float deltaTime)
{
    for (int i = 0; i < enemies.size(); ++i) {
        SDL_FPoint playerPos = {
            player.r.x,
            player.r.y
        };

        enemies[i].Combat(enemyT, &bullets, playerPos, renderer);
        SDL_FRect enemyR = enemies[i].r;
        --enemyR.x;
        --enemyR.y;
        enemyR.w += 2;
        enemyR.h += 2;
        if (!SDL_HasIntersectionF(&enemies[i].r, &extendedWindowR)) {
            enemies.erase(enemies.begin() + i--);
        }
    }
}

void PowerUpSpawner()
{
    if (planetClock.getElapsedTime() > PLANET_SPAWN_DELAY_IN_MS && !hasShield) {
        planets.push_back(Entity());
        planets.back().r.w = 64;
        planets.back().r.h = 64;
        planets.back().r.x = random(0, windowWidth - planets.back().r.w);
        planets.back().r.y = -planets.back().r.h;
        planets.back().dy = 1;
        if (!hasShotgun) {
            planets.back().planetType = (PlanetType)random(0, 1);
        }
        else {
            planets.back().planetType = (PlanetType)random(0, 0);
        }
        planetClock.restart();
    }

    //Randomize the spawn time of the health
    healthSpawnTime = healthSpawnTime == 0 ? random(HEALTH_SPAWN_MIN_DELAY_IN_MS, HEALTH_SPAWN_MAX_DELAY_IN_MS) : healthSpawnTime;

    if (healthPickupClock.getElapsedTime() > healthSpawnTime) {
        healthPickups.push_back(Entity());
        healthPickups.back().r.w = 64;
        healthPickups.back().r.h = 64;
        healthPickups.back().r.x = random(0, windowWidth - healthPickups.back().r.w);
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
        bombs.back().r.x = random(0, windowWidth - bombs.back().r.w);
        bombs.back().r.y = -bombs.back().r.h;
        bombs.back().dy = 1;
    }
}

void RenderAll()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderCopyF(renderer, bgT, 0, 0);
    if (hasShield) {
        SDL_RenderCopyF(renderer, shieldT, 0, &player.r);
        shieldHealthText.draw(renderer);
    }
    if (pausing || gameOver) {
        RotateEntityTowards(playerT, player, pauseMosPos, renderer);
    }
    else {
        RotateEntityTowards(playerT, player, mousePos, renderer);
        //SDL_RenderCopyF(renderer, playerT, 0, &player.r);
    }

    //Render bullets
    for (int i = 0; i < bullets.size(); ++i) {
        switch (bullets[i].GetTargetMask()) {
        case TargetMask::PlayerMask:
            SDL_RenderCopyF(renderer, redBulletT, 0, &bullets[i].r);
            break;
        case TargetMask::EnemiesMask | TargetMask::PlayerMask:
            SDL_RenderCopyF(renderer, purpleBulletT, 0, &bullets[i].r);
            break;
        default:
            SDL_RenderCopyF(renderer, bulletT, 0, &bullets[i].r);
            break;
        }
    }

    //Render enemies
    for (int i = 0; i < enemies.size(); ++i) {
        //Based on the enemies' spawn position, rotate them to the proper angle
        double angles[] = { 180.0, 0.0, 90.0, 270.0 }; // Array of rotations mapped to the enum of spawn positions
        int index = (int)enemies[i].spawnPlace;
        SDL_RenderCopyExF(renderer, enemyT, 0, &enemies[i].r, angles[index], 0, SDL_FLIP_NONE);
    }
    //Render UI text
    killPointsText.draw(renderer);
    healthText.draw(renderer);

    //Renders planets
    for (int i = 0; i < planets.size(); ++i) {
        if (planets[i].planetType == PlanetType::Shield) {
            SDL_RenderCopyF(renderer, planetT, 0, &planets[i].r);
        }
        else if (planets[i].planetType == PlanetType::Weapon) {
            SDL_RenderCopyF(renderer, weaponPlanetT, 0, &planets[i].r);
        }
    }

    //Renders meat
    for (size_t i = 0; i < healthPickups.size(); i++) {
        SDL_RenderCopyF(renderer, meatT, 0, &healthPickups[i].r);
    }

    //Render portals
    for (int i = 0; i < portalRects.size(); ++i) {
        SDL_RenderCopyF(renderer, portalT, 0, &portalRects[i]);
    }

    //Render bomb
    for (size_t i = 0; i < bombs.size(); i++) {
        SDL_RenderCopyF(renderer, explosionT, 0, &bombs[i].r);
    }

    SDL_RenderCopyF(renderer, coinsT, 0, &moneyR);
    moneyText.draw(renderer);

    if (buyingShield) {
        SDL_SetRenderDrawColor(renderer, 125, 125, 125, 0);
        SDL_RenderFillRectF(renderer, &buyR);
        shieldPriceText.draw(renderer);
        shieldText.draw(renderer);
        SDL_RenderCopyF(renderer, buyT, 0, &buyBtnR);
        SDL_RenderCopyF(renderer, shieldT, 0, &buyShieldR);
        SDL_RenderCopyF(renderer, closeT, 0, &closeBtnR);
        SDL_RenderCopyF(renderer, coinsT, 0, &shieldPrizeCoinsR);
    }
    if (buyingShotgun) {
        SDL_SetRenderDrawColor(renderer, 125, 125, 125, 0);
        SDL_RenderFillRectF(renderer, &buyR);
        shotgunPriceText.draw(renderer);
        shotgunText.draw(renderer);
        SDL_RenderCopyF(renderer, shotgunT, 0, &buyShotgunR);
        SDL_RenderCopyF(renderer, closeT, 0, &closeBtnR);
        SDL_RenderCopyF(renderer, buyT, 0, &buyShotgunBtnR);
    }

    if (pausing) {
        RenderPauseMenu(robotoF);
    }

    if (gameOver) {
        RenderGameOverMenu(robotoF);
    }
    if (controlsMenu) {
        RenderControlsMenu(robotoF);
    }

    SDL_RenderPresent(renderer);
}

void FireWhenReady()
{
    if (bulletClock.getElapsedTime() > BULLET_SPAWN_DELAY_IN_MS) {
        audioManager->PlaySFX(SFXAudio::PlayerFire);
        //TODO: Encapsulate this in a function (and probably a different file)
        bullets.push_back(Bullet(TargetMask::EnemiesMask));
        bullets.back().r.w = 32;
        bullets.back().r.h = 32;
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

        if (hasShotgun) {
            bullets.push_back(bullets.back());
            bullets.back().dx += randomF(-0.5, 0);
            bullets.push_back(bullets.back());
            bullets.back().dx += randomF(0, 0.5);
        }

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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    Text titleText;
    titleText.dstR.w = windowWidth - 100;
    titleText.dstR.h = 100;
    titleText.dstR.x = windowWidth / 2.0f - titleText.dstR.w / 2.0f;
    titleText.dstR.y = titleText.dstR.h / 2.0f;
    titleText.setText(renderer, titleFont, "Total Chaos In Space", TITLE_COLOR);

    float largestWidth = 0;
    // Setup buttons
    for (int i = 0; i < NUMMENU; ++i) {
        buttons[i].label = labels[i];
        buttons[i].menuType = menuTypes[i];
        buttons[i].selected = false;
        buttons[i].buttonText.dstR.w = strlen(labels[i].c_str()) * LETTER_WIDTH;
        buttons[i].buttonText.dstR.h = 40;
        CalculateButtonPosition(&buttons[i].buttonText.dstR, i, NUMMENU, windowWidth, windowHeight, MAIN_MENU_BUTTON_PADDING);
        buttons[i].buttonText.dstR.y += titleText.dstR.h;
        buttons[i].buttonText.setText(renderer, font, buttons[i].label, BUTTON_UNSELECTED);
        largestWidth = std::max(buttons[i].buttonText.dstR.w, largestWidth);
    }

    // Setup pointer by selected button.
    SDL_FRect uiSelectedR;
    uiSelectedR.w = 32;
    uiSelectedR.h = 32;

    SDL_Event eventMenu;
    while (true) {
        SDL_RenderClear(renderer);
        while (SDL_PollEvent(&eventMenu)) {
            switch (eventMenu.type) {
            case SDL_QUIT:
                return MenuOption::Quit;
            case SDL_WINDOWEVENT:
                if (eventMenu.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SDL_RenderSetScale(renderer, eventMenu.window.data1 / (float)windowWidth, eventMenu.window.data2 / (float)windowHeight);
                }
                break;
            case SDL_MOUSEMOTION:
                float scaleX, scaleY;
                SDL_RenderGetScale(renderer, &scaleX, &scaleY);
                mousePos.x = eventMenu.motion.x / scaleX;
                mousePos.y = eventMenu.motion.y / scaleY;
                realMousePos.x = eventMenu.motion.x;
                realMousePos.y = eventMenu.motion.y;

                for (int i = 0; i < NUMMENU; ++i) {
                    if (SDL_PointInFRect(&mousePos, &buttons[i].buttonText.dstR)) {
                        if (!buttons[i].selected) {
                            buttons[i].selected = true;
                            buttons[i].buttonText.setText(renderer, font, buttons[i].label, BUTTON_SELECTED);
                        }
                    }
                    else {
                        if (buttons[i].selected) {
                            buttons[i].selected = false;
                            buttons[i].buttonText.setText(renderer, font, buttons[i].label, BUTTON_UNSELECTED);
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                for (int i = 0; i < NUMMENU; ++i) {
                    if (SDL_PointInFRect(&mousePos, &buttons[i].buttonText.dstR)) {
                        audioManager->PlaySFX(SFXAudio::UI);
                        return buttons[i].menuType;
                    }
                }
                break;
            }
        }

        titleText.draw(renderer);
        for (MenuButton button : buttons) {
            if (button.selected) {
                uiSelectedR.x = windowWidth / 2.0f - largestWidth / 2.0f - uiSelectedR.w - POINTER_OFFSET;
                uiSelectedR.y = button.buttonText.dstR.y;
                SDL_RenderCopyF(renderer, uiSelectedT, 0, &uiSelectedR);
            }
            button.buttonText.draw(renderer);
        }

        SDL_RenderPresent(renderer);
    }
}

void RenderPauseMenu(TTF_Font* font)
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 200);
    SDL_RenderFillRectF(renderer, &pauseContainer);
    pauseTitleText.draw(renderer);

    for (int i = 0; i < PAUSE_NUM_OPTIONS; ++i) {
        if (pauseOptions[i].selected) {
            pauseOptions[i].buttonText.setText(renderer, font, pauseOptions[i].label, BUTTON_SELECTED);
            uiSelector.x = windowWidth / 2.0f - pauseLabelLargest / 2.0f - uiSelector.w - POINTER_OFFSET;
            uiSelector.y = pauseOptions[i].buttonText.dstR.y;
            SDL_RenderCopyF(renderer, uiSelectedT, 0, &uiSelector);
        }
        else {
            pauseOptions[i].buttonText.setText(renderer, font, pauseOptions[i].label, BUTTON_UNSELECTED);
        }

        pauseOptions[i].buttonText.draw(renderer);
    }
}

void PauseInit(TTF_Font* titleFont, TTF_Font* font)
{
    // Setup background and title
    pauseContainer.w = windowWidth / 2.0f - 75;
    pauseContainer.h = windowHeight;
    pauseContainer.x = windowWidth / 2.0f - pauseContainer.w / 2.0f;
    pauseContainer.y = 0;

    pauseTitleText.dstR.w = pauseContainer.w - 100;
    pauseTitleText.dstR.h = 100;
    pauseTitleText.dstR.x = windowWidth / 2.0f - pauseTitleText.dstR.w / 2.0f;
    pauseTitleText.dstR.y = pauseTitleText.dstR.h / 2.0f;
    pauseTitleText.setText(renderer, titleFont, "Paused", TITLE_COLOR);

    pauseLabelLargest = 0.0f;
    // Setup buttons
    for (int i = 0; i < PAUSE_NUM_OPTIONS; ++i) {
        pauseOptions[i].label = pauseLabels[i];
        pauseOptions[i].menuType = pauseMenuTypes[i];
        pauseOptions[i].selected = false;
        pauseOptions[i].buttonText.dstR.w = strlen(pauseOptions[i].label.c_str()) * LETTER_WIDTH;
        pauseOptions[i].buttonText.dstR.h = 50;
        CalculateButtonPosition(&pauseOptions[i].buttonText.dstR, i, PAUSE_NUM_OPTIONS, windowWidth, windowHeight, PAUSE_MENU_BUTTON_PADDING);
        pauseOptions[i].buttonText.dstR.y += pauseTitleText.dstR.h;
        pauseOptions[i].buttonText.setText(renderer, font, pauseOptions[i].label, BUTTON_UNSELECTED);

        pauseLabelLargest = std::max(pauseLabelLargest, pauseOptions[i].buttonText.dstR.w);
        ;
    }
}

void RenderGameOverMenu(TTF_Font* font)
{
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 150);
    SDL_RenderFillRectF(renderer, &gameOverContainer);
    gameOverTitleText.draw(renderer);

    for (int i = 0; i < GAMEOVER_NUM_OPTIONS; ++i) {
        if (gameOverOptions[i].selected) {
            gameOverOptions[i].buttonText.setText(renderer, font, gameOverOptions[i].label, BUTTON_SELECTED);
            uiSelector.x = windowWidth / 2.0f - gameOverLargest / 2.0f - uiSelector.w - POINTER_OFFSET;
            uiSelector.y = gameOverOptions[i].buttonText.dstR.y;
            SDL_RenderCopyF(renderer, uiSelectedT, 0, &uiSelector);
        }
        else {
            gameOverOptions[i].buttonText.setText(renderer, font, gameOverOptions[i].label, BUTTON_UNSELECTED);
        }

        gameOverOptions[i].buttonText.draw(renderer);
    }
}

void GameOverInit(TTF_Font* titleFont, TTF_Font* font)
{
    // Setup background and title
    gameOverContainer.w = windowWidth;
    gameOverContainer.h = windowHeight;
    gameOverContainer.x = 0;
    gameOverContainer.y = 0;

    gameOverTitleText.dstR.w = gameOverContainer.w - 300;
    gameOverTitleText.dstR.h = 100;
    gameOverTitleText.dstR.x = windowWidth / 2.0f - gameOverTitleText.dstR.w / 2.0f;
    gameOverTitleText.dstR.y = gameOverTitleText.dstR.h / 2.0f + 100;
    gameOverTitleText.setText(renderer, titleFont, "GAME OVER", TITLE_COLOR);

    gameOverLargest = 0.0f;
    // Setup buttons
    for (int i = 0; i < GAMEOVER_NUM_OPTIONS; ++i) {
        gameOverOptions[i].label = gameOverLabels[i];
        gameOverOptions[i].menuType = gameOverMenuTypes[i];
        gameOverOptions[i].selected = false;
        gameOverOptions[i].buttonText.dstR.w = strlen(gameOverOptions[i].label.c_str()) * LETTER_WIDTH;
        gameOverOptions[i].buttonText.dstR.h = 50;
        CalculateButtonPosition(&gameOverOptions[i].buttonText.dstR, i, GAMEOVER_NUM_OPTIONS, windowWidth, windowHeight, GAMEOVER_MENU_BUTTON_PADDING);
        gameOverOptions[i].buttonText.dstR.y += gameOverTitleText.dstR.h;
        gameOverOptions[i].buttonText.setText(renderer, font, gameOverOptions[i].label, BUTTON_UNSELECTED);

        gameOverLargest = std::max(gameOverLargest, gameOverOptions[i].buttonText.dstR.w);;
    }
}

void RenderControlsMenu(TTF_Font* font)
{
    SDL_RenderCopyF(renderer, controlsT, 0, 0);
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRectF(renderer, &controlsTitleContainer);

    SDL_SetRenderDrawColor(renderer, 60, 60, 60, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRectF(renderer, &controlsOptionContainer);

    controlsTitleText.draw(renderer);

    if (controlsOptions.selected) {
        controlsOptions.buttonText.setText(renderer, font, controlsOptions.label, BUTTON_SELECTED);
    }
    else {
        controlsOptions.buttonText.setText(renderer, font, controlsOptions.label, BUTTON_UNSELECTED);
    }

    controlsOptions.buttonText.draw(renderer);
}

void ControlsInit(TTF_Font* titleFont, TTF_Font* font)
{
    // Setup background and title
    controlsTitleContainer.w = 500;
    controlsTitleContainer.h = 100;
    controlsTitleContainer.x = windowWidth / 2.0f - controlsTitleContainer.w / 2.0f;
    controlsTitleContainer.y = 0;

    controlsOptionContainer.w = strlen(controlsLabel.c_str()) * LETTER_WIDTH + 50;
    controlsOptionContainer.h = 75;
    controlsOptionContainer.x = 20;
    controlsOptionContainer.y = windowHeight - controlsOptionContainer.h - 20;

    controlsTitleText.dstR.w = 450;
    controlsTitleText.dstR.h = 75;
    controlsTitleText.dstR.x = windowWidth / 2.0f - controlsTitleText.dstR.w / 2.0f;
    controlsTitleText.dstR.y = controlsTitleContainer.h / 2.0f - controlsTitleText.dstR.h / 2.0f;
    controlsTitleText.setText(renderer, titleFont, "Controls", TITLE_COLOR);

    controlsOptions.label = controlsLabel;
    controlsOptions.menuType = controlsMenuType;
    controlsOptions.selected = false;
    controlsOptions.buttonText.dstR.w = strlen(controlsOptions.label.c_str()) * LETTER_WIDTH;
    controlsOptions.buttonText.dstR.h = 50;
    controlsOptions.buttonText.dstR.x = controlsOptionContainer.x + controlsOptionContainer.w / 2.0f - controlsOptions.buttonText.dstR.w / 2.0f;
    controlsOptions.buttonText.dstR.y = controlsOptionContainer.y + controlsOptionContainer.h / 2.0f - controlsOptions.buttonText.dstR.h / 2.0f;
    controlsOptions.buttonText.setText(renderer, font, controlsOptions.label, BUTTON_UNSELECTED);
}

void HandleMenuOption(MenuOption option)
{
    switch (option) {
    case MenuOption::Play:
        gameRunning = true;
        audioManager->PlayMusic(MusicAudio::Background);
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
        audioManager->PlayMusic(MusicAudio::Background);
        break;
    case MenuOption::Resume:
        pausing = false;
        pauseKeyHeld = false;
        audioManager->ResumeMusic();
        break;
    case MenuOption::Controls:
        gameRunning = false;
        controlsMenu = true;
        menuRunning = true;
        break;
    case MenuOption::Highscores:
        gameRunning = false;
        break;
    case MenuOption::Main:
        gameRunning = false;
        restart = false;
        menuRunning = false;
        audioManager->StopMusic();
        break;
    case MenuOption::Quit:
        gameRunning = false;
        appRunning = false;
        menuRunning = false;
        audioManager->StopMusic();
        audioManager->Release();
        break;
    }
}

void menuMainLoop()
{
    SDL_Event event;

    SDL_FRect extendedWindowR;
    extendedWindowR.w = windowWidth + 5;
    extendedWindowR.h = windowHeight + 5;
    extendedWindowR.x = -5;
    extendedWindowR.y = -5;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            menuRunning = false;
            appRunning = false;
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
            SDL_RenderSetScale(renderer, event.window.data1 / (float)windowWidth, event.window.data2 / (float)windowHeight);
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            buttons[event.button.button] = true;
            if (controlsMenu) {
                if (SDL_PointInFRect(&mousePos, &controlsOptions.buttonText.dstR)) {
                    audioManager->PlaySFX(SFXAudio::UI);
                    controlsMenu = false;
                    HandleMenuOption(controlsOptions.menuType);
                }
            }
        }
        if (event.type == SDL_MOUSEBUTTONUP) {
            buttons[event.button.button] = false;
        }
        if (event.type == SDL_MOUSEMOTION) {
            float scaleX, scaleY;
            SDL_RenderGetScale(renderer, &scaleX, &scaleY);
            mousePos.x = event.motion.x / scaleX;
            mousePos.y = event.motion.y / scaleY;
            realMousePos.x = event.motion.x;
            realMousePos.y = event.motion.y;
            if (controlsMenu) {
                if (SDL_PointInFRect(&mousePos, &controlsOptions.buttonText.dstR)) {
                    controlsOptions.selected = true;
                }
                else {
                    controlsOptions.selected = false;
                }
            }
        }
    }
    RenderAll();
}
#pragma endregion

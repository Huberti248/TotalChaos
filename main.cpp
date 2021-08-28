#include "SdlDefinitions.h"

Player player;
SDL_Texture* playerT;
SDL_Texture* bgT;
SDL_Texture* bulletT;
SDL_Texture* redBulletT;
SDL_Texture* purpleBulletT;
SDL_Texture* enemyT;
SDL_Texture* planetT;
SDL_Texture* meatT;
SDL_Texture* buyT;
SDL_Texture* shieldT;
SDL_Texture* closeT;
SDL_Texture* coinsT;
SDL_Texture* uiSelectedT;
SDL_Texture* portalT;
SDL_Texture* weaponPlanetT;
SDL_Texture* shotgunT;
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
Text shotgunPriceText;
Text shotgunText;
SDL_FRect buyShotgunR;
SDL_FRect buyShotgunBtnR;
bool buyingShotgun = false;
bool hasShotgun = false;

void WindowInit();

void mainLoop();

void TexturesInit();

void UiInit();

void ClocksInit();

void BounceOff(Entity* a, Entity* b, bool affectB);

MenuOption displayMainMenu(SDL_Renderer* rendererMenu, TTF_Font* fontMenu, SDL_Point* mousePosMenu);

void InputEvents(const SDL_Event& event);

void EnemySpawn();

void EntityMovement(const SDL_FRect& extendedWindowR, float deltaTime);

void BulletCollisions(const SDL_FRect& extendedWindowR, float deltaTime);

void EnemyBehavior(const SDL_FRect& extendedWindowR, float deltaTime);

void PowerUpSpawner();

void RenderAll();

void FireWhenReady();

void mainLoop()
{
    float deltaTime = globalClock.restart();
    SDL_Event event;

    SDL_FRect extendedWindowR;
    extendedWindowR.w = windowWidth + 5;
    extendedWindowR.h = windowHeight + 5;
    extendedWindowR.x = -5;
    extendedWindowR.y = -5;

    while (SDL_PollEvent(&event)) {
        InputEvents(event);
    }
    if (!buyingShield && !buyingShotgun) {
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

        EnemySpawn();

        EntityMovement(extendedWindowR, deltaTime);

        BulletCollisions(extendedWindowR, deltaTime);

        EnemyBehavior(extendedWindowR, deltaTime);

        PowerUpSpawner();

        if (portalClock.getElapsedTime() > PORTAL_SPAWN_DELAY_IN_MS) {
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
                }
                else {
                    portalRects.erase(portalRects.begin() + i);
                    portalRects.erase(portalRects.begin() + i--);
                }
            }
        }
    }
    RenderAll();
}

int main(int argc, char* argv[])
{
    WindowInit();
    TexturesInit();

    player.r.w = 32;
    player.r.h = 32;
    player.r.x = windowWidth / 2 - player.r.w / 2;
    player.r.y = windowHeight / 2 - player.r.h / 2;

    UiInit();
    ClocksInit();

    MenuOption buttonType = displayMainMenu(renderer, robotoF, &realMousePos);
    if (buttonType == MenuOption::Quit) {
        running = false;
    }
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    while (running) {
        mainLoop();
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
    buyT = IMG_LoadTexture(renderer, "res/buy.png");
    shieldT = IMG_LoadTexture(renderer, "res/shield.png");
    closeT = IMG_LoadTexture(renderer, "res/close.png");
    coinsT = IMG_LoadTexture(renderer, "res/coins.png");
    uiSelectedT = IMG_LoadTexture(renderer, "res/player.png");
    portalT = IMG_LoadTexture(renderer, "res/portal.png");
    weaponPlanetT = IMG_LoadTexture(renderer, "res/weaponPlanet.png");
    shotgunT = IMG_LoadTexture(renderer, "res/shotgun.png");
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
    shieldHealthText.setText(renderer, robotoF, sText, { 255, 0, 0 });

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

    moneyText.setText(renderer, robotoF, 100);
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
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    window = SDL_CreateWindow("TotalChaos", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    robotoF = TTF_OpenFont("res/roboto.ttf", 72);
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_RenderSetScale(renderer, w / (float)windowWidth, h / (float)windowHeight);
    SDL_AddEventWatch(eventWatch, 0);
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

MenuOption displayMainMenu(SDL_Renderer* rendererMenu, TTF_Font* fontMenu, SDL_Point* mousePosMenu)
{
    const int NUMMENU = 2;
    MenuButton buttons[NUMMENU];
    const std::string labels[NUMMENU] = { "Play", "Exit" };
    const MenuOption menuTypes[NUMMENU] = { MenuOption::Play, MenuOption::Quit };
    SDL_Color color[2] = { { 255, 255, 255 }, { 255, 0, 0 } };

    // Setup background and title
    SDL_SetRenderDrawColor(rendererMenu, 0, 0, 0, 1);

    Text titleText;
    titleText.dstR.w = windowWidth - 100;
    titleText.dstR.h = 100;
    titleText.dstR.x = windowWidth / 2.0f - titleText.dstR.w / 2.0f;
    titleText.dstR.y = titleText.dstR.h / 2.0f;
    titleText.setText(rendererMenu, fontMenu, "Total Chaos In Space", MAIN_MENU_COLOR);

    // Setup buttons
    for (int i = 0; i < NUMMENU; ++i) {
        buttons[i].label = labels[i];
        buttons[i].menuType = menuTypes[i];
        buttons[i].selected = false;
        buttons[i].buttonText.dstR.w = 100;
        buttons[i].buttonText.dstR.h = 40;
        CalculateButtonPosition(&buttons[i].buttonText.dstR, i, NUMMENU, windowWidth, windowHeight, MAIN_MENU_BUTTON_PADDING);
        buttons[i].buttonText.dstR.y += titleText.dstR.h;
        buttons[i].buttonText.setText(rendererMenu, fontMenu, buttons[i].label, color[0]);
    }

    // Setup pointer by selected button.
    SDL_FRect uiSelectedR;
    uiSelectedR.w = 32;
    uiSelectedR.h = 32;

    SDL_Event eventMenu;
    while (true) {
        SDL_RenderClear(rendererMenu);
        while (SDL_PollEvent(&eventMenu)) {
            switch (eventMenu.type) {
            case SDL_QUIT:
                return MenuOption::Quit;
            case SDL_WINDOWEVENT:
                if (eventMenu.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SDL_RenderSetScale(rendererMenu, eventMenu.window.data1 / (float)windowWidth, eventMenu.window.data2 / (float)windowHeight);
                }
                break;
            case SDL_MOUSEMOTION:
                mousePosMenu->x = eventMenu.motion.x;
                mousePosMenu->y = eventMenu.motion.y;
                for (int i = 0; i < NUMMENU; ++i) {
                    if (SDL_PointInFRect(mousePosMenu, &buttons[i].buttonText.dstR)) {
                        if (!buttons[i].selected) {
                            buttons[i].selected = true;
                            buttons[i].buttonText.setText(rendererMenu, fontMenu, buttons[i].label, color[1]);
                        }
                    }
                    else {
                        if (buttons[i].selected) {
                            buttons[i].selected = false;
                            buttons[i].buttonText.setText(rendererMenu, fontMenu, buttons[i].label, color[0]);
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                mousePosMenu->x = eventMenu.motion.x;
                mousePosMenu->y = eventMenu.motion.y;
                for (int i = 0; i < NUMMENU; ++i) {
                    if (SDL_PointInFRect(mousePosMenu, &buttons[i].buttonText.dstR)) {
                        return buttons[i].menuType;
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (eventMenu.key.keysym.sym == SDLK_ESCAPE) {
                    return MenuOption::Quit;
                }
                break;
            }
        }

        titleText.draw(rendererMenu);
        for (MenuButton button : buttons) {
            if (button.selected) {
                uiSelectedR.x = button.buttonText.dstR.x - button.buttonText.dstR.w / 2.0f - uiSelectedR.w / 2.0f;
                uiSelectedR.y = button.buttonText.dstR.y;
                SDL_RenderCopyF(rendererMenu, uiSelectedT, 0, &uiSelectedR);
            }
            button.buttonText.draw(rendererMenu);
        }

        SDL_RenderPresent(rendererMenu);
    }
}

void InputEvents(const SDL_Event& event)
{
    if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        running = false;
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
    player.r.x = std::clamp(player.r.x, 0.0f, windowWidth - player.r.w);
    player.r.y = std::clamp(player.r.y, 0.0f, windowHeight - player.r.h);

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
                enemies.erase(enemies.begin() + j--);
                bullets.erase(bullets.begin() + i--);
                player.streak++;
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
}

void RenderAll()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderCopyF(renderer, bgT, 0, 0);
    if (hasShield) {
        SDL_RenderCopyF(renderer, shieldT, 0, &player.r);
    }
    RotateEntityTowards(playerT, player, mousePos, renderer);
    //SDL_RenderCopyF(renderer, playerT, 0, &player.r);

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
    if (hasShield)
        shieldHealthText.draw(renderer);

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
    SDL_RenderCopyF(renderer, coinsT, 0, &moneyR);
    moneyText.draw(renderer);
    for (int i = 0; i < portalRects.size(); ++i) {
        SDL_RenderCopyF(renderer, portalT, 0, &portalRects[i]);
    }
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
    SDL_RenderPresent(renderer);
}

void FireWhenReady()
{
    if (bulletClock.getElapsedTime() > BULLET_SPAWN_DELAY_IN_MS) {
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

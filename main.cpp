#include "Definitions.h"

void logOutputCallback(void* userdata, int category, SDL_LogPriority priority, const char* message)
{
	std::cout << message << std::endl;
}

int random(int min, int max)
{
	return min + rand() % ((max + 1) - min);
}

int SDL_QueryTextureF(SDL_Texture* texture, Uint32* format, int* access, float* w, float* h)
{
	int wi, hi;
	int result = SDL_QueryTexture(texture, format, access, &wi, &hi);
	if (w) {
		*w = wi;
	}
	if (h) {
		*h = hi;
	}
	return result;
}

SDL_bool SDL_PointInFRect(const SDL_Point* p, const SDL_FRect* r)
{
	return ((p->x >= r->x) && (p->x < (r->x + r->w)) && (p->y >= r->y) && (p->y < (r->y + r->h))) ? SDL_TRUE : SDL_FALSE;
}

std::ostream& operator<<(std::ostream& os, SDL_FRect r)
{
	os << r.x << " " << r.y << " " << r.w << " " << r.h;
	return os;
}

std::ostream& operator<<(std::ostream& os, SDL_Rect r)
{
	SDL_FRect fR;
	fR.w = r.w;
	fR.h = r.h;
	fR.x = r.x;
	fR.y = r.y;
	os << fR;
	return os;
}

SDL_Texture* renderText(SDL_Texture* previousTexture, TTF_Font* font, SDL_Renderer* renderer, const std::string& text, SDL_Color color)
{
	if (previousTexture) {
		SDL_DestroyTexture(previousTexture);
	}
	SDL_Surface* surface;
	if (text.empty()) {
		surface = TTF_RenderUTF8_Blended(font, " ", color);
	}
	else {
		surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
	}
	if (surface) {
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		return texture;
	}
	else {
		return 0;
	}
}

struct Text {
	std::string text;
	SDL_Surface* surface = 0;
	SDL_Texture* t = 0;
	SDL_FRect dstR{};
	bool autoAdjustW = false;
	bool autoAdjustH = false;
	float wMultiplier = 1;
	float hMultiplier = 1;

	~Text()
	{
		if (surface) {
			SDL_FreeSurface(surface);
		}
		if (t) {
			SDL_DestroyTexture(t);
		}
	}

	Text()
	{
	}

	Text(const Text& rightText)
	{
		text = rightText.text;
		if (surface) {
			SDL_FreeSurface(surface);
		}
		if (t) {
			SDL_DestroyTexture(t);
		}
		if (rightText.surface) {
			surface = SDL_ConvertSurface(rightText.surface, rightText.surface->format, SDL_SWSURFACE);
		}
		if (rightText.t) {
			t = SDL_CreateTextureFromSurface(renderer, surface);
		}
		dstR = rightText.dstR;
		autoAdjustW = rightText.autoAdjustW;
		autoAdjustH = rightText.autoAdjustH;
		wMultiplier = rightText.wMultiplier;
		hMultiplier = rightText.hMultiplier;
	}

	Text& operator=(const Text& rightText)
	{
		text = rightText.text;
		if (surface) {
			SDL_FreeSurface(surface);
		}
		if (t) {
			SDL_DestroyTexture(t);
		}
		if (rightText.surface) {
			surface = SDL_ConvertSurface(rightText.surface, rightText.surface->format, SDL_SWSURFACE);
		}
		if (rightText.t) {
			t = SDL_CreateTextureFromSurface(renderer, surface);
		}
		dstR = rightText.dstR;
		autoAdjustW = rightText.autoAdjustW;
		autoAdjustH = rightText.autoAdjustH;
		wMultiplier = rightText.wMultiplier;
		hMultiplier = rightText.hMultiplier;
		return *this;
	}

	void setText(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color c = { 255, 255, 255 })
	{
		this->text = text;
#if 1 // NOTE: renderText
		if (surface) {
			SDL_FreeSurface(surface);
		}
		if (t) {
			SDL_DestroyTexture(t);
		}
		if (text.empty()) {
			surface = TTF_RenderUTF8_Blended(font, " ", c);
		}
		else {
			surface = TTF_RenderUTF8_Blended(font, text.c_str(), c);
		}
		if (surface) {
			t = SDL_CreateTextureFromSurface(renderer, surface);
		}
#endif
		if (autoAdjustW) {
			SDL_QueryTextureF(t, 0, 0, &dstR.w, 0);
		}
		if (autoAdjustH) {
			SDL_QueryTextureF(t, 0, 0, 0, &dstR.h);
		}
		dstR.w *= wMultiplier;
		dstR.h *= hMultiplier;
	}

	void setText(SDL_Renderer* renderer, TTF_Font* font, int value, SDL_Color c = { 255, 255, 255 })
	{
		setText(renderer, font, std::to_string(value), c);
	}

	void draw(SDL_Renderer* renderer)
	{
		if (t) {
			SDL_RenderCopyF(renderer, t, 0, &dstR);
		}
	}
};

int SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius)
{
	int offsetx, offsety, d;
	int status;

	offsetx = 0;
	offsety = radius;
	d = radius - 1;
	status = 0;

	while (offsety >= offsetx) {
		status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
		status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
		status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
		status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
		status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
		status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
		status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
		status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

		if (status < 0) {
			status = -1;
			break;
		}

		if (d >= 2 * offsetx) {
			d -= 2 * offsetx + 1;
			offsetx += 1;
		}
		else if (d < 2 * (radius - offsety)) {
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else {
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}

	return status;
}

int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius)
{
	int offsetx, offsety, d;
	int status;

	offsetx = 0;
	offsety = radius;
	d = radius - 1;
	status = 0;

	while (offsety >= offsetx) {

		status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
			x + offsety, y + offsetx);
		status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
			x + offsetx, y + offsety);
		status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
			x + offsetx, y - offsety);
		status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
			x + offsety, y - offsetx);

		if (status < 0) {
			status = -1;
			break;
		}

		if (d >= 2 * offsetx) {
			d -= 2 * offsetx + 1;
			offsetx += 1;
		}
		else if (d < 2 * (radius - offsety)) {
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else {
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}

	return status;
}

struct Clock {
	Uint64 start = SDL_GetPerformanceCounter();

	float getElapsedTime()
	{
		Uint64 stop = SDL_GetPerformanceCounter();
		float secondsElapsed = (stop - start) / (float)SDL_GetPerformanceFrequency();
		return secondsElapsed * 1000;
	}

	float restart()
	{
		Uint64 stop = SDL_GetPerformanceCounter();
		float secondsElapsed = (stop - start) / (float)SDL_GetPerformanceFrequency();
		start = SDL_GetPerformanceCounter();
		return secondsElapsed * 1000;
	}
};

SDL_bool SDL_FRectEmpty(const SDL_FRect* r)
{
	return ((!r) || (r->w <= 0) || (r->h <= 0)) ? SDL_TRUE : SDL_FALSE;
}

SDL_bool SDL_IntersectFRect(const SDL_FRect* A, const SDL_FRect* B, SDL_FRect* result)
{
	int Amin, Amax, Bmin, Bmax;

	if (!A) {
		SDL_InvalidParamError("A");
		return SDL_FALSE;
	}

	if (!B) {
		SDL_InvalidParamError("B");
		return SDL_FALSE;
	}

	if (!result) {
		SDL_InvalidParamError("result");
		return SDL_FALSE;
	}

	/* Special cases for empty rects */
	if (SDL_FRectEmpty(A) || SDL_FRectEmpty(B)) {
		result->w = 0;
		result->h = 0;
		return SDL_FALSE;
	}

	/* Horizontal intersection */
	Amin = A->x;
	Amax = Amin + A->w;
	Bmin = B->x;
	Bmax = Bmin + B->w;
	if (Bmin > Amin)
		Amin = Bmin;
	result->x = Amin;
	if (Bmax < Amax)
		Amax = Bmax;
	result->w = Amax - Amin;

	/* Vertical intersection */
	Amin = A->y;
	Amax = Amin + A->h;
	Bmin = B->y;
	Bmax = Bmin + B->h;
	if (Bmin > Amin)
		Amin = Bmin;
	result->y = Amin;
	if (Bmax < Amax)
		Amax = Bmax;
	result->h = Amax - Amin;

	return (SDL_FRectEmpty(result) == SDL_TRUE) ? SDL_FALSE : SDL_TRUE;
}

SDL_bool SDL_HasIntersectionF(const SDL_FRect* A, const SDL_FRect* B)
{
	int Amin, Amax, Bmin, Bmax;

	if (!A) {
		SDL_InvalidParamError("A");
		return SDL_FALSE;
	}

	if (!B) {
		SDL_InvalidParamError("B");
		return SDL_FALSE;
	}

	/* Special cases for empty rects */
	if (SDL_FRectEmpty(A) || SDL_FRectEmpty(B)) {
		return SDL_FALSE;
	}

	/* Horizontal intersection */
	Amin = A->x;
	Amax = Amin + A->w;
	Bmin = B->x;
	Bmax = Bmin + B->w;
	if (Bmin > Amin)
		Amin = Bmin;
	if (Bmax < Amax)
		Amax = Bmax;
	if (Amax <= Amin)
		return SDL_FALSE;

	/* Vertical intersection */
	Amin = A->y;
	Amax = Amin + A->h;
	Bmin = B->y;
	Bmax = Bmin + B->h;
	if (Bmin > Amin)
		Amin = Bmin;
	if (Bmax < Amax)
		Amax = Bmax;
	if (Amax <= Amin)
		return SDL_FALSE;

	return SDL_TRUE;
}

int eventWatch(void* userdata, SDL_Event* event)
{
	// WARNING: Be very careful of what you do in the function, as it may run in a different thread
	if (event->type == SDL_APP_TERMINATING || event->type == SDL_APP_WILLENTERBACKGROUND) {
	}
	return 0;
}

Player player; //Note: Default player health = 5
SDL_Texture* playerT;
SDL_Texture* bgT;
SDL_Texture* bulletT;
SDL_Texture* enemyT;
Clock globalClock;
std::vector<Bullet> bullets;
Clock bulletClock;
Clock enemyClock;
std::vector<Enemy> enemies;
Text killPointsText;
Text healthText;

void mainLoop()
{
	float deltaTime = globalClock.restart();
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
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
			bulletClock.restart();
		}
	}
	for (int i = 0; i < bullets.size(); ++i) {
		bullets[i].r.y += bullets[i].dy * deltaTime * BULLET_SPEED;
		bullets[i].r.x += bullets[i].dx * deltaTime * BULLET_SPEED;
	}
	player.r.x += player.dx * deltaTime * PLAYER_SPEED;
	player.r.y += player.dy * deltaTime * PLAYER_SPEED;
	player.r.x = std::clamp(player.r.x, 0.0f, windowWidth - player.r.w);
	player.r.y = std::clamp(player.r.y, 0.0f, windowHeight - player.r.h);
	for (int i = 0; i < bullets.size(); ++i) {
		if (bullets[i].r.y + bullets[i].r.h < 0) {
			bullets.erase(bullets.begin() + i--);
		}
	}
	if (enemyClock.getElapsedTime() > ENEMY_SPAWN_DELAY_IN_MS) {
		enemies.push_back(Enemy());
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
	for (int i = 0; i < enemies.size(); ++i) {
		enemies[i].r.x += enemies[i].dx * deltaTime * ENEMY_SPEED;
		enemies[i].r.y += enemies[i].dy * deltaTime * ENEMY_SPEED;
	}

deleteCollidingBegin:
	for (int i = 0; i < bullets.size(); ++i) {
		//Player collision (NOT TESTED YET)
		if ((bullets[i].GetTargetMask() & TargetMask::PlayerMask) != 0) {
			if (SDL_HasIntersectionF(&bullets[i].r, &player.r)) {
				player.health--;
				bullets.erase(bullets.begin() + i--);
				healthText.setText(renderer, robotoF, player.health, { 255, 0, 0 });
				goto deleteCollidingBegin;
			}
		}

		//Enemy collision
		for (int j = 0; j < enemies.size(); ++j) {
			//Layer control
			if (bullets[i].GetTargetMask() < TargetMask::EnemiesMask) continue;
			if (SDL_HasIntersectionF(&bullets[i].r, &enemies[j].r)) {
				enemies.erase(enemies.begin() + j--);
				bullets.erase(bullets.begin() + i--);
				killPointsText.setText(renderer, robotoF, std::stoi(killPointsText.text) + 1);
				goto deleteCollidingBegin;
			}
		}
	}
	for (int i = 0; i < enemies.size(); ++i) {
		SDL_FRect windowR;
		windowR.w = windowWidth;
		windowR.h = windowHeight;
		windowR.x = 0;
		windowR.y = 0;
		SDL_FRect enemyR = enemies[i].r;
		--enemyR.x;
		--enemyR.y;
		enemyR.w += 2;
		enemyR.h += 2;
		if (!SDL_HasIntersectionF(&enemies[i].r, &windowR)) {
			enemies.erase(enemies.begin() + i--);
		}
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_RenderCopyF(renderer, bgT, 0, 0);
	PlayerRotation(playerT, player, mousePos, renderer);
	//SDL_RenderCopyF(renderer, playerT, 0, &player.r);
	for (int i = 0; i < bullets.size(); ++i) {
		SDL_RenderCopyF(renderer, bulletT, 0, &bullets[i].r);
	}
	for (int i = 0; i < enemies.size(); ++i) {
		//Based on the enemies' spawn position, rotate them to the proper angle
		double angles[] = { 180.0, 0.0, 90.0, 270.0 }; // Array of rotations mapped to the enum of spawn positions
		int index = (int)enemies[i].spawnPlace;
		SDL_RenderCopyExF(renderer, enemyT, 0, &enemies[i].r, angles[index], 0, SDL_FLIP_NONE);
	}
	killPointsText.draw(renderer);
	healthText.draw(renderer);

	SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[])
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
	playerT = IMG_LoadTexture(renderer, "res/player.png");
	bgT = IMG_LoadTexture(renderer, "res/bg.png");
	bulletT = IMG_LoadTexture(renderer, "res/bullet.png");
	enemyT = IMG_LoadTexture(renderer, "res/enemy.png");
	player.r.w = 32;
	player.r.h = 32;
	player.r.x = windowWidth / 2 - player.r.w / 2;
	player.r.y = windowHeight - player.r.h;
	killPointsText.dstR.w = 30;
	killPointsText.dstR.h = 20;
	killPointsText.dstR.x = windowWidth / 2 - killPointsText.dstR.w / 2;
	killPointsText.dstR.y = 0;
	killPointsText.setText(renderer, robotoF, "0");
	healthText.dstR.w = 30;
	healthText.dstR.h = 20;
	healthText.dstR.x = windowWidth - healthText.dstR.w;
	healthText.dstR.y = 0;
	healthText.setText(renderer, robotoF, player.health, { 255, 0, 0 });
	globalClock.restart();
	bulletClock.restart();
	enemyClock.restart();
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
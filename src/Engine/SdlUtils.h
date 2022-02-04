#pragma once
//Rename to SdlDefinitions once we get rid of the window ptr that's over there
#include "SystemImports.h"
#include "SdlImports.h"
#include "../UI/Menu.h"

class SdlUtils {
public:
	static SDL_Renderer* renderer;

	SdlUtils() {};
	~SdlUtils() {};

	static int SDL_QueryTextureF(SDL_Texture* texture, Uint32* format, int* access, float* w, float* h);

	static SDL_bool SDL_PointInFRect(const SDL_Point* p, const SDL_FRect* r);

	static SDL_Texture* renderText(SDL_Texture* previousTexture, TTF_Font* font, SDL_Renderer* renderer, const std::string& text, SDL_Color color);

	static int SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius);

	static int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius);

	static SDL_bool SDL_FRectEmpty(const SDL_FRect* r);

	static SDL_bool SDL_IntersectFRect(const SDL_FRect* A, const SDL_FRect* B, SDL_FRect* result);

	static SDL_bool SDL_HasIntersectionF(const SDL_FRect* A, const SDL_FRect* B);

	static int eventWatch(void* userdata, SDL_Event* event);
};

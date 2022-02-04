#pragma once
#include <fstream>
#include <map>
#include <string>
#include "../Engine/SdlImports.h"

#define TEXTURES_PATH "res/texturePaths.dat"
#define TEXTURES_PATHS_LENGTH 7

class TextureLoader {
public:
	TextureLoader() {};
	~TextureLoader() {};

	static void LoadEntityTextures(SDL_Renderer* renderer);

	static SDL_Texture* GetTextureByName(const char* name);

private:
	//TODO: refactor this with enums in the enemies' and planets textures
	static inline std::map<const char*, SDL_Texture*> nameAndTexturesMap = {
		{"Player", nullptr},
		{"BlueBullet", nullptr},
		{"RedBullet", nullptr},
		{"PurpleBullet", nullptr},
		{"Enemy", nullptr},
		{"ShieldPlanet", nullptr},
		{"WeaponPlanet", nullptr},
		{"Background", nullptr},
		{"Meat", nullptr},
		{"Gun", nullptr},
		{"Portal", nullptr},
		{"Buy", nullptr},
		{"Shield", nullptr},
		{"Close", nullptr},
		{"Coin", nullptr},
		{"Shotgun", nullptr},
		{"ControlsMenu", nullptr},
		{"HighScoresMenu", nullptr},
		{"Main", nullptr}
	};
};
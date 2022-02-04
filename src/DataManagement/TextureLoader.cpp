#include "TextureLoader.h"

void TextureLoader::LoadEntityTextures(SDL_Renderer* renderer) {
	//Read the file into a buffer for each line
	//And then call IMG_LOAD whatever to the ptr assigned
	std::ifstream texturePathsFile;
	texturePathsFile.open(TEXTURES_PATH);
	std::string buffer;
	for (auto& [k, v] : nameAndTexturesMap) {
		if (!std::getline(texturePathsFile, buffer))
			break;
		nameAndTexturesMap[k] = IMG_LoadTexture(renderer, buffer.c_str());
	}
	texturePathsFile.close();
}

SDL_Texture* TextureLoader::GetTextureByName(const char* name) {
	return TextureLoader::nameAndTexturesMap[name];
}

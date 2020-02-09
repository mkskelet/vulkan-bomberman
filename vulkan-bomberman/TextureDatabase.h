#pragma once

#include <map>

#include "Texture.h"

class TextureDatabase
{
private:
	std::map<const char*, Texture*> textures;

public:
	TextureDatabase() : textures({}) {};
	void ReleaseTextures();
	void ReleaseTexture(Texture* texture);
	Texture* GetTexture(const char* path);
	static TextureDatabase& GetInstance();
};


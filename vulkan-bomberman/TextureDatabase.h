#pragma once

#include <map>

#include "Texture.h"

class TextureDatabase
{
private:
	std::map<const char*, Texture*> textures;

public:
	TextureDatabase() : textures({}) {};
	~TextureDatabase();
	Texture* GetTexture(const char* path);
};


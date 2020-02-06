#include "TextureDatabase.h"

void TextureDatabase::ReleaseTextures()
{
	for (auto const& [path, tex] : textures)
	{
		delete tex;
	}
}

Texture* TextureDatabase::GetTexture(const char* path)
{
	auto i = textures.find(path);

	if (i != textures.end())
	{
		return i->second;
	}
	else
	{
		Texture* t = new Texture(path);
		textures[path] = t;
		return t;
	}
}

TextureDatabase& TextureDatabase::GetInstance()
{
	static TextureDatabase instance;
	return instance;
}
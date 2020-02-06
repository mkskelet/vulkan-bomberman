#include "TextureDatabase.h"

TextureDatabase::~TextureDatabase()
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
	}
}
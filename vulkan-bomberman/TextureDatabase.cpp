#include "TextureDatabase.h"
#include "Say.h"

void TextureDatabase::ReleaseTextures()
{
	for (auto const& [path, tex] : textures)
	{
		delete tex;
	}
}

void TextureDatabase::ReleaseTexture(Texture* texture)
{
	for (auto const& [path, tex] : textures)
	{
		if (tex == texture)
		{
			tex->DecrementRefCount();

			if (tex->GetRefCount() <= 0)
			{
				Say::Log("Releasing texture", path != nullptr ? path : "null");
				delete tex;
				textures.erase(path);
			}
			break;
		}
	}
}

Texture* TextureDatabase::GetTexture(const char* path)
{
	auto i = textures.find(path);

	if (i != textures.end())
	{
		i->second->IncrementRefCount();
		return i->second;
	}
	else
	{
		Texture* t = new Texture(path);
		textures[path] = t;
		t->IncrementRefCount();
		return t;
	}
}

TextureDatabase& TextureDatabase::GetInstance()
{
	static TextureDatabase instance;
	return instance;
}
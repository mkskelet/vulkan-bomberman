#include "Sprite.h"
#include "TextureDatabase.h"

std::map<Texture*, std::vector<Sprite*>> Sprite::spriteMap = {};

/// Constructor to create sprite with specified position, scale and textureID, default pivot PIVOT_CENTER is used.
Sprite::Sprite(glm::vec3 position, glm::vec2 scale, Texture* texture) {
	this->position = position;
	this->scale = scale;
	this->texture = texture;
	pivot = glm::vec2(0.5f, 0.5f);
	tiling = glm::vec2(1.0f, 1.0f);
	color = { 1.0f, 1.0f, 1.0f, 1.0f };

	AddToMap(this);
}

/// Constructor to create sprite with specified position, scale and textureID and pivot.
Sprite::Sprite(glm::vec3 position, glm::vec2 scale, Texture* texture, glm::vec2 pivot) {
	this->position = position;
	this->scale = scale;
	this->texture = texture;
	this->pivot = pivot;
	tiling = glm::vec2(1.0f, 1.0f);
	color = { 1.0f, 1.0f, 1.0f, 1.0f };

	AddToMap(this);
}

Sprite::~Sprite() 
{
	RemoveFromMap(this);
}

void Sprite::SetTexture(Texture* texture)
{
	RemoveFromMap(this);
	this->texture = texture;
	AddToMap(this);
}

void Sprite::AddToMap(Sprite* sprite)
{
	auto it = spriteMap.find(sprite->GetTexture());
	if (it != spriteMap.end())
	{
		it->second.push_back(sprite);
	}
	else
	{
		spriteMap[sprite->GetTexture()].push_back(sprite);
	}
}

void Sprite::RemoveFromMap(Sprite* sprite)
{
	auto it = spriteMap.find(sprite->GetTexture());
	if (it != spriteMap.end())
	{
		auto s = std::find(it->second.begin(), it->second.end(), sprite);

		// remove sprite from map if we found it
		if (s != it->second.end())
		{
			it->second.erase(s);
		}

		// release texture if we don't have any references to it
		if (it->second.size() == 0)
		{
			TextureDatabase::GetInstance().ReleaseTexture(it->first);
			spriteMap.erase(it->first);
		}
	}
}

void Sprite::Render() {

}

void Sprite::Start() {

}

void Sprite::Update() {

}